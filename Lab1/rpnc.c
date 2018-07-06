#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/*
 * Reverse Polish Notation (RPN) Calculator
 *
 * Accepts a single argument in RPN, evaluates it and prints the answer to stdout.
 * 
 * Returns -1 on error, 0 otherwise.
 */
 
// Global Variables
int stack[40];
int pointer = -1;

// Stack Functions
void push(int element)
{
	pointer = pointer + 1;
	stack[pointer] = element;
}

void pop()
{
	pointer = pointer - 1;
}

int top()
{
	return stack[pointer];
}

int empty()
{
	if(pointer == -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}

void printStack()
{
	int i;
	printf("stack: ");
	for(i = 0; i <= pointer; i++)
	{
		printf("%d ", stack[i]);
	}
	printf("\n");
}

// Arithmetric Functions
int addition(int oper1, int oper2)
{
	int sum;
	sum = oper1 + oper2;
	return sum;
}

int subtraction(int oper1, int oper2)
{
	int difference;
	difference = oper1 - oper2;
	return difference;
}

int multiplication(int oper1, int oper2)
{
	int product;
	product = oper1 * oper2;
	return product;
}

int division(int oper1, int oper2)
{
	int dividen;
	dividen = oper1/oper2;
	return dividen;
}

int powerResult(int oper1, int oper2)
{
	return pow(oper1, oper2);
}

// Main Funciton
int main(int argc, char *argv[]) 
{
	char *token = strtok(argv[1], " ");
	while(token != NULL)
	{
		// Is an operand
		if(isdigit(token[0]) != 0)
		{
			push(atoi(token));
		}
		// Is an operator
		else
		{
			int oper2 = stack[pointer];
			pop();
			int oper1 = stack[pointer];
			pop();
			if(strcmp(token,"+") == 0)
			{
				//printf("addition\n");
				int result = addition(oper1, oper2);
				push(result);
			}
			else if(strcmp(token,"-") == 0)
			{
				//printf("subtraction\n");
				int result = subtraction(oper1, oper2);
				push(result);
			}
			else if(strcmp(token,"*") == 0)
			{
				//printf("multiplication\n");
				int result = multiplication(oper1, oper2);
				push(result);
			}
			else if(strcmp(token,"/") == 0)
			{
				//printf("division\n");
				int result = division(oper1, oper2);
				push(result);
			}
			else if(strcmp(token,"^") == 0)
			{
				//printf("power\n");
				int result = powerResult(oper1, oper2);
				push(result);
			}
		}
		token = strtok(NULL, " ");		
	}
	//printStack();
	printf("%d \n", stack[0]);
    return -1;
}
