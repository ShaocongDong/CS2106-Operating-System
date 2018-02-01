//
//  main.c
//  lab1
//
//  Created by DongShaocong on 1/2/18.
//  Copyright © 2018 nus.cs3217.a0148008. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "db.h"

//declare all the function prototypes
void flushInput(void);
void showMenu(void);
void loadPhonebook(void);
void savePhonebook(void);
void newEntry(void);
void displayEntry(void);
void listEntries(void);
void deleteEntry(void);
void readName(char *name, int maxlen);


// ac = number of arguments on command line
// av = an arra of strings containing the arguments
int main(int ac, char **av) {
    if (ac != 2) {
        printf("\nUsage: %s <num record>\n\n", av[0]);
        exit(-1);
    }

    int numRecords = strtol(av[1], NULL, 10);

    if (numRecords < 1) {
        printf("Number of records must be a positive integer. You gave %d\n", numRecords);
        return -1;
    }

    initPhonebook(numRecords);
    printf("Hello welcome to Phonebook\n");
    showMenu();
    printf("\nGoodBye!");

    return 0;

}

void flushInput() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readName(char *name, int maxlen) {
    fgets(name, maxlen, stdin);
    name[strlen(name)-1] = '\0';
}

void showMenu() {
    int exit = 0;

    while (!exit) {
        int nr, ms;
        getDBSize(&nr, &ms);

        printf("Phonebook records %d of %d\n\n", nr, ms);

        printf("1. Enter new entry\n");
        printf("2. Display entry\n");
        printf("3. List phonebook\n");
        printf("4. Delete entry\n");
        printf("5. Save phonebook\n");
        printf("6. Load phonebook\n");
        printf("0. Quit\n");

        printf("\nEnter choice: ");
        int num;
        scanf("%d", &num);
        flushInput();

        switch (num) {
            case 1:
                newEntry();
                break;

            case 2:
                displayEntry();
                break;

            case 3:
                listEntries();
                break;

            case 4:
                deleteEntry();
                break;

            case 5:
                savePhonebook();
                break;

            case 6:
                loadPhonebook();
                break;

            case 0:
                exit = 1;
                break;

            default:
                printf("Illegal entry. You entered %d. PLease try again\n", num);
        }
    }
}

void loadPhonebook() {
    printf("\nLOAD\n");
    printf(  "====\n\n");

    char filename[128];
    printf("Enter phonebook filename: ");
    scanf("%s", filename);
    flushInput();

    printf("\n");
    int result = loadDB(filename);

    if (result == OK)
        printf("\n** Load OK! **\n\n");
    else
        printf("\n** Load FAILED! **\n\n");
}

void savePhonebook() {
    printf("\nSAVE\n");
    printf(  "====\n\n");

    char filename[128];
    printf("Enter phonebook filename: ");
    scanf("%s", filename);
    flushInput();

    printf("\n");
    int result = saveDB(filename);

    if (result == OK)
        printf("\n** Save OK! **\n\n");
    else
        printf("\n** Save FAILED! **\n\n");
}

void newEntry() {
    printf("\nNEW ENTRY\n");
    printf(  "=========\n\n");

    char name[NAME_LENGTH];
    char countryCode[C_LENGTH];
    char phoneNumber[NUM_LENGTH];

    printf("Enter name: ");

    readName(name, NAME_LENGTH);

    printf("\nEnter country code: ");
    scanf("%s", countryCode);
    flushInput();

    printf("\nEnter phone code: ");
    scanf("%s", phoneNumber);
    flushInput();

    int result;

    addPerson(name, countryCode, phoneNumber, &result);

    switch (result) {
        case OK:
            printf("\n** NEW ENTRY OK **!\n");
            break;

        case DUPLICATE:
            printf("\n** ERR: Person already exists. **!\n\n");
            break;

        case MAX_REACHED:
            printf("\n** ERR: Maximum records reached. **!\n\n");
            break;

        default:
            printf("\n** ERR: *SOMETHING* went wrong **!\n\n");
    }
}

void displayEntry() {
    printf("\nDISPLAY\n");
    printf(  "=======\n\n");

    char name[NAME_LENGTH];

    printf("Enter name to search: ");
    readName(name, NAME_LENGTH);
    TPhonebook *entry = findPerson(name);

    if (entry != NULL) {
        printf("\nName: %s\n", entry->name);
        printf("Phone: (%s)-(%s)\n\n", entry->countryCode, entry->phoneNumber);
    } else {
        printf("\n** Cannot find entry %s **\n\n", name);
    }
}

void listEntries() {
    listPhonebook();
}

void deleteEntry() {
    printf("\nDELETE\n");
    printf(  "======\n\n");

    char name[NAME_LENGTH];
    printf("Enter name: ");
    readName(name, NAME_LENGTH);

    int result = deletePerson(name);

    if (result == OK)
        printf("\n** Deletion OK! **\n\n");
    else
        printf("\n** Cannot find %s **\n\n", name);
}




