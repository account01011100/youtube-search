#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_SEARCH_TERM_LENGTH 255
#define PAYLOAD_LENGHT 80

typedef struct {
  char *memory;
  size_t mem_size;
} MemoryStruct;

static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->mem_size + realsize + 1);

    if(!ptr) {
        fprintf(stderr, "reallocating mem->memory failed\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->mem_size]), contents, realsize);
    mem->mem_size += realsize;
    mem->memory[mem->mem_size] = 0;

    return realsize;
}

char *search(char *query) {
    char payload[PAYLOAD_LENGHT+MAX_SEARCH_TERM_LENGTH];

    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.mem_size = 0;

    CURL *curl;

    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    if(result != CURLE_OK)
        return(NULL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Content-Type: application/json");

        snprintf(payload, sizeof(payload), "{\"context\":{\"client\":{\"clientName\":\"WEB\",\"clientVersion\":\"2.20260206.01.00\"}},\"query\":\"%s\"}", query);

        curl_easy_setopt(curl, CURLOPT_URL, "https://www.youtube.com/youtubei/v1/search");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        result = curl_easy_perform(curl);

        if (result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return(chunk.memory);
}
