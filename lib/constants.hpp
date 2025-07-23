#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <filesystem>
#include <vector>

extern std::filesystem::path AUTH_FILE_LOC;

inline const std::string TOKEN_VALIDATION_PATH = "/api/auth/validateToken";
inline const std::string TOKEN_HEADER_NAME = "x-api-key";

inline const std::string ACCEPT_APP_JSON = "Accept: application/json";

/**
 * @brief: Immich supported types.
 * https://immich.app/docs/features/supported-formats/
 */
inline const std::vector<std::string> SUPPORTED_FILE_TYPES = {".avif", ".bmp", ".git",  ".heic", ".jp2",  ".webp",
                                                              ".jpg",  ".jpe", ".insp", ".jxl",  ".png",  ".psd",
                                                              ".raw",  ".rw2", ".svg",  ".tif",  ".tiff", ".webp"};

#endif
