#ifndef THREAD_UTILS
#define THREAD_UTILS

#include <curl/curl.h>

#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>
#include <thread>
#include <tuple>

class ThreadPool {
   private:
    std::queue<std::tuple<std::vector<std::filesystem::path>, CURL, std::string>> img_queue;
    std::vector<std::unique_ptr<std::thread>> workers;

    std::mutex queue_mutex;
    std::condition_variable queue_state;

    void worker();

   public:
    ThreadPool(const unsigned int size);
    ~ThreadPool();
    void add_job(const std::vector<std::filesystem::path>&, CURL*, const std::string&);
};

#endif
