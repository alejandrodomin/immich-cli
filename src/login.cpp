#include "../lib/login.hpp"

#include <curl/curl.h>

using namespace std;

void login(string &url, string &key) {
    cout << "Login into server at " << url << " with key " << key << endl;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("x-api-key: " + key).c_str());

    CURL *curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res_code = curl_easy_perform(curl);

        cout << "Response code: " << curl_easy_strerror(res_code) << "\n"
             << response << "" << endl;

        curl_easy_cleanup(curl);
    }
}

size_t write_response(char *res_data, size_t size, size_t el_num,
                      void *user_data) {
    string *response = static_cast<string *>(user_data);
    response->append(res_data, size * el_num);
    return size * el_num;
}
