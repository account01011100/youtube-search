#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SEARCH_HEADERS "Content-Type: application/json"
#define SEARCH_URL "https://www.youtube.com/youtubei/v1/search"
//it's important to put %s after the query so snprintf can use that if the payload were to change
#define SEARCH_PAYLOAD "{\"context\":{\"client\":{\"clientName\":\"WEB\",\"clientVersion\":\"2.20260206.01.00\"}},\"query\":\"%s\"}"

#endif
