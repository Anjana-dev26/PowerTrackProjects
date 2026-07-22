/*
    Name : Anjana V
    Description : Creating a address book with contact functionalities like create, delete, edit, save, search using structures, functions, file i/o, string, loops, operators
*/

#include<stdio.h>
#include "contact.h"
#include "populate.h"
#include "file.h"


int main()
{
    int choice;
    // varaible declaration of structre adress book
    AddressBook var;
    // function call to load address book with contact info from contact.txt
    initialise(&var);

    // loop to run address book menu until exited
    do
    {
        printf("\nAddressBook Menu\n\n");
        printf("1.Create contact\n");
        printf("2.Edit contact\n");
        printf("3.Search contact\n");
        printf("4.Delete contact\n");
        printf("5.List all contact\n");
        printf("6.Save contact\n");
        printf("7.Exit\n");
        printf("\nEnter your choice :\n");
        scanf("%d", &choice);

        // choosing address menu
        switch(choice)
        {
            case 1:
                // creating new contact
                contactCreate(&var);
                break;
            case 2:
                // editing existing contact
                contactEdit(&var);
                break;
            case 3:
                // search from existing contct
                contactSearch(&var);
                break;
            case 4:
                // delete particular contact
                contactDelete(&var);
                break;
            case 5:
                // list of all the contacts in addressbook 
                contactLists(&var);
                break;
            case 6:
                // saving the updation in the address book to contact.txt
                saveContactsToFile(&var);
                break;
            case 7:
                // exiting from adress menu
                exits(&var);
                break;
            default:
                printf("Invalid Input, enter value from 1 - 7\n");
                break;
        }
    }while(choice != 7);
    return 0;
}
