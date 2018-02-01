#ifndef PHONEBOOK
#define PHONEBOOK

#define NAME_LENGTH   		64
#define C_LENGTH			4
#define NUM_LENGTH 			10


typedef struct
{
	unsigned index;
	int deleted;
	char name[NAME_LENGTH];
	char countryCode[C_LENGTH];
	char phoneNumber[NUM_LENGTH];
} TPhonebook;
