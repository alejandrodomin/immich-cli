#include "../lib/login.hpp"

#include <curl/curl.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "../lib/constants.hpp"

using json = nlohmann::json;
using namespace std;

void login(const string &url, const string &key) {
    CURL *curl = curl_easy_init();
    string response;

    if (!curl) {
        cerr << "Error initializing curl." << endl;
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, (url + TOKEN_VALIDATION_PATH).c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, (TOKEN_HEADER_NAME + ": " + key).c_str());
    headers = curl_slist_append(headers, ACCEPT_APP_JSON.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res_code = curl_easy_perform(curl);
    json res_j = json::parse(response);

    if (CURLE_OK == res_code && res_j.contains("authStatus") && res_j.at("authStatus") == true) {
        // todo: save the creds for use later in the upload command
        cout << "Successfully logged in to the server." << endl;
    } else {
        cerr << "Response error: " << curl_easy_strerror(res_code) << endl;
    }

    curl_easy_cleanup(curl);
}

size_t write_response(char *res_data, size_t size, size_t el_num, void *user_data) {
    string *response = static_cast<string *>(user_data);
    response->append(res_data, size * el_num);
    return size * el_num;
}
