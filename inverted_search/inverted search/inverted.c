#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

int create_database(hash_t *arr, Flist *head)
{
    //printf("create database\n");
    Flist *temp = head;
    int index;
    char input_word[WORD_SIZE];
    while(temp != NULL)
    {
        // read word one by one from the file
        // open file in read mode
        FILE *fptr = fopen(temp -> filename, "r");
        //printf("file opened\n");
        if(fptr == NULL)
        {
            printf("Unable to open the file %s\n", temp -> filename);
        }
        while(fscanf(fptr, "%s", input_word) != EOF)
        {
            //printf("finding index\n");
            // find index for word
            if(islower(input_word[0]))
            {
                //printf("lower\n");
                index = input_word[0] - 97;
            }
            else if(isupper(input_word[0]))
            {
                //printf("upper\n");
                index = input_word[0] - 65;
            }
            else
            {
                index = 26;
            }
            // check index link is NULL or not
            if(arr[index].link == NULL)
            {
                //printf("creating node\n");
                // create new mainnode and subnode
                mnode *mnew = malloc(sizeof(mnode));
                if(mnew == NULL)
                {
                    return 0;
                }
                snode *snew = malloc(sizeof(snode));
                if(snew == NULL)
                {
                    return 0;
                }
                // update values
                mnew->filecount = 1;
                strcpy(mnew -> word, input_word);
                mnew -> sublink = snew;
                mnew -> mainlink = NULL;
                snew -> wordcount = 1;
                strcpy(snew -> filename, temp -> filename);
                snew -> slink = NULL;
                arr[index].link = mnew;
            }
            else{
                //printf("2 pointers\n");
                // take two pointers temp and prev
                mnode *mtemp = arr[index].link;
                mnode *mprev = NULL;
                while(mtemp != NULL)
                {
                    // check words are same or not
                    if(strcmp(mtemp -> word, input_word) == 0)
                    {
                        // if same check filename
                        // increment wordcount
                        snode *stemp = mtemp -> sublink; 
                        snode *sprev = NULL;
                        while(stemp != NULL)
                        {
                            if(strcmp(temp -> filename, stemp -> filename) == 0)
                            {
                                (stemp -> wordcount)++;
                                break;
                            }
                            else{
                                sprev = stemp;
                                stemp = stemp -> slink;
                            }
                        }    
                        // file name different
                        if(stemp == NULL)
                        {
                            //increment filecount
                            // create subnode also
                            snode *snew = malloc(sizeof(snode));
                            if(snew == NULL)
                            {
                                return 0;
                            }
                            snew -> wordcount = 1;
                            strcpy(snew -> filename, temp -> filename);
                            snew -> slink = NULL;
                            sprev -> slink = snew;
                            (mtemp -> filecount)++;
                        }
                        break;
                    }
                    else
                    {
                        mprev = mtemp;
                        mtemp = mtemp -> mainlink;
                    } 
                }
                // words are different
                if(mtemp == NULL)
                {
                    // create new mainnode and subnode
                    mnode *mnew = malloc(sizeof(mnode));
                    if(mnew == NULL)
                    {
                        return 0;
                    }
                    snode *snew = malloc(sizeof(snode));
                    if(snew == NULL)
                    {
                        return 0;
                    }
                    // update values
                    mnew -> filecount = 1;
                    strcpy(mnew -> word, input_word);
                    mnew -> sublink = snew;
                    mnew -> mainlink = NULL;
                    snew -> wordcount = 1;
                    strcpy(snew -> filename, temp -> filename);
                    snew -> slink = NULL;
                    
                    mprev -> mainlink = mnew;
                }   
            }
        }
        fclose(fptr);
        temp = temp -> link;
    }
    printf("\nDatabase created successfully\n");
    return SUCCESS;
}

int display_database(hash_t *arr)
{
    // display hash table
    for(int i = 0; i < SIZE; i++)
    {
        if(arr[i].link != NULL)
        {
            mnode *mtemp = arr[i].link;
            while(mtemp != NULL)
            {
                // printindex, filecount, word
                printf("[%d]\t[%s]\t%d\t", i, mtemp -> word, mtemp -> filecount);
                snode *stemp = mtemp -> sublink;
                while(stemp != NULL)
                {
                    // print filename and wordcount
                    printf("%s\t%d\t", stemp -> filename, stemp -> wordcount);
                    stemp = stemp -> slink;
                }
                printf("\n");
                mtemp = mtemp -> mainlink;
            }
        }
    }
}

int search_database(hash_t *arr)
{
    /* 
        read word from user
        if word is present in database
        if yes => print details
        if no => print error message
    */
    char search_word[WORD_SIZE];
    int found = 0;
    printf("Enter the word to search : ");
    scanf(" %s", search_word);
    for(int i = 0; i < SIZE; i++)
    {
        if(arr[i].link != NULL)
        {
            mnode *mtemp = arr[i].link;
            while(mtemp != NULL)
            {
                if(strcmp(search_word, mtemp -> word) == 0)
                {
                    found = 1;
                    // printindex, filecount, word
                    //printf("[%d]\t%s\t%d\t", i, mtemp -> word, mtemp -> filecount);
                    printf("Word Found: '%s'\n", mtemp -> word);
                    printf("Appears in %d file(s):\n", mtemp -> filecount);
                    snode *stemp = mtemp -> sublink;
                    while(stemp != NULL)
                    {
                        // print filename and wordcount
                        //printf("%s\t%d\t", stemp -> filename, stemp -> wordcount);
                        printf("%s %d time(s)\n", stemp -> filename, stemp -> wordcount);
                        stemp = stemp -> slink;
                    }
                    printf("\n");
                    return SUCCESS;
                }
                mtemp = mtemp -> mainlink;
            }
        }
    }
    if(found == 0)
    {
        printf("\nWord not found in database\n");
        return FAILURE;
    }
}

int update_database(hash_t *arr, Flist **head)
{
    /*
        read file name from user
        check extention .txt
        check the file is present or not
        check the file is empty or not
        check the dulipcate or not
        add in flist at last position
        call create_database(arr, lastnode_address)
     */
    char filename[50];

    printf("Enter file name to update: ");
    scanf("%s", filename);

    // check extension
    char *ext = strrchr(filename, '.');
    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("Error: Not a .txt file\n");
        return FAILURE;
    }

    // check file existence
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Error: File not found\n");
        return FAILURE;
    }

    // check empty file
    fseek(fptr, 0, SEEK_END);
    if (ftell(fptr) == 0)
    {
        printf("Error: File is empty\n");
        fclose(fptr);
        return FAILURE;
    }
    rewind(fptr);
    fclose(fptr);

    // check duplicate in Flist
    Flist *temp = *head;
    while (temp != NULL)
    {
        if (strcmp(temp -> filename, filename) == 0)
        {
            printf("Error: %s file already exists in database\n", temp -> filename);
            return FAILURE;
        }
        temp = temp -> link;
    }

    // insert into Flist
    if (insert_last(head, filename) == FAILURE)
    {
        printf("Error: Unable to insert %s\n", temp -> filename);
        return FAILURE;
    }

    // get last node (new file only)
    Flist *new_node = *head;
    while (new_node -> link != NULL)
    {
        new_node = new_node -> link;
    }

    // create database only for this file
    int res = create_database(arr, new_node);
    if(res == SUCCESS)
    {
        printf("\nFile '%s' added successfully. Updating Database..... \n", filename);
        printf("\nFile update with %s is successful..! \n", filename);
    }
    else
    {
        printf("Error : Failed to update '%s' in Database", new_node -> filename);
    }
    return SUCCESS;
}

int save_database(hash_t *arr)
{
    // content in the terminal are stored in resultfile.txt file
    // read file name from the user
    // check file extention .txt
    // open the file in write mode
    char filename[50];
    printf("Enter output file name: ");
    scanf("%s", filename);
    // .txt validation
    char *ext = strrchr(filename, '.');
    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("Error: file must have .txt extension\n");
        return FAILURE;
    }

    FILE *fptr = fopen(filename, "w");
    if (!fptr)
    {
        printf("Unable to open file\n");
        return FAILURE;
    }
    for(int i = 0; i < SIZE; i++)
    {
        if(arr[i].link != NULL)
        {
            mnode *mtemp = arr[i].link;
            while(mtemp != NULL)
            {
                // printindex, filecount, word
                //fprintf(fptr, "%d\t%s\t%d", i, mtemp -> word, mtemp -> filecount);
                fprintf(fptr, "Word: %s File(s): %d\n", mtemp -> word, mtemp -> filecount);
                snode *stemp = mtemp -> sublink;
                while(stemp != NULL)
                {
                    // print filename and wordcount
                    //fprintf(fptr, "%s\t%d", stemp -> filename, stemp -> wordcount);
                    fprintf(fptr, "%s %d time(s)\n", stemp -> filename, stemp -> wordcount);
                    stemp = stemp -> slink;
                }
                fprintf(fptr, "\n");
                mtemp = mtemp -> mainlink;
            }
        }
    }
    fclose(fptr);
    printf("\nDatabase successfully saved to %s\n", filename);

    return SUCCESS;
}

int insert_last(Flist **head, char *filename)
{
    Flist *new = malloc(sizeof(Flist));
    if (!new)
        return FAILURE;

    strcpy(new -> filename, filename);
    new->link = NULL;

    if (*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    Flist *temp = *head;
    while (temp -> link != NULL)
    {
        temp = temp -> link;
    }

    temp -> link = new;
    return SUCCESS;
}