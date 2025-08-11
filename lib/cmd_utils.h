#ifndef CMD_UTILS_H
#define CMD_UTILS_H

/**
 * Prints the help text.
 */
void help(char *[]);

/**
 * Command will ask stdin for url and api-key.
 */
void login(char *[]);

/**
 * Recursively searches the current directory for all images and videos that immich supports.
 */
void upload(char *[]);

struct cmd {
    char *str;
    void (*run)(char *[]);
};

enum cmd_opts { HELP = 0, LOGIN, UPLOAD };
static const struct cmd cmd_table[] = {
    [HELP] = {"help", help}, [LOGIN] = {"login", login}, [UPLOAD] = {"upload", upload}};

#endif
