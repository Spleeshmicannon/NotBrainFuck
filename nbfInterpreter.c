/*
 * This a modified version of the interpreter made by Urban Müller.
 */

//#include "pch.h"
#include <stdio.h>
#define PROGRAM_MEMORY 5000
#define GOTO_MEMORY 20

// type definitions and struct
typedef enum { false = 0, true = 1 } bool; // making an enum for booleans, better than macros
typedef char string[PROGRAM_MEMORY];

struct stringAndPointer
{
	int i; // position in array
	char * p; // pointer to array 
	string a; // array
};

struct gotoExpressions
{
	char a[GOTO_MEMORY], l[GOTO_MEMORY];
	int i;
};

typedef struct stringAndPointer sap;

// globals
char b, o;
sap out, in;

// meta functions

int strLength(const char *str) 
{
	int length = 0;

	while (*str++) length++;

	return length;
}

bool compareString(char *str0, char *str1)
{
	int length;

	if ((length = strLength(str0)) != strLength(str1))
	{
		return false;
	}

	for(int i = 0; i < length; i++)
	{
        	if(*str0++ != *str1++) return false;
	}

	return true;
}

// the main program

void interpret(char *c, int argc)
{
	char *d; // for assigning c to when the [] loop is activated on lines 43 - 60
	int base = 1; // goto the ^ and v cases for info vvvvv
	int comments = false; // a boolean, true when comments are active and false when they're not
	
	// storage of labels and other related data
	struct gotoExpressions jmp;

	int oldJmpI;
	bool changed = false;
	
	for(jmp.i = 0; jmp.i < GOTO_MEMORY; jmp.i++)
	{
		jmp.a[jmp.i] = 0;
	}

	jmp.i = 0;

	while (*c)
	{
		if (!comments)
		{
			switch (o = 1, *c++)
			{
			// added stuff
			case '|': comments = true;			 			 break; // adds commenting properly as: code here|comment here|code here
			case '*': out.a[out.i] *= out.a[out.i + 1]; 	 break; // multiplies current cell by its upper neighbour
			case 'x': out.a[out.i] *= out.a[out.i - 1];		 break; // multiplies current cell by its lower neighbour
			case '/':	// divides current cell by its upper neighbour
				if((out.a[out.i] != 0)&&(out.a[out.i + 1] != 0)) 
					out.a[out.i] /= out.a[out.i + 1];
				else 
				{
					puts("CAN'T DIVIDE BY ZERO");
					return;
				}
				break;
			case'\\':	// divides current cell by its lower neighbour
				if((out.a[out.i] != 0)&&(out.a[out.i - 1] != 0))
					out.a[out.i] /= out.a[out.i - 1];	
				else
				{ 
					puts("CAN'T DIVIDE BY ZERO");
					return;	 
				}
				break;
			case '#': out.a[out.i] = *c; 			 		 break; // makes the current cell = the next character as a char value (syntax for making cell = #d)
			case '^': base++;				 				 break; // increases the base
			case 'v': if (base > 1) base--;			 		 break; // decreases the base if is larger than one
			case '{': 
				jmp.a[jmp.i] = *c++;
				jmp.l[jmp.i] = *c;
				jmp.i++;
				break;
			case '}':
				oldJmpI = jmp.i--;

				if(jmp.a[jmp.i] == 0)
				{
					puts("CAN'T FIND LABEL");
					return;
				}

				*c++;

				for(int i = 0; i < GOTO_MEMORY;i++)
				{
					if(jmp.l[i] == *c)
					{
						jmp.i = i;
						changed = true;
						break;
					}
				}

				if(!changed)
				{
					puts("CAN'T FIND LABEL");
					return;
				}

				*c--;

				if(out.a[out.i] != 0) // checking value of current cell
				{
					*c -= (*c - jmp.a[jmp.i]); // moving pointer
				}

				jmp.i = oldJmpI; // putting jmp.i back the way it was
				break;
			
			// original BrainFuck
			case '<': 
				if (out.i > 0) out.i--;
				else
				{
					puts("POINTER IS NEGATIVE");
					return;
				}
				break;
			case '>': out.i++;                                break;
			case '+': out.a[out.i] += base;			 break;
			case '-': out.a[out.i] -= base;			 break;
			case '.': putchar(out.a[out.i]); fflush(stdout);	 break;
			case ',': out.a[out.i] = getchar(); fflush(stdout); break;
			case '[':
				for (b = 1, d = c; b && *c; c++)
				{
					b += *c == '[', b -= *c == ']';
				}

				if (!b)
				{
					c[-1] = 0;
					while (out.a[out.i])
					{
						interpret(d, argc);
					}
				}

				c[-1] = ']';
				break;

			case ']':
				puts("UNBALANCED BRACKETS");
				return;
			default: o = 0;
			}
		}
		else if (*c++ == '|')
		{
			comments = !comments;
		}

		if (out.i < 0 || out.i>100)
		{
			puts("RANGE ERROR");
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	// initialising structs/new types
	in.p = in.a;
	in.i = 0;

	out.p = out.a;
	out.i = 0;

	char * f = "-f";
	char * i = "-i";

	if (compareString(argv[1], f))
	{
		// for interpreting programs as files
		printf("Proccessing file...\n");
		FILE * program = fopen(argv[2], "r");

		if (program != NULL)
		{
			while ((b = getc(program)) > 0)
			{
				*in.p++ = b;
			}
			*in.p = 0;
			interpret(in.a, argc);
		}
	}
	else if (compareString(argv[1], i))
	{
		// message used to explain use
		printf("Write your program, character by character, using enter to interpret it as you need to.\n\n");
		
		// initialising variable
		char command;

		// A while loop where, getchar is assigned to command.
		// Then it is checked if command is an EOF or \n character.
		// Then the loop iterates through *pin and in[] as it does above and below.
		// The loop label is so when n is pressed below the loop can be gone back to.
		loop: while ((command = getchar()) != EOF && command != '\n') /////////// LOOP
		{
			*in.p++ = command;
		}
		
		printf("Would you like to interpret the program (press y for yes, n for no, e for exit)\n");
		

		// goes back here if wrong input
		// if yes the program is interpreted
		question: if ((command = getchar()) == 'y') ///////// QUESTION
		{
			*in.p = 0;
			interpret(in.a, argc);
			printf("Would you like to continue programming (n) or exit (e)\n");
			goto question;
		}
		else if (command == 'n')
		{
			goto loop;
		}
		else if (command == 'e')
		{
			printf("exiting\n");	
		}
		else
		{
			printf("Please press 'y', 'n' or 'e'\n");
			goto question;
		}
	}
	else
	{
		// for interpreting programs as command line arguments
		printf("Processing command line arguments as a code.\n");
		for (int i = 1; i < argc; i++) // iterating through each argument seperated by spaces
		{
			for (int j = 0; j < strLength(argv[i]); j++) // iterating through each char
			{
				*in.p++ = (char)((argv[i])[j]); // assigning character to in[] and increment the pointer
			}
			if(i < argc) *in.p++ = ' '; // adding in spaces based on argc and i
		}
		*in.p = 0;
		interpret(in.a, argc);
	}
	return 0; ////////// EXIT
}
