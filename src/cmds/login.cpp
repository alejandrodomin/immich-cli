#include "login.hpp"

#include <curl/curl.h>

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

#include "constants.hpp"
#include "curl-util.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;

void login(const string &url, const string &key) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        cerr << "Error initializing curl." << endl;
        return;
    }

    string response;
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
        save_creds(url, key);
        cout << "Successfully logged in to the server." << endl;
    } else {
        cerr << "Response error: " << curl_easy_strerror(res_code) << endl;
    }

    curl_easy_cleanup(curl);
}

void logout() {
    if (!fs::exists(AUTH_FILE_LOC)) {
        cout << "Not logged in, no further action needed\n";
        return;
    }

    if (!fs::remove(AUTH_FILE_LOC)) {
        cerr << "Error during logout process\n";
        return;
    }

    cout << "Logged out successfully\n";
}
