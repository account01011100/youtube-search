#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

//reasonable limits, youtubes maximum character lenght
//for the title would be 100 chars and others will be way less
typedef struct {
    char title[256];
    char length[256];
    char channel[256];
    char age[256];
    char link[256];
} VideoInfo;

size_t videoCount = 0;

VideoInfo* parse_json(const char *jsonString) {
    VideoInfo *videos = NULL;
    int counter = 0;
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        printf("Error parsing JSON\n");
    }

    cJSON *contents = cJSON_GetObjectItem(json, "contents");
    cJSON *twoColumnSearchResults = cJSON_GetObjectItem(contents, "twoColumnSearchResultsRenderer");
    cJSON *primaryContents = cJSON_GetObjectItem(twoColumnSearchResults, "primarycontents");
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

        cJSON *lenghtText = cJSON_GetObjectItem(videoRenderer, "lengthText");

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
        if (lenghtText != NULL) {
            length = cJSON_GetObjectItem(lenghtText, "simpleText")->valuestring;
        }
        if (ownerItem != NULL) {
            channel = cJSON_GetObjectItem(ownerItem, "text")->valuestring;
        }
        if (videoRenderer != NULL) {
            link = cJSON_GetObjectItem(videoRenderer, "videoId")->valuestring;
        }
        counter++;
        if (videoCount < counter) {
            VideoInfo *temp = realloc(videos, sizeof(VideoInfo)*(counter));
            if (!temp) {
                fprintf(stderr, "reallocating videos array failed\n");
                free(videos);
                return NULL;
            }
            videos = temp;
            strncpy(videos[videoCount].title, title, sizeof(videos[videoCount].title) - 1);
            strncpy(videos[videoCount].age, age, sizeof(videos[videoCount].age) - 1);
            strncpy(videos[videoCount].length, length, sizeof(videos[videoCount].length) - 1);
            strncpy(videos[videoCount].channel, channel, sizeof(videos[videoCount].channel) - 1);
            strncpy(videos[videoCount].link, link, sizeof(videos[videoCount].link) - 1);
            videoCount++;
        }
    }
    cJSON_Delete(json);
    return videos;
}
