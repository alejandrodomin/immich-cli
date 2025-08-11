#include "curl_utils.h"

#include <stdlib.h>
#include <string.h>

size_t stream_res(char *res_data, size_t size, size_t el_num, void *user_data) {
    char **res_str = (char **)user_data;
    size_t len = 0;
    if (*res_str != NULL || *res_str != nullptr) {
        len = strlen(*res_str);
    }

    *res_str = realloc(*res_str, len + (size * el_num) + 1);
    if (!*res_str) {
        return 0;
    }

    memcpy(*res_str + len, res_data, size * el_num);
    (*res_str)[len + (size * el_num)] = '\0';

    return size * el_num;
}
