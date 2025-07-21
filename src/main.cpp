#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "../lib/login.hpp"
#include "../lib/upload.hpp"

using namespace std;
using namespace CLI;

int main(int argc, char **argv) {
    App app{"Command line tool for immich"};

    App *login_cmd = app.add_subcommand("login", "Login using API key");
    App *logout_cmd = app.add_subcommand("logout", "Logout from immich server");
    App *upload_cmd = app.add_subcommand("upload", "Upload all assets, recursively, under the current directory");

    string url, key;
    login_cmd->add_option("url", url, "Immich server url")->required();
    login_cmd->add_option("key", key, "Immich API key")->required();

    login_cmd->callback([&]() { login(url, key); });
    logout_cmd->callback([]() { logout(); });
    upload_cmd->callback([]() { upload(); });

    CLI11_PARSE(app, argc, argv);

    return 0;
}
