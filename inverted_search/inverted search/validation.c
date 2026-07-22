#include <stdio.h>
#include <string.h>
#include "main.h"

/*
    CLA are passed or not
    if not passed print error message
    check file one by one if file extention is .txt
        if not => print error message for that file and continue
        if yes => check file is present or not
            if not => print error message for that file and continue
            if yes => if the file is empty or not
                if not => print error message and continue
    store valide files in linked list
    check for duplicate files
*/

int read_and_validation(int argc, char *argv[], Flist **head)
{
    // check CLA
    if(argc < 2)
    {
        printf("Error: No files passed through command line\n");
        printf("Example format to pass : ./a.out f1.txt f2.txt f3.txt\n");
        return FAILURE;
    }

    // process each file
    for(int i = 1; i < argc; i++)
    {
        char *filename = argv[i];

        // check .txt extension
        char *ext = strrchr(filename, '.');
        if(ext == NULL || strcmp(ext, ".txt") != 0)
        {
            printf("Error: %s is not a .txt file\n", filename);
            continue;
        }

        // check file existence
        FILE *fptr = fopen(filename, "r");
        if(fptr == NULL)
        {
            printf("Error: %s file not found\n", filename);
            continue;
        }

        // check empty file
        fseek(fptr, 0, SEEK_END);
        if(ftell(fptr) == 0)
        {
            printf("Error: %s is empty\n", filename);
            fclose(fptr);
            continue;
        }
        rewind(fptr);
        fclose(fptr);

        // check duplicate file
        Flist *temp = *head;
        int duplicate = 0;

        while(temp != NULL)
        {
            if(strcmp(temp -> filename, filename) == 0)
            {
                printf("Error: %s is duplicate file\n", filename);
                duplicate = 1;
                break;
            }
            temp = temp -> link;
        }

        if(duplicate)
            continue;

        // insert into linked list
        if(insert_last(head, filename) == FAILURE)
        {
            printf("Error: Unable to insert %s into list\n", filename);
            return FAILURE;
        }
    }

    // list empty or not
    if(*head == NULL)
    {
        printf("Error: No valid files found\n");
        return FAILURE;
    }

    Flist *ptemp = *head;
    while(ptemp != NULL)
    {
        printf("%s added successfully\n\n", ptemp -> filename);
        ptemp = ptemp -> link;
    }

    return SUCCESS;
}