#include "../lib/upload.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>

#include "../lib/constants.hpp"
#include "../lib/login.hpp"

using namespace std;
namespace fs = std::filesystem;

string iso_time(fs::file_time_type file_time);

void upload(const string& url, const string& key) {
    CURL* curl = curl_easy_init();
    string response;

    if (!curl) {
        cerr << "Error initializing curl." << endl;
        return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, (url + "/api/assets").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, (TOKEN_HEADER_NAME + ": " + key).c_str());
    headers = curl_slist_append(headers, ACCEPT_APP_JSON.c_str());

    fs::path curr_dir = fs::current_path();
    cout << "Searching for files under " << curr_dir << "\n";

    for (const auto& file : fs::recursive_directory_iterator(curr_dir)) {
        if (fs::file_type::regular == fs::status(file).type()) {
            string ext = file.path().extension().string();
            auto res = find(SUPPORTED_FILE_TYPES.begin(), SUPPORTED_FILE_TYPES.end(), ext);

            if (SUPPORTED_FILE_TYPES.end() != res) {
                string hash_header = "x-immich-checksum: " + to_string(fs::hash_value(file));
                headers = curl_slist_append(headers, hash_header.c_str());
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                upload_file(curl, file);
                cout << "response from immich " << response << " for file " << file << endl;
                break;
            }
        }
    }

    curl_easy_cleanup(curl);
}

void upload_file(CURL* curl, const fs::path& file) {
    curl_mime* mime = curl_mime_init(curl);

    curl_mimepart* data_field = curl_mime_addpart(mime);
    curl_mime_name(data_field, "assetData");
    curl_mime_filedata(data_field, file.string().c_str());

    curl_mimepart* id_field = curl_mime_addpart(mime);
    curl_mime_name(id_field, "deviceAssetId");
    string raw_name = file.filename().string();
    raw_name.erase(remove(raw_name.begin(), raw_name.end(), ' '), raw_name.end());
    string name = raw_name + "-" + to_string(fs::file_size(file));
    curl_mime_data(id_field, name.c_str(), CURL_ZERO_TERMINATED);

    curl_mimepart* device_field = curl_mime_addpart(mime);
    curl_mime_name(device_field, "deviceId");
    curl_mime_data(device_field, "CLI", 3);

    string time = iso_time(fs::last_write_time(file));
    curl_mimepart* created_field = curl_mime_addpart(mime);
    curl_mime_name(created_field, "fileCreatedAt");
    curl_mime_data(created_field, time.c_str(), CURL_ZERO_TERMINATED);

    curl_mimepart* modified_field = curl_mime_addpart(mime);
    curl_mime_name(modified_field, "fileModifiedAt");
    curl_mime_data(modified_field, time.c_str(), CURL_ZERO_TERMINATED);

    cout << "Found 1 valid file " << file << " to upload." << endl;
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    CURLcode res_code = curl_easy_perform(curl);
    if (CURLE_OK == res_code) {
        cout << "Successfully uploaded " << file << " to immich." << endl;
    }

    curl_mime_free(mime);
}

string iso_time(fs::file_time_type file_time) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
    std::ostringstream oss;
    oss << std::put_time(std::gmtime(&cftime), "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}
