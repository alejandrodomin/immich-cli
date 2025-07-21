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
 * @brief Logs out of immich server by deleting the saved creds at AUTH_FILE_LOC
 */
void logout();

#endif
