#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"

char *argHelper(int argc, char *argv[]) {
    char *query = NULL;
    if (argc < 2) {
        return(NULL);
    }
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                printf(HELP_MSG);
                exit(0);  
            } else if (argv[i][0] != '-') {
                query = argv[i];
            } else {
                printf("Please see -h or --help for usage.\n");
                exit(0);
            }
        }
    }
    return(query);
}
