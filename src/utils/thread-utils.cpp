#include "thread-utils.hpp"

#include <memory>

using namespace std;

void ThreadPool::worker() {
    unique_lock<mutex> lock(pool_mutex);
    pool_state.wait(lock, [this] { return pool.empty(); });

    auto& job = pool.front();
    job;
    pool.pop();
}

ThreadPool::ThreadPool(const unsigned int size) {
    for (int i = 0; i < size; i++) {
        auto unq_worker = make_unique<thread>(thread(&ThreadPool::worker));
        workers.push_back(unq_worker);
    }
}

ThreadPool::~ThreadPool() {
    for (auto& worker : workers) {
        if (worker->joinable()) worker->join();
    }
}
