#ifndef CURL_UTILS_H
#define CURL_UTILS_H

#include <stdlib.h>

/**
 * Curl callback function that will append data to the end of a buffer. The user_data pointer should be a pointer to a
 * string aka a char**
 * @warning careful with passing the same string pointer when multi-threading, data will be overriden and race
 * conditions could occur
 *
 * @param res_data string data returned by the curl call
 * @param size size of each characters
 * @param el_num number of characters
 * @param user_data pointer to a user defined buffer
 *
 * @return size of the data written. If the size does not match size * el_num curl will consider the function to have
 * failed.
 */
size_t stream_res(char *res_data, size_t size, size_t el_num, void *user_data);

#endif
