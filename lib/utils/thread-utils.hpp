#ifndef THREAD_UTILS
#define THREAD_UTILS

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
   private:
    std::queue<std::function<void>> pool;
    std::vector<std::unique_ptr<std::thread>> workers;

    std::mutex pool_mutex;
    std::condition_variable pool_state;

    void worker();

   public:
    ThreadPool(const unsigned int size);
    ~ThreadPool();
    void add_job(std::function<void> job);
};

#endif
