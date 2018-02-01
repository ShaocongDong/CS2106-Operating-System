//  Created by DongShaocong on 1/2/18.
#ifndef PHONEBOOK
#define PHONEBOOK

#define NAME_LENGTH           64
#define C_LENGTH              4
#define NUM_LENGTH            10


typedef struct
{
    unsigned index;
    int deleted;
    char name[NAME_LENGTH];
    char countryCode[C_LENGTH];
    char phoneNumber[NUM_LENGTH];
} TPhonebook;

enum
{
    OK=0,
    MAX_REACHED=1,
    DUPLICATE=2,
    CANNOT_FIND=3,
    SAVE_FAIL=4,
    LOAD_FAIL=5
};

void initPhonebook(int maxRecords);
void freePhonebook(void);
void addPerson(char *name, char *countryCode, char *phoneNumber, int *result);
TPhonebook *findPerson(char *name);
void listPhonebook(void);
int deletePerson(char *name);
int saveDB(char *filename);
int loadDB(char *filename);
void resizeDB(int numNewRecords);
void getDBSize(int *nr, int *ms);

#endif
