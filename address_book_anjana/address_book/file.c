#include<stdio.h>
#include<string.h>
#include "file.h"


// loading of contacts from contact.txt to the address book
void loadContactsFromFile(AddressBook *addressBook) 
{
    // opening of file in read mode and assigning file pointer
    FILE *fp = fopen("contact.txt", "r");
    // checking file present or not
    if (!fp) {
        printf("No existing contacts found. Starting fresh.\n");
        return;
    }

    int count;
    // reading the contact count from contact.txt
    fscanf(fp, "%d\n", &count);
    addressBook->contactcount = count;
    // reading the contacts from contact.txt
    for(int i = 0; i < count; i++)
    {
        fscanf(fp, "%30[^,],%20[^,],%60[^\n]\n",
                      addressBook->contacts[i].name,
                      addressBook->contacts[i].phone,
                      addressBook->contacts[i].email);
    }
    // cloasing file
    fclose(fp);
    //printf("Contacts loaded successfully from contact.txt\n");    
}

// Updating or adding contact data to the contact.txt file
void saveContactsToFile(AddressBook *addressBook)
{
    // contact need to be written to the file
    // open a file (write mode)
    FILE *fp = fopen("contact.txt", "w");
    // checking file present or not
    if (!fp) {
        perror("Error opening contact.txt for writing");
        return;
    }

    // Coping datas to the contact.txt file in the order {count, datas......}
    fprintf(fp, "%d\n", addressBook->contactcount); 
    for (int i = 0; i < addressBook->contactcount; i++)
    {
        fprintf(fp, "%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    
    // fclose
    fclose(fp);
    printf("Contacts saved successfully to contact.txt\n");
}