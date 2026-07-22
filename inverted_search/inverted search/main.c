/*

    Name : Anjana V
    Description : Using inverted search method, hash table, single linked list, files created , dispalyed, updated and save database for a directory
    Sample Input : 
                    ./a.out f1.txt f2.txt empty.txt f2.txt
    Sample Output :
                    Error: empty.txt is empty
                    Error: f2.txt is duplicate file
                    f1.txt added successfully

                    f2.txt added successfully

                    Inputs are validated and files added in database successfully



                    1. Create Database
                    2. Display Database
                    3. Search Database
                    4. Update Database
                    5. Save Database
                    6. Exit
                    Choose option(1-6) :
                    1

                    Create Database is selected

                    Database created successfully

*/

#include<stdio.h>
#include "main.h"

int main(int argc, char *argv[])
{
    Flist *head = NULL;
    int option;
    static int flag = 0;

    hash_t arr[SIZE];
    for(int i = 0; i < SIZE; i++)
    {
        arr[i].index = i;
        arr[i].link = NULL;
    }
    if(read_and_validation(argc, argv, &head) == SUCCESS)
    {
        printf("Inputs are validated and files added in database successfully\n\n");
        //print_list(head);

        while(1)
        {
            // display menu
            printf("\n\n1. Create Database\n2. Display Database\n3. Search Database\n4. Update Database\n5. Save Database\n6. Exit\n");
            // read option from the user
            printf("Choose option(1 - 6) :\n");
            scanf("%d", &option);
            switch(option)
            {
                case 1:
                    // only once create database should be performed
                    if(flag == 0)
                    {
                        printf("\nCreate Database is selected\n");
                        create_database(arr, head);
                        flag = 1;
                    }
                    else
                    {
                        printf("Create Database already done\n");
                    }
                    break;
                case 2:
                    printf("\nDisplay Database selected\n");
                    display_database(arr);
                    break;
                case 3:
                    printf("\nSearch Database selected\n");
                    search_database(arr);
                    break;
                case 4:
                    printf("\nUpdate Database selected\n");
                    update_database(arr, &head);
                    break;
                case 5:
                    printf("\nSearch Database selected\n");
                    save_database(arr);
                    break;
                case 6:
                    printf("Exiting...\n");
                    return 0;
                default:
                    printf("Error : choose from option");
                    break;
            }
        }
    }
}