#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *argHelper(int argc, char *argv[]) {
    char *query = NULL;
    if (argc < 2) {
        printf("Please see -h or --help for usage.\n");
        return NULL;
    }
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            char *arg = argv[i];
            if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
                printf("Help TODO: ADD HELP\n"); //TODO: ADD HELP
                return NULL;
            } else if (arg[0] != '-'|| (arg[0] != '-' && arg[1] != '-')) {
                query = arg;
            } else {
                printf("Please see -h or --help for usage.\n");
                return NULL;
            }
        }
    }
    return query;
}
