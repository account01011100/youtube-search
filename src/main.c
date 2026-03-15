#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "request.h"
#include "parser.h"
#include "tui.h"
#include "helper.h"

int playVideo(char *link) {
    size_t size = strlen(link);
    char *start = "mpv https://youtu.be/";
    char *end = "> /dev/null 2>&1";
    char command[strlen(start)+strlen(link)+strlen(end)+1];
    snprintf(command, sizeof(command), "%s%s%s", start, link, end);
    system(command);
}

void draw(videoArray *vidArr, int highlight) {
    clear();
    for (int i = 0; i < vidArr->count; ++i) {
        if (i == highlight) {
            reverseVideo(true);
        }
        printf("%d) %s\n %s %s %s\n", i+1, vidArr->videos[i].title, vidArr->videos[i].channel, vidArr->videos[i].length, vidArr->videos[i].age);
        if (i == highlight) {
            reverseVideo(false);
        }
    }
}

bool tui(videoArray *vidArr) {
    int ch;
    int done;
    int highlight = 0;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    draw(vidArr, highlight);

    done = 0;
    while (!done) {
        ch = getchar();
        switch (ch) {
            case 'w':
            case 'k':
                highlight = (highlight > 0) ? highlight -1 : vidArr->count - 1;
                draw(vidArr, highlight);
                break;
            case 's':
            case 'j':
                highlight = (highlight + 1) % vidArr->count;
                draw(vidArr, highlight);
                break;
            case '\n':
            case 'p':
                playVideo(vidArr->videos[highlight].link);
                break;
            case 'q':
                done = 1;
                break;
            case 'r':
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return true;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return false;
}

int main(int argc, char *argv[]) {
    
    char *json;
    char *query;
    char *post_url;
    char *payload;
    bool input;
    bool run = true;

    query = argHelper(argc, argv);
    while (run) {
        videoArray *vidArr;
        if (query == NULL) {
            int e = 0;
            clear();
            printf("Search: ");
            input = true;
            query = malloc(sizeof(char)+1);
            while(input) {
                query[e] = fgetc(stdin);
                if (query[e] != '\n') {
                    e++;
                    query = realloc(query, sizeof(char)*e+1);
	            } else {
		            input = 0;
	            }
            }
        }
        json = search(query);
        vidArr = parseJson(json);
        free(json);
        if (vidArr == NULL) {
            return(1);
        } else {
            run = tui(vidArr);
            if (run) {
                query = NULL;
            }
        }
        for (int i = 0; i < vidArr->count; ++i) {
            free(vidArr->videos[i].title);
            free(vidArr->videos[i].age);
            free(vidArr->videos[i].length);
            free(vidArr->videos[i].channel);
            free(vidArr->videos[i].link);
        }
        free(vidArr->videos);
        free(vidArr);
    }
    return(0);
}
