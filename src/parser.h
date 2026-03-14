#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct {
    char *title;
    char *length;
    char *channel;
    char *age;
    char *link;
} videoInfo;

typedef struct {
    videoInfo *videos;
    int count; 
} videoArray;

videoArray* parseJson(const char *jsonString);

#endif
