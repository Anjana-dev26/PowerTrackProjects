#include<stdio.h>
#include "apc.h"

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	int borrow = 0, diff, flag = 0;
	// start from least significant digits (tail)
    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
	int var2;
	// loop until all digits of first number are processed
	while(temp1)
	{
		// if second list still has digits
		if (temp2 != NULL)
		{
			// avoiding segmantation fault
			var2 = temp2->data;
		}
		else
		{
			// if second list is shorter, treat missing digits as 0
			var2 = 0;
		}
		// check if borrow is needed
		if((temp1->data - borrow) < var2 )
		{
			// take borrow from next higher digit
			diff = temp1->data + 10;
			// store result digit at front
			insert_first(headR, tailR, (diff - var2 - borrow));
			// set borrow for next iteration
			borrow = 1;
		}
		else
		{
			// normal subtraction without borrow
			diff = temp1->data - var2 - borrow;
			// store result digit
			insert_first(headR, tailR, diff);
			// reset borrow
			borrow = 0;
		}
		// move to next digit (right -> left)
		temp1 = temp1->prev;
		 // MOVE ONLY IF NOT NULL
        if (temp2 != NULL)
        {
            temp2 = temp2->prev;
        }
	}
	return SUCCESS;
}
