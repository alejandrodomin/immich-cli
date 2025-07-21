#ifndef CURLUTIL_HPP
#define CURLUTIL_HPP

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

/**
 * @brief utility function to convert iso time to string for immich
 */
std::string iso_time(std::filesystem::file_time_type file_time);

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

/**
 * @brief save credentials to AUTH_FILE_LOC for reading between commands
 * @param url of the immich server
 * @param key secret for immich server login
 */
void save_creds(const std::string& url, const std::string& key);

/**
 * @brief fetch the saved credentials from the auth file at AUTH_FILE_LOC
 */
nlohmann::json ret_creds();

#endif
