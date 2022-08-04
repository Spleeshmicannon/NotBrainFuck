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
	char * a[GOTO_MEMORY];
	char l[GOTO_MEMORY];
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
			case '|': 
				comments = true;			 		
				break; // adds commenting properly as: code here|comment here|code here
			case '*': 
				out.a[out.i] *= out.a[out.i + 1]; 
				break; // multiplies current cell by its upper neighbour
			case 'x': 
				out.a[out.i] *= out.a[out.i - 1];
				break; // multiplies current cell by its lower neighbour
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
			case '#': 
				out.a[out.i] = *c; 
				break; // makes the current cell = the next character as a char value (syntax for making cell = #d)
			case '^':
				base++;	
				break; // increases the base
			case 'v': 
				if (base > 1) base--;	 
				break; // decreases the base if is larger than one
			case '{': 
				jmp.a[jmp.i] = c;
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

				if(changed)
				{
					puts("CAN'T FIND LABEL");
					return;
				}

				*c--;

				if(out.a[out.i] != 0) // checking value of current cell
				{
					c = jmp.a[jmp.i]; // moving pointer
				}

				jmp.i = oldJmpI; // putting jmp.i back the way it was
				changed = false;
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
			case '>': 
				out.i++; 
				break;
			case '+':
				out.a[out.i] += base;
				break;
			case '-': 
				out.a[out.i] -= base;
				break;
			case '.': 
				putchar(out.a[out.i]); fflush(stdout);
				break;
			case ',': 
				out.a[out.i] = getchar(); fflush(stdout); 
				break;
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

int main(int argc, char* argv[])
{
	// initialising structs/new types
	in.p = in.a;
	in.i = 0;

	out.p = out.a;
	out.i = 0;

	// for interpreting programs as files
	if (argc > 1)
	{
		printf("Proccessing file...\n");

		FILE* program;
		int err = fopen_s(&program, argv[1], "r");

		if (err != 0)
		{
			printf("file not opened successfully\n");
		}

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
	else
	{
		printf("Not enough arguments, must provide a file to read.\n");
	}
}

