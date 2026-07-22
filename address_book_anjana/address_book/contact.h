#ifndef CONTACT_H
#define CONTACT_H

// defining maximum contact capacity
#define MAX_CONTACTS 100

// declaration of datatype structre Contact
typedef struct
{
    char name[30];
    char phone[20];
    char email[60];
} Contact;

// declaration of datatype of nested structre AddressBook
typedef struct
{
    Contact contacts[MAX_CONTACTS];
    // variable to keep track of number of contacts present in address book
    int contactcount;
} AddressBook;

// declarations of address menu functions
void contactCreate(AddressBook *addressBook);
void contactDelete(AddressBook *addressBook);
void contactCreate(AddressBook *addressBook);
void contactEdit(AddressBook *addressBook);
void contactSearch(AddressBook *addressBook);
void contactDelete(AddressBook *addressBook);
void contactLists(AddressBook *addressBook);
void exits(AddressBook *addressBook);
void initialise(AddressBook *addressBook);
int  ValidPhone(char phone[], AddressBook *addressBook);
int ValidEmail(char email[], AddressBook *addressBook);

#endif
