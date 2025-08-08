#include <stdio.h>
#include <string.h>

struct cmd {
    void (*run)();
};

void help();

#define HELP 0
static const struct cmd cmd_table[] = {
    [HELP] = {help},
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    if (strcmp("help", argv[1]) == 0) {
        struct cmd help_cmd = cmd_table[HELP];
        help_cmd.run();
    }

    return 0;
}

void help() { printf("options list\n"); }
