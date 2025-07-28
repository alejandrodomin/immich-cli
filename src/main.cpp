#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <filesystem>

#include "login.hpp"
#include "upload.hpp"

using namespace std;
using namespace CLI;
namespace fs = std::filesystem;

void init_auth_loc(string &);

int main(int argc, char **argv) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    App app{"Command line tool for immich"};
    app.footer(
        "Examples:\n"
        "  ./immich-cli login http://localhost:2283 my_api_key\n"
        "  ./immich-cli --auth-loc=~/.config/immich/auth.json upload");

    string raw_loc;
    app.add_flag("-a,--auth-loc", raw_loc, "Sets location to check for auth file");
    app.parse_complete_callback([&raw_loc]() { init_auth_loc(raw_loc); });

    App *login_cmd = app.add_subcommand("login", "Login using API key");
    App *logout_cmd = app.add_subcommand("logout", "Logout from immich server");
    App *upload_cmd = app.add_subcommand("upload", "Upload all assets, recursively, under the current directory");

    string url, key;
    login_cmd->add_option("url", url, "Immich server url")->required();
    login_cmd->add_option("key", key, "Immich API key")->required();

    login_cmd->callback([&url, &key]() { login(url, key); });
    logout_cmd->callback([]() { logout(); });
    upload_cmd->callback([]() { upload(); });

    CLI11_PARSE(app, argc, argv);
    curl_global_cleanup();

    return 0;
}

fs::path AUTH_FILE_LOC;
void init_auth_loc(string &loc) {
    auto possible = fs::path(loc);
    if (fs::exists(possible)) {
        AUTH_FILE_LOC = possible;
    } else {
        AUTH_FILE_LOC = std::string(std::getenv("HOME")) + "/.config/immich/auth.json";
    }
}
