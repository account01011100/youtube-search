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

void draw(VideoInfo *videos, int highlight) {
    clear();
    for (int i = 0; i < videoCount; ++i) {
            if (i == highlight) {
                reverseVideo(true);
            }
            printf("%d) %s\n", i+1, videos[i].title);
            if (i == highlight) {
                reverseVideo(false);
            }
    }
}

void tui(VideoInfo *videos) {
    int ch;
    int done;
    int highlight = 0;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    draw(videos, highlight);

    done = 0;
    while (!done) {
        ch = getchar();
        switch (ch) {
            case 'w':
            case 'k':
                highlight = (highlight > 0) ? highlight -1 : videoCount - 1;
                draw(videos, highlight);
                break;
            case 's':
            case 'j':
                highlight = (highlight + 1) % videoCount;
                draw(videos, highlight);
                break;
            case '\n':
            case 'p':
                playVideo(videos[highlight].link);
                break;
            case 'q':
                done = 1;
                break;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return;
}

int main(int argc, char *argv[]) {
    //FILE* fptr; TBD
    //fptr = fopen("config.txt", "r")
    char *json;
    char *query;
    VideoInfo *videos;
    query = argHelper(argc, argv);
    if (query != NULL) {
        json = search(argv[1]);
        videos = parse_json(json);
        free(json);
        if (videos == NULL) {
            return(1);
        } else {
            tui(videos);
            free(videos);
        }
    }
    return(0);
}
