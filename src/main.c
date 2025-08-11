#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

#include "cmd_utils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    if (argc > 2) printf("Only one command is accepted, all others will be ignored.\n");

    curl_global_init(CURL_GLOBAL_DEFAULT);

    unsigned int len = sizeof cmd_table / sizeof(struct cmd);
    for (int i = 0; i < len; i++) {
        struct cmd curr = cmd_table[i];

        if (strcmp(curr.str, argv[1]) == 0) curr.run(&argv[2]);
    }

    curl_global_cleanup();

    return 0;
}
