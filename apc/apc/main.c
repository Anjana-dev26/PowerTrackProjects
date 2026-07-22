/*

	Name : Anjana V
	Descrition : Program for Arbitary precesion calculator(APC) using double linked list, functions, pointers, loop, operators etc. 
				 APC is used to get precesion result other than the large exponential results in normal calculator.
	Sample Input 1 : ./a.out 123 + 456
	Sample Output 1 :
						list1 : 123
						Operator : +
						list2 : 456
						Result : 579
	Sample Input 2 : ./a.out 456 - 123
	Sample Output 2 :
						list1 : 456
						Operator : -
						list2 : 123
						Result : 333
	Sample Input 3 : ./a.out 12 "*" 3
	Sample Output 3 :
						list1 : 12
						Operator : *
						list2 : 3
						Result : 36
	Sample Input 4 : ./a.out 10 / 2
	Sample Output 4 :			
						list1 : 10
						Operator : /
						list2 : 2
						Result : 5			

*/
#include<stdio.h>
#include "apc.h"

int main(int argc, char *argv[])
{
	/* Declare the pointers */
	Dlist *head1=NULL, *tail1=NULL;
    Dlist *head2=NULL, *tail2=NULL; 
    Dlist *headR=NULL,*tailR=NULL;
	/* checking number of CLA */
	if (argc != 4)
	{
		printf("Invalid number of arguments\n");
		printf("Format : ./a.out <num1> <operator> <num2>\n         Use ' ./a.out <num1> x <num2> ' for multiplication\n");
		return FAILURE;
	}
	// opertion to be performed
	char operator=argv[2][0];

	/* validate numbers */
   // Check argv[1]
	for (int i = 0; argv[1][i]; i++) {
		if (argv[1][i] < '0' || argv[1][i] > '9') 
		{
			printf("Invalid input. Enter integer\n");
			return FAILURE;
		}
	}
	// Check argv[3]
	for (int i = 0; argv[3][i]; i++) {
		if (argv[3][i] < '0' || argv[3][i] > '9') {
			printf("Invalid input. Enter integer\n");
			return FAILURE;
		}
	}

	digit_to_list(&head1,&tail1,&head2,&tail2,argv);

	printf("list1 : ");
	print_list(head1);
	printf("\n");
	printf("Operator : %c\n", operator);
	printf("list2 : ");
	print_list(head2);
	printf("\n");
	// selected the operation is performed
	switch (operator)
	{
		case '+':
			/* call the function to perform the addition operation */
			addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);
			break;
		case '-':	
			/* call the function to perform the subtraction operation */
			// check list1 > list 2 or not
			int res = list_compare(head1, head2);
			int flag = 0;
			// list1 > list2
			if(res == 1)
			{
				subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			}
			// list2 > list1
			else if(res == 0)
			{
				//update the list1 = list2
				//update the list2 = list1
				subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
				// set the flag to indicate negative to result
				flag = 1;
			}
			if(flag == 1)
			{
				// removing leading zeroes after sub to avoid 0 * (-)
				remove_leading_zeros(&headR, &tailR);
				// updating the ist digit with it's -ve
				headR->data = headR->data * -1;
			}
			break;
		case 'x':	
			/* call the function to perform the multiplication operation */
			multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		case '/':
			/* call the function to perform the division operation */
			/* when the divided by 0 */
			if (!head2 || (head2->data == 0 && head2->next == NULL))
			{
				printf("Error: Division by zero\n");
				return FAILURE;
			}
			/* when divide by non negetive integer*/
			else
			{
				division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			}
			break;
		default:
			/*prompt for invalid input */
			printf("Invalid Input:-( Try again...\n");
			printf("Input format: ./a.out <num1> <operator> <num2>\n              Use ' ./a.out <num1> x <num2> ' for multiplication\n");
			return FAILURE;
	}
	// remove leading zeros from result
	remove_leading_zeros(&headR, &tailR);
	/* printing the result list */
	printf("Result : ");
	print_list(headR);
    printf("\n");

	return 0;
}
