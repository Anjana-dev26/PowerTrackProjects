#include<stdio.h>
#include<stdlib.h>
#include "apc.h"

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	// stores quotient, number of times subtraction happens
	int count = 0;
	int flag = 1;
	// repeat subtraction until list1 < list2
	while(flag)
	{
		// compare list1 and list2
		int ret = list_compare(*head1, *head2);
		// list1 >= list2
		if(ret == 1)
		{
			// subtract list2 from list1
			subtraction(head1, tail1, head2, tail2, headR, tailR);
			count++;
			/* update the list1 with listR */
			// deleting list1
			delete_list(head1, tail1);
			/* updating list1 with listR */
			*head1 = *headR;
			*tail1 = *tailR;
			/* reseting the listR head and tail */
			*headR = NULL;
			*tailR = NULL;
			// remove leading zeros 
			remove_leading_zeros(head1, tail1);
		}
		else
		{
			// stop when list1 < list2
			flag = 0;
		}
	}
	// final quotient stored as a single node
	insert_first(headR, tailR, count);
	return SUCCESS;
}
