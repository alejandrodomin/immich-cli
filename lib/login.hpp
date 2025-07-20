#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>

/**
 * @brief Entry point for the functionality of the login command.
 *
 * @param url Required url of the immich server.
 * @param key Required key for access to the immich server.
 */
void login(const std::string& url, const std::string& key);

/**
 * @brief When using curl we need a callback function to write the response into
 * memory.
 *
 * @param res_data Pointer to the response data curl recieved.
 * @param size byte size of each element in the response
 * @param el_num number of elements in the response. Total size of response is
 * size * el_num
 * @param user_data Pointer to the memory chunck provided by us in the
 * CURLOPT_WRITEDATA option
 *
 * @return The return size must match the recieved size, else curl will treat it
 * as an error.
 */
size_t write_response(char* res_data, size_t size, size_t el_num, void* user_data);

#endif
