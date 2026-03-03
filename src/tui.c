#include <stdio.h>

void clear() {
    // clean the terminal and jump to 0,0
    printf("\033[H\033[J");
    fflush(stdout);
}

void reverseVideo(bool rev) {
    if (rev) {
        printf("\033[7m");
        fflush(stdout);
    } else {
        printf("\033[0m");
        fflush(stdout);
    }
}
