#include <stdio.h>
#define TRUE 1
#define FALSE 0

int p = 0;
int r = 0;
char out[5000], in[5000], b, o, *pointer=in;

void interpret(char *c, int argc)
{
	char *d;
	int base = 1;
	int comments = FALSE;

	while( *c )
    {
		//if(strchr("<>+-,.[]*/#\n",*c))printf("%c",*c);
		if(!comments)
		{
		switch(o=1,*c++) 
        {
			// added stuff
			case '|': comments = TRUE;				  break; // adds commenting properly as: code here|comment here|code here
			case '*': out[p] *= out[p+1]; 			  break; // multiplies current cell by its upper neighbour
			case 'x': out[p] *= out[p-1];			  break; // multiplies current cell by its lower neighbour
			case '/': out[p] /= out[p+1];			  break; // divides current cell by its upper neighbour
			case'\\': out[p] /= out[p-1];			  break; // divides current cell by its lower neighbour
			case '#': out[p] = *c; 			   		  break; // makes the current cell = the next character as a char value (syntax for making cell = d:#d)
			case '^': base++;						  break; // increases the base
			case 'V': if(base > 1) base--;			  break; // decreases the base if is larger than one

			// original BrainFuck
			case '<': p--;                            break;
			case '>': p++;                            break;
			case '+': out[p] += base;                 break;
			case '-': out[p] -= base;                 break;
			case '.': putchar(out[p]); fflush(stdout);break;
			case ',': out[p]=getchar();fflush(stdout);break;
			case '[':
				for( b=1,d=c; b && *c; c++ )
				{
					b+=*c=='[', b-=*c==']';
				}

				if(!b) 
				{
					c[-1]=0;
					while( out[p] )
					{
						interpret(d, argc);
					}
				}

				c[-1]=']';
				break;

			case ']':
				puts("UNBALANCED BRACKETS");
				return;
			default: o=0;
		}
		}
		else if(*c++ == '|')
		{
			comments = !comments;
		}
		
		if( p<0 || p>100)
			puts("RANGE ERROR");
	}
}

int main(int argc,char *argv[])
{
	FILE * program;

	if((program=fopen(argv[1],"r"))) 
    {
		while( (b=getc(program))>0 )
        {
			*pointer++=b;
        }
        *pointer=0;

		interpret(in, argc);
	}

    return r;
}