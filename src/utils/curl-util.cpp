#include "curl-util.hpp"

#include <fstream>
#include <iostream>

#include "constants.hpp"

using namespace std;
using namespace chrono;
namespace fs = std::filesystem;
using json = nlohmann::json;

string iso_time(fs::file_time_type file_time) {
    auto sctp =
        time_point_cast<system_clock::duration>(file_time - fs::file_time_type::clock::now() + system_clock::now());
    time_t cftime = system_clock::to_time_t(sctp);

    ostringstream oss;
    oss << put_time(gmtime(&cftime), "%Y-%m-%dT%H:%M:%SZ");

    return oss.str();
}

size_t write_response(char *res_data, size_t size, size_t el_num, void *user_data) {
    string *response = static_cast<string *>(user_data);
    response->append(res_data, size * el_num);
    return size * el_num;
}

void save_creds(const std::string &url, const std::string &key) {
    json creds;
    creds["server"] = url;
    creds["secret"] = key;

    if (!AUTH_FILE_LOC.has_parent_path()) {
        fs::create_directories(AUTH_FILE_LOC.parent_path());
    }

    ofstream auth_file(AUTH_FILE_LOC);
    if (!auth_file.is_open()) {
        cerr << "Error saving creds to file." << endl;
        return;
    }

    auth_file << setw(2) << creds << endl;
    auth_file.close();
}

nlohmann::json ret_creds() {
    if (!fs::exists(AUTH_FILE_LOC)) {
        cerr << "No credentials file please login first.\n";
        return nullptr;
    }

    ifstream auth_file(AUTH_FILE_LOC);
    if (!auth_file.is_open()) {
        cerr << "Error reading from creds file.\n";
        return nullptr;
    }

    json creds;
    auth_file >> creds;

    return creds;
}
