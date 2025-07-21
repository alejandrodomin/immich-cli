#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "../lib/login.hpp"
#include "../lib/upload.hpp"

using namespace std;
using namespace CLI;

int main(int argc, char **argv) {
    App app{"Command line tool for immich"};

    string url, key;
    app.add_option("url", url, "Immich server url")->required();
    app.add_option("key", key, "Immich API key")->required();

    App *login_cmd = app.add_subcommand("login", "Login using API key");
    App *upload_cmd = app.add_subcommand("upload", "Upload assets");

    login_cmd->callback([&]() { login(url, key); });
    upload_cmd->callback([&]() { upload(url, key); });

    CLI11_PARSE(app, argc, argv);

    return 0;
}
