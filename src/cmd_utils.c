#include "cmd_utils.h"

#include <stdio.h>

void help() {
    const char *const opt_help =
        "Usage: immich [cmd]\n\n"
        "Commands:\n"
        "  help   - prints this page\n"
        "  login  - logs into your specifie immich server\n"
        "  upload - recursively searches and uploads all images and videos under "
        "the current directory\n";
    printf("%s", opt_help);
}

void login() { printf("login cmd\n"); }
void upload() { printf("upload cmd\n"); }
