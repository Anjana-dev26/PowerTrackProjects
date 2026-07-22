#include<stdio.h>
#include "apc.h"

int multiplication(Dlist **head1, Dlist **tail1,Dlist **head2, Dlist **tail2,Dlist **headR, Dlist **tailR)
{
    // keeps track of position (used for shifting zeros)
    int count = 0;
    // creating tempory variable
    Dlist *temp2 = *tail2;
    Dlist *headR1 = NULL, *tailR1 = NULL; // accumulated result
    // loop through each digit of second number (like manual multiplication)
    while (temp2)
    {
        // start from last digit of first number
        Dlist *temp1 = *tail1;
        int carry = 0;
        // headR2/tailR2 → stores partial result of one digit multiplication
        Dlist *headR2 = NULL, *tailR2 = NULL; // current partial result
        // multiply one digit of list2 with full list1
        while (temp1)
        {
            // multiply digits + previous carry
            int prod = temp1->data * temp2->data + carry;
            // store last digit of result
            insert_first(&headR2, &tailR2, prod % 10);
            // update carry
            carry = prod / 10;
            // move to next digit (right -> left)
            temp1 = temp1->prev;
        }
        // insert remaining carry
        if (carry)
        {
            insert_first(&headR2, &tailR2, carry);
        }
        // shifting (adding zeros at end)
        for (int i = 0; i < count; i++)
        {
            insert_last(&headR2, &tailR2, 0);
        }
        // first partial result -> directly assign to final result
        if (headR1 == NULL)
        {
            headR1 = headR2;
            tailR1 = tailR2;
        }
        else
        {
            // add current partial result with accumulated result
            addition(&headR1, &tailR1, &headR2, &tailR2, headR, tailR);
            // free old lists
            //delete_list(&headR1, &tailR1);
            //delete_list(&headR2, &tailR2);
            // Delete the partial result list
            /*headR1 = NULL;
            tailR1 = NULL;*/
            // update result
            headR1 = *headR;
            tailR1 = *tailR;
			// reset headR for next iteration
            *headR = NULL;
            *tailR = NULL;
            // Delete the partial result list
            headR2 = NULL;
            tailR2 = NULL;
        }
        // move to next digit of second number
        temp2 = temp2->prev;
        count++;
    }
    // assign final result to output pointers   
    *headR = headR1;
    *tailR = tailR1;

    return SUCCESS;
}