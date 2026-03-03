#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct {
    char title[256];
    char length[256];
    char channel[256];
    char age[256];
    char link[256];
} VideoInfo;

extern size_t videoCount;

VideoInfo* parse_json(const char *jsonString);

#endif
