#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

extern int liberty_parser_parse(void);
extern void liberty_parser_report(void);


/* test program for liberty parser */

int main(int argc, char **argv)
{
	extern FILE *liberty_parser2_in;
	liberty_parser2_in = fopen(argv[1],"r");
    liberty_parser_parse();
    fclose(liberty_parser2_in);
    liberty_parser_report();

#if 0
    {
        int j,k;
        double x,y;
        for (j=0;j<1234;j++)
        for (k=0;k<123456;k++) {
            x = j+k;
            y = exp(-x);
            x = exp(y);
        }
    }
#endif

	exit(0);
}
