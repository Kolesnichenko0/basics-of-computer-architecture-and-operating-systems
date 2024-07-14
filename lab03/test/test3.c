#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main()
{
    extern char **environ;
    for (char **e = environ; *e; e++)
        printf ("%s\n", *e);
    return 0;
}