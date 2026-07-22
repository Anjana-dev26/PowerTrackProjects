#include<stdio.h>
#include<stdlib.h>
#include "apc.h"

/* Insert node at the beginning of the doubly linked list*/
int insert_first(Dlist **head, Dlist **tail, int data)
{
    // allocate memory for new node
    Dlist *new_node = (Dlist *)malloc(sizeof(Dlist));
    if (!new_node)
        // memory allocation failed
        return FAILURE;
    // assign data
    new_node->data = data;
    // first node has no previous
    new_node->prev = NULL;
    // link new node to current head
    new_node->next = *head;
    // if list is not empty, update previous pointer of old head
    if (*head)
        (*head)->prev = new_node;
    else
        *tail = new_node; // empty list: tail also points here
    // update head pointer
    *head = new_node;
    return SUCCESS;
}

// Insert node at the end of the doubly linked list
int insert_last(Dlist **head, Dlist **tail, int data)
{
    // allocate memory for new node
    Dlist *new_node = (Dlist *)malloc(sizeof(Dlist));
    if (!new_node)
        return FAILURE;
    // assign data
    new_node->data = data;
    // last node has no next
    new_node->next = NULL;
    // link with current tail
    new_node->prev = *tail;
    // if list is not empty, update next pointer of old tail
    if (*tail)
        (*tail)->next = new_node;
    else
        *head = new_node; // empty list: head also points here
    // update tail pointer
    *tail = new_node;
    return SUCCESS;
}

// Print the list data in order
void print_list(Dlist *head)
{
    // if list is empty, print 0
    if (!head)
    {
        printf("0");
        return;
    }
    // traverse from head to tail
    Dlist *temp = head;
    while (temp)
    {
        printf("%d", temp->data); // print each digit
        temp = temp->next;
    }
}

// Free the entire list
void delete_list(Dlist **head, Dlist **tail)
{
    Dlist *temp;
    // traverse and free each node
    while (*head)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    // reset tail after deletion
    *tail = NULL;
}

// Convert argv[1] and argv[3] (operands) to doubly linked lists
void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[])
{
    // argv[1] is first number string
    char  *str1 = argv[1];
    int i = 0;
    int data;
    // convert each character to digit and insert at end
    while(str1[i] != '\0')
    {
        // convert char to int
        data = str1[i] - '0';
        insert_last(head1, tail1, data);
        i++;
    }
    // argv[3] is second number string
    char  *str2 = argv[3];
    int j = 0;
    while(str2[j] != '\0')
    {
        data = str2[j] - '0';
        insert_last(head2, tail2, data);
        j++;
    }
    // remove unnecessary leading zeros from both numbers
    remove_leading_zeros(head1, tail1);
    remove_leading_zeros(head2, tail2);
}

/* 
    Compare two lists
    return 1 -> list1 > list2
    return 0 -> list1 < list2
    return 1 -> equal
 */
int list_compare(Dlist *h1, Dlist *h2)
{
    int list1_count = 0, list2_count = 0;
    Dlist *temp1 = h1, *temp2 = h2;
    /* count digits in list1 */
    while (temp1)
    {
        list1_count++;
        temp1 = temp1->next;
    }
    /* count digits in list2 */
    while (temp2)
    {
        list2_count++;
        temp2 = temp2->next;
    }
    // compare based on number of digits
    if(list1_count > list2_count)
    {
        return 1;
    }
    else if(list1_count < list2_count)
    {
        return 0;
    }
    else if(list1_count == list2_count)
    {
        /* if lengths are equal, compare digit by digit */
        temp1 = h1;
        temp2 = h2;
        while(temp1 != NULL)
        {
            if(temp1->data > temp2->data)
            {
                return 1;
            }
            else if(temp1->data < temp2->data)
            {
                return 0;
            }
            else if(temp1->data == temp2->data)
            {
                // when data are equal, move to next digits
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
        }
    }
    // if all digits are equal
    return 2;
}

/* removing extra zeroes */
void remove_leading_zeros(Dlist **head, Dlist **tail)
{
    /* Remove leading zeros from a number list
        Example: 0->8->0 becomes 8->0 */
    while (*head && (*head)->data == 0 && (*head)->next != NULL)
    {
        Dlist *temp = *head;
        // move head forward
        *head = (*head)->next;
        // update prev pointer
        (*head)->prev = NULL;
        free(temp);
    }
}