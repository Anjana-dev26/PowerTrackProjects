#include<stdio.h>
#include<string.h>
#include "contact.h"
#include "populate.h"
#include "file.h"
#include <ctype.h>


// creating new contact
void contactCreate(AddressBook *addressBook)
{
    //read name and store in address book
    char phone[20], email[60];
    printf("Enter name\n");
    scanf(" %[^\n]", addressBook->contacts[addressBook->contactcount].name);

    while(1)
    {
        // read mobile number
        printf("Enter phone\n");
        scanf(" %[^\n]", phone);
        // Validate mobile number
        if(ValidPhone(phone, addressBook))
        {
            //copy mobile no to contacts
            strcpy(addressBook->contacts[addressBook->contactcount].phone, phone);
            break;
        }
        else
        {
            printf("Invalid phone number! Try again.\n");
        }
    }

    while (1) 
    {
        // reading and adding email
        printf("Enter Email (must be in format abc@gmail.com):\n");
        scanf(" %[^\n]", email);
        // validate email
        if (ValidEmail(email, addressBook)) {
            // coping email to contact
            strcpy(addressBook->contacts[addressBook->contactcount].email, email);
            break;
        }
       	else 
	    {
            printf("Invalid email! Try again.\n");
        }
    }
    addressBook->contactcount++;
}
void contactEdit(AddressBook *addressBook)
{
    // read a name to search
    char nametobeedited[30];
    printf("Enter name :\n");
    scanf(" %[^\n]", nametobeedited);
    int edit = 0;
    int count = 0;
    // search logic for contact and printing the found result
    for(int i = 0; i < addressBook->contactcount; i++)
    {
        // comparing the contact name with search name
        if(strcmp(addressBook->contacts[i].name, nametobeedited) == 0)
        {
            printf("\nContact found\n");
            printf("\n%s\t",addressBook->contacts[i].name);
            printf("%s\t",addressBook->contacts[i].phone);
            printf("%s\n",addressBook->contacts[i].email);
            edit = 1;
            count = i;
            break;
        }
    }
    if(edit == 0)
    {
        printf("\nContact not found\n");
    }
    // if contact found enter what you want to edit 1. name 2. phone 3. email
    if(edit == 1)
    {
        // read choice
        int choiceEdit;
        do
        {
            printf("1. Name\n");
            printf("2. Mobile no\n");
            printf("3. Email\n");
            printf("4. Exit\n");
            printf("Enter choice to edit (1/2/3/4) : \n");
            // read new name/phone/email
            scanf("%d", &choiceEdit);
            //modify the existing one using strcpy()
            char newValue[60];
            switch(choiceEdit)
            {
                // name edit
                case 1:
                    //read new name form user to edit
                    printf("New name : \n");
                    scanf(" %[^\n]", newValue);
                    // updating new name
                    strcpy(addressBook->contacts[count].name, newValue);
                    printf("New name : %s\n", addressBook->contacts[count].name);
                    break;
                // mobile no edit
                case 2:
                    printf("New mobile no : \n");
                    while(1)
                    {
                        // reading new mobile no
                        scanf(" %[^\n]", newValue);
                        // validation
                        if(ValidPhone(newValue, addressBook))
                        {
                            // updating new mobile nummber
                            strcpy(addressBook->contacts[count].phone, newValue);
                            printf("New mobile no : %s\n", addressBook->contacts[count].phone);
                            break;
                        }
                        else
                        {
                            printf("Invalid phone number! Try again.\n");
                        }
                    }
                    break;
                // email edit
                case 3:
                    printf("New email : \n");
                    while (1) 
                    {
                        // reading new email id
                        printf("Enter Email (must be in format abc@gmail.com):\n");
                        scanf(" %[^\n]", newValue);
                        // validation
                        if (ValidEmail(newValue, addressBook)) {
                            // updating email id
                            strcpy(addressBook->contacts[count].email, newValue);
                            printf("New email : %s\n", addressBook->contacts[count].email);
                            break;
                        }
                        else 
                        {
                            printf("Invalid email! Try again.\n");
                        }
                    }
                    break;
                default:
                    printf("Invalid Input. Enter choice 1/2/3/4\n");
                    break;
            }
        }while(choiceEdit != 4);
    }
}
void contactSearch(AddressBook *addressBook)
{
    /* Define the logic for search */
   //read name from user to search in addressbook
    char nametobefound[30];
    // read name to be found
    printf("\nEnter name to be found\n");
    scanf(" %[^\n]", nametobefound);
    int found = 0;
    // search logic for the contact
    for(int i = 0; i < addressBook->contactcount; i++)
    {
        // comparing the contact name with search name
        if(strcmp(addressBook->contacts[i].name, nametobefound) == 0)
        {
            // contact info is printed for the searched and found contact
            printf("Contact found\n");
            printf("\n%s\t",addressBook->contacts[i].name);
            printf("%s\t",addressBook->contacts[i].phone);
            printf("%s\n",addressBook->contacts[i].email);
            found = 1;
            break;
        }
    }
    if(found == 0)
    {
        printf("Contact not found\n");
    }
}
void contactDelete(AddressBook *addressBook)
{
    // read the delete name
    char deleteName[30];
    // search if the contact found
    printf("Enter name to be deleted :\n");
    scanf(" %[^\n]", deleteName);

    int deletes = 0;
    int count = 0;

    // search logic w.r.t name
    for(int i = 0; i < addressBook->contactcount; i++)
    {
        if(strcmp(addressBook->contacts[i].name, deleteName) == 0)
        {
            printf("\nContact found\n");
            printf("\n%s\t",addressBook->contacts[i].name);
            printf("%s\t",addressBook->contacts[i].phone);
            printf("%s\n",addressBook->contacts[i].email);
            deletes = 1;
            count = i;
            break;
        }
    }

    if(deletes == 0)
    {
        printf("\nContact not found\n");
    }

    // if found, then index position need i th to be replaced with (i+1)th
    if(deletes == 1)
    {
        int confirm;

        printf("Press 1 and enter to confirm deletion\n");
        scanf("%d", &confirm);
        
        // deleting contact
        if(confirm == 1)
        {
            // shifting the position till end
             for (int j = count; j < addressBook->contactcount - 1; j++) 
	        {
                addressBook->contacts[j] = addressBook->contacts[j + 1];
            }
            // decrement count to reset the array size
            addressBook->contactcount--;
            printf("Contact deleted successfully.\n");
        }
    }
}

// getting all the contacts from addressbook
void contactLists(AddressBook *addressBook)
{
    printf("Name \t Mobile no \t Mail id\n");
    // loop to gets all the contacts{name,mobile,email}
    for(int i = 0; i < addressBook->contactcount; i++)
    {
        // printing contact info
        printf("%s\t",addressBook->contacts[i].name);
        printf("%s\t",addressBook->contacts[i].phone);
        printf("%s\n",addressBook->contacts[i].email);
    }
}

// exicts the address menu
void exits(AddressBook *addressBook)
{

}


void initialise(AddressBook *addressBook)
{
    //initialise address book with some dummy contact
    //populateAddressBook(addressBook);
    // to copy dummy contact to adress book

    // copy contact from file to addressbook
    loadContactsFromFile(addressBook);
}

// check the phone no is of 10 digits
int ValidPhone(char phone[], AddressBook *addressBook) 
{
    // checking length and number type is digit
    if (strlen(phone) != 10)
	    return 0;
    for (int i = 0; i < 10; i++) 
    {
        if (!isdigit(phone[i])) 
	        return 0;
    }

    // checking uniquness of mobile number
    for(int i = 0; i < addressBook->contactcount; i++)
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            printf("Contact alread exsist \n");
            return 0;
        }
    }

    return 1;
}

// Validate email ends with @gmail.com
int ValidEmail(char email[], AddressBook *addressBook) 
{
    int len = strlen(email);
    // checking substrting @gmail.com and also set min length as 12
    if (strstr(email, "@gmail.com") != NULL && (len > 11)) {
        // checking uniquness of email id
        for(int i = 0; i < addressBook->contactcount; i++)
        {
            if(strcmp(addressBook->contacts[i].email, email) == 0)
            {
                printf("Contact alread exsist \n");
                return 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}
