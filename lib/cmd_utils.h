#ifndef CMD_UTILS_H
#define CMD_UTILS_H

void help();
void login();
void upload();

struct cmd {
    char *str;
    void (*run)();
};

enum cmd_opts { HELP = 0, LOGIN, UPLOAD };
static const struct cmd cmd_table[] = {
    [HELP] = {"help", help}, [LOGIN] = {"login", login}, [UPLOAD] = {"upload", upload}};

#endif
