#include "upload.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <deque>
#include <filesystem>
#include <iostream>
#include <thread>
#include <utility>

#include "constants.hpp"
#include "curl-util.hpp"
#include "statx-util.hpp"

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;
using json = nlohmann::json;

curl_slist* headers(size_t, const string&);
bool is_supported_format(const fs::directory_entry&);
vector<fs::path> dir_crawl(const fs::path&);
void batch_upload(vector<fs::path>, CURL*, const string&);

void upload() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Error initializing curl." << endl;
        return;
    }

    json creds = ret_creds();
    if (!creds.contains("server") && !creds.contains("secret")) {
        cerr << "No valid credentials found, please login first.\n";
        return;
    }

    string url = creds.at("server");
    string key = creds.at("secret");

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, (url + "/api/assets").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);

    fs::path curr_dir = fs::current_path();
    vector<fs::path> imgs = dir_crawl(curr_dir);
    auto start = imgs.begin();

    auto workers = vector<thread>{};
    while (imgs.end() > start) {
        auto end = start + SIZE_OF_BATCH;
        if (end >= imgs.end()) end = imgs.end();

        vector<fs::path> sub_imgs(start, end);
        thread worker(batch_upload, std::move(sub_imgs), curl_easy_duphandle(curl), key);
        workers.push_back(std::move(worker));

        start = end;
    }

    for (auto& entry : workers) {
        if (entry.joinable()) entry.join();
    }
    cout << "\x1b[2Kfinished uploading " << imgs.size() << "\n";

    // cout << "Uploaded a total of " << count << " files\n.";
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

    string m_time = iso_time(fs::last_write_time(file));
    string b_time = m_time;
    if (STATX_COMPATIBLE) {
        b_time = iso_time(birth_time(file));
    }

    curl_mimepart* created_field = curl_mime_addpart(mime);
    curl_mime_name(created_field, "fileCreatedAt");
    curl_mime_data(created_field, b_time.c_str(), CURL_ZERO_TERMINATED);

    curl_mimepart* modified_field = curl_mime_addpart(mime);
    curl_mime_name(modified_field, "fileModifiedAt");
    curl_mime_data(modified_field, m_time.c_str(), CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    CURLcode res_code = curl_easy_perform(curl);
    if (CURLE_OK == res_code) {
        // cout << "Successfully uploaded " << file << " to immich." << endl;
    }

    curl_mime_free(mime);
}

curl_slist* headers(size_t hash, const string& key) {
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, (TOKEN_HEADER_NAME + ": " + key).c_str());
    headers = curl_slist_append(headers, ACCEPT_APP_JSON.c_str());

    string hash_header = "x-immich-checksum: " + to_string(hash);
    headers = curl_slist_append(headers, hash_header.c_str());

    return headers;
}

bool is_supported_format(const fs::directory_entry& file) {
    if (fs::file_type::regular != fs::status(file).type()) {
        return false;
    }

    string ext = file.path().extension().string();
    transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return tolower(c); });
    if (!SUPPORTED_FILE_TYPES.count(ext)) {
        return false;
    }

    return true;
}

vector<fs::path> dir_crawl(const fs::path& dir) {
    auto imgs = vector<fs::path>{};
    if (fs::file_type::directory != fs::status(dir).type()) {
        return imgs;
    }

    deque<fs::path> dirs = {dir};
    while (!dirs.empty()) {
        fs::path curr_dir = dirs.front();
        dirs.pop_front();

        cout << "searching through dir " << curr_dir << " ...\r";
        for (const auto& entry : fs::directory_iterator(curr_dir)) {
            if (fs::file_type::directory == fs::status(entry).type()) {
                dirs.push_back(entry);
            } else if (fs::file_type::regular == fs::status(entry).type() && is_supported_format(entry)) {
                imgs.push_back(entry);
            }
        }
    }

    cout << "\x1b[2Kfound " << imgs.size() << " file(s)\n";
    return imgs;
}

void batch_upload(vector<fs::path> imgs, CURL* curl, const string& key) {
    cout << "upload batch of " << imgs.size() << " image(s)\r" << flush;
    for (auto file : imgs) {
        curl_slist* h_list = headers(fs::hash_value(file), key);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, h_list);

        // TODO: add json check for success upload and duplicate upload.
        upload_file(curl, file);
        curl_slist_free_all(h_list);
    }

    curl_easy_cleanup(curl);
}
