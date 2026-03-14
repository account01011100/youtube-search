#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "parser.h"

videoArray* parseJson(const char *jsonString) {
    int counter = 0;
    videoArray *vidArr;
    vidArr = malloc(sizeof(videoArray));
    vidArr->videos = malloc(sizeof(videoInfo));
    vidArr->count = 0;
    
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON\n");
    }

    cJSON *contents = cJSON_GetObjectItem(json, "contents");
    cJSON *twoColumnSearchResults = cJSON_GetObjectItem(contents, "twoColumnSearchResultsRenderer");
    cJSON *primaryContents = cJSON_GetObjectItem(twoColumnSearchResults, "primaryContents");
    cJSON *sectionList = cJSON_GetObjectItem(primaryContents, "sectionListRenderer");
    cJSON *contentsArray = cJSON_GetObjectItem(sectionList, "contents");
    cJSON *itemSection = cJSON_GetArrayItem(contentsArray, 0);
    cJSON *itemContents = cJSON_GetObjectItem(itemSection, "itemSectionRenderer");
    cJSON *videoContents = cJSON_GetObjectItem(itemContents, "contents");
    int videoArraySize = cJSON_GetArraySize(videoContents);
    for (int i = 0; i < videoArraySize; ++i) {
        cJSON *videoItem = cJSON_GetArrayItem(videoContents, i);
        cJSON *videoRenderer = cJSON_GetObjectItem(videoItem, "videoRenderer");
        if (videoRenderer == NULL) {
            continue;
        }
        cJSON *titleObject = cJSON_GetObjectItem(videoRenderer, "title");
        cJSON *runsArray = cJSON_GetObjectItem(titleObject, "runs");
        cJSON *firstRun = cJSON_GetArrayItem(runsArray, 0);

        cJSON *publishedTime = cJSON_GetObjectItem(videoRenderer, "publishedTimeText");

        cJSON *lengthText = cJSON_GetObjectItem(videoRenderer, "lengthText");

        cJSON *ownerText = cJSON_GetObjectItem(videoRenderer, "ownerText");
        cJSON *ownerRuns = cJSON_GetObjectItem(ownerText, "runs");
        cJSON *ownerItem = cJSON_GetArrayItem(ownerRuns, 0);

        const char *title = "NULL";
        const char *age = "NULL";
        const char *length = "NULL";
        const char *channel = "NULL";
        const char *link = "NULL";

        if (firstRun != NULL) {
            title = cJSON_GetObjectItem(firstRun, "text")->valuestring;
        }
        if (publishedTime != NULL) {
            age = cJSON_GetObjectItem(publishedTime, "simpleText")->valuestring;
        }
        if (lengthText != NULL) {
            length = cJSON_GetObjectItem(lengthText, "simpleText")->valuestring;
        }
        if (ownerItem != NULL) {
            channel = cJSON_GetObjectItem(ownerItem, "text")->valuestring;
        }
        if (videoRenderer != NULL) {
            link = cJSON_GetObjectItem(videoRenderer, "videoId")->valuestring;
        }
        counter++;
        if (vidArr->count < counter) {
            videoInfo *temp = realloc(vidArr->videos, sizeof(videoInfo)*(counter));
            if (!temp) {
                fprintf(stderr, "reallocating videos array failed\n");
                free(vidArr->videos);
                return NULL;
            }
            vidArr->videos = temp;
            
            vidArr->videos[vidArr->count].title = malloc(strlen(title) + 1);
            vidArr->videos[vidArr->count].length = malloc(strlen(length) + 1);
            vidArr->videos[vidArr->count].channel = malloc(strlen(channel) + 1);
            vidArr->videos[vidArr->count].age = malloc(strlen(age) + 1);
            vidArr->videos[vidArr->count].link = malloc(strlen(link) + 1);
// strcpy adds a null terminator
            strcpy(vidArr->videos[vidArr->count].title, title);
            strcpy(vidArr->videos[vidArr->count].age, age);
            strcpy(vidArr->videos[vidArr->count].length, length);
            strcpy(vidArr->videos[vidArr->count].channel, channel);
            strcpy(vidArr->videos[vidArr->count].link, link);
            
            vidArr->count++;
        }
    }
    cJSON_Delete(json);
    return vidArr;
}
