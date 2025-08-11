#include "cmd_utils.h"

#include <stdio.h>

void help(char* argv[]) {
    const char* const opt_help =
        "Usage: immich [cmd] [options]\n\n"
        "Options:\n"
        "  --url  - url of your immich server"
        "  --key"
        "Commands:\n"
        "  help   - prints this page\n"
        "  login  - logs into your specifie immich server\n"
        "  upload - recursively searches and uploads all images and videos under "
        "the current directory\n";
    printf("%s", opt_help);
}

void login(char* argv[]) { printf("login cmd\n"); }
