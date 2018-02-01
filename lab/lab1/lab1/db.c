#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "db.h"

static int maxSize = 0;
static int numRecords = 0;
static TPhonebook *database = NULL;

void initPhonebook(int maxRecords) 
{
    if (database != NULL)
        free (database);

    maxSize = maxRecords;
    numRecords = 0;

    database = (TPhonebook *) calloc(maxSize, sizeof(TPhonebook));
}

void freePhonebook() {
    if (database != NULL)
        free(database);
}

void addPerson(char *name, char *countryCode, char *phoneNumber, int *result) {
    if (numRecords >= maxSize)
        *result = MAX_REACHED;
    else {
        if (findPerson(name) != NULL)
            *result = DUPLICATE;
        else {
            //insert the relevant info into the numRecords index data
            strncpy(database[numRecords].name, name, NAME_LENGTH);
            strncpy(database[numRecords].countryCode, countryCode, C_LENGTH);
            strncpy(database[numRecords].phoneNumber, phoneNumber, NUM_LENGTH);

            database -> deleted = 0;
            database -> index = numRecords++;
            *result = OK;
        }
    }
}

TPhonebook *findPerson(char *name) {
    int i;
    int found = 0;
    TPhonebook *result = NULL;

    //Run through every record
    for (i = 0; i< numRecords && !found; i++) {
        found = (strcmp(name, database[i].name) == 0);

        if (found && !database[i].deleted)
            result = &database[i];
    }

    return result;
}

void listPhonebook(void) {
    printf("\nPHONE LISTING\n");
    printf(  "=============\n\n");

    if (database != NULL) {
        int i;

        for (i=0; i<numRecords; i++)
            if(!database[i].deleted)
                printf("%d: %s (%s)-(%s)\n",i+1, database[i].name,
                       database[i].countryCode, database[i].phoneNumber);
    } else {
        printf("*** EMPTY ***\n\n");
    }
}

int deletePerson(char *name) {
    TPhonebook *result = findPerson(name);

    if (result == NULL)
        return CANNOT_FIND;
    else {
        result -> deleted = 1;
        return OK;
    }
}

int saveDB(char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp != NULL) {
        fprintf(fp, "%d %d\n", maxSize, numRecords);

        for (int i=0; i<numRecords; i++)
            fprintf(fp, "%d\n%d\n%s\n%s\n%s\n", database[i].index, database[i].deleted, database[i].name, database[i].countryCode, database[i].phoneNumber);
        fclose(fp);
        return OK;
    } else {
        return SAVE_FAIL;
    }
}

void stripNL (char *str) {
    // strlen returns an unsigned long data type, so I changed it to unsigned long
    unsigned long NLIdx = strlen(str) - 1;

    if (str[NLIdx] == '\r' || str[NLIdx] == '\n')
        str[NLIdx] = '\0';
}

int loadDB(char *filename) {
    FILE *fp = fopen(filename, "r");

    int dbSize, dbRecords;

    if (fp != NULL) {
        fscanf(fp, "%d %d\n", &dbSize, &dbRecords);

        printf("Read %d records max DB size is %d\n", dbRecords, dbSize);

        initPhonebook(dbSize);

        for (int i=0; i<dbRecords; i++) {
            char buffer[128];

            fscanf(fp, "%d\n", &database[i].index);
            fscanf(fp, "%d\n", &database[i].deleted);
            fgets(buffer, NAME_LENGTH, fp);

            stripNL(buffer);
            printf("%d: Name: %s\n", i, buffer);
            strcpy(database[i].name, buffer);

            fgets(buffer, C_LENGTH, fp);
            stripNL(buffer);
            printf("%d: Country Code: %s\n", i, buffer);
            strcpy(database[i].countryCode, buffer);

            fgets(buffer, NUM_LENGTH, fp);
            stripNL(buffer);
            printf("%d: Phone Number: %s\n", i, buffer);
            strcpy(database[i].phoneNumber, buffer);
        }
        maxSize = dbSize;
        numRecords = dbRecords;
        return OK;
    } else {
        return LOAD_FAIL;
    }
}

void resizeDB(int numNewRecords) {
    maxSize += numNewRecords;
    database = (TPhonebook *) realloc(database, sizeof(TPhonebook) * maxSize);
}

void getDBSize(int *nr, int *ms) {
    *nr = numRecords;
    *ms = maxSize;
}
