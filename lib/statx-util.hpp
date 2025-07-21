#ifndef STATXUTIL_HPP
#define STATXUTIL_HPP

#ifndef AT_STATX_SYNC_AS_STAT
#define AT_STATX_SYNC_AS_STAT 0x0000
#endif

#include <filesystem>

inline bool STATX_COMPATIBLE = true;

std::filesystem::file_time_type birth_time(const std::filesystem::path& file);

#endif
