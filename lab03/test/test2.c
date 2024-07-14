#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int c;
    int aflag = 0;
    int bflag = 0;
    char *ofile = NULL;

    while ((c = getopt(argc, argv, "abo:")) != -1)
    {
        switch (c)
        {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'o':
                ofile = optarg;
                break;
        }
    }

    printf("aflag = %d, bflag = %d, ofile = %s\n", aflag, bflag, ofile);

    return 0;
}