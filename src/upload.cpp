#include "../lib/upload.hpp"

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void upload() {
    fs::path curr_dir = fs::current_path();

    cout << "All files under " << curr_dir << "\n";
    for (const auto& file : fs::recursive_directory_iterator(curr_dir)) {
        if (fs::file_type::regular == fs::status(file).type()) {
            cout << "\t" << file << "\n";
        }
    }
}
