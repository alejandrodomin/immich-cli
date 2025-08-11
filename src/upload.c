#include <curl/curl.h>
#include <stdio.h>

#include "cmd_utils.h"
#include "curl_utils.h"

void upload(char *argv[]) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Unable to initialize curl handler.");
        return;
    }

    CURLcode res;

    char *response = nullptr;
    curl_easy_setopt(curl, CURLOPT_URL, "127.0.0.1");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_res);

    res = curl_easy_perform(curl);
    printf("Return code %d\n", res);
    printf("Returned data %s\n", response);

    free(response);
    curl_easy_cleanup(curl);
}
