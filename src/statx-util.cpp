#include "../lib/statx-util.hpp"

#include <fcntl.h>
#include <linux/stat.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include <cstring>

namespace fs = std::filesystem;
using namespace std::chrono;

fs::file_time_type birth_time(const fs::path& file) {
    const char* c_path = file.c_str();
    struct statx stx;

    int ret = syscall(SYS_statx, AT_FDCWD, c_path, AT_STATX_SYNC_AS_STAT, STATX_BTIME, &stx);

    if (ret != 0) {
        STATX_COMPATIBLE = false;
        throw ret;
    }

    struct tm t;
    time_t bt = stx.stx_btime.tv_sec;
    localtime_r(&bt, &t);

    system_clock::time_point sys_tp = system_clock::from_time_t(bt);
    return fs::file_time_type::clock::now() + (sys_tp - std::chrono::system_clock::now());
}
