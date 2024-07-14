#include <stdio.h>
#include <getopt.h>

int main (int argc, char *argv[]) {
    int c;
    while (true) {
        c = getopt(argc, argv, "hlf:");
        if (c == -1) break;
        switch (c) {
            case 'h':
                printf ("использование: %s [-h] [-l] [-f имя]\n", argv[0]);
                break;
            case 'l':
                printf ("задано -l\n");
                break;
            case 'f':
                printf ("задано -f с параметром: %s\n", lsoptarg);
                break;
        }
    }
}