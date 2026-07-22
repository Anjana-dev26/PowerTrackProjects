#include<stdio.h>
#include "apc.h"

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR,  Dlist **tailR)
{
	int carry = 0;
	/* starting from LSD */
	Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
	// Traverse until both lists are empty
	while(temp1 || temp2)
	{
		if(temp1 != NULL && temp2 != NULL)
		{
			// Add digits from both lists and carry
			int data = temp1->data + temp2->data + carry;
			// If sum exceeds 9, compute carry and store remainder
			if(data > 9)
			{
				insert_first(headR, tailR, (data % 10));
				carry = data / 10;
			}
			else
			{
				insert_first(headR, tailR, data);
				carry = 0 ;
			}
			// Move to previous digits
			temp1 = temp1->prev;
			temp2 = temp2->prev;
		}
		else if(temp1 != NULL && temp2 == NULL)
		{
			/* Only list1 has digits left */
			int data = temp1->data + carry;
			if(data > 9)
			{
				insert_first(headR, tailR, (data % 10));
				carry = data / 10;
			}
			else
			{
				insert_first(headR, tailR, data);
				carry = 0;
			}
			temp1 = temp1->prev;
		}
		else if(temp1 == NULL && temp2 != NULL)
		{
			/* Only list2 has digits left */
			int data = temp2->data + carry;
			if(data > 9)
			{
				insert_first(headR, tailR, (data % 10));
				carry = data / 10;
			}
			else
			{
				insert_first(headR, tailR, data);
				carry = 0;
			}
			temp2 = temp2->prev;
		}
	}
	// If carry remains after final addition, insert it at the front
	if (carry)
	{
    	insert_first(headR, tailR, carry);
	}
	
	return SUCCESS;
}
