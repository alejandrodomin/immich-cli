#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <curl/curl.h>

#include <filesystem>

void upload();

/**
 * @brief upload the binary of a file to the immich server
 * @param curl initialized curl instance
 * @param file to upload
 */
void upload_file(CURL* curl, const std::filesystem::path& file);

#endif
