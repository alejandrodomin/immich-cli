#include "thread-utils.hpp"

#include <iostream>
#include <memory>

using namespace std;

void ThreadPool::worker() {
    unique_lock<mutex> lock(queue_mutex);
    queue_state.wait(lock, [this] { return !img_queue.empty(); });

    auto& data = img_queue.front();

    vector<std::filesystem::path>& imgs = get<0>(data);
    CURL* curl = get<1>(data);
    string& key = get<2>(data);

    cout << "upload batch of " << imgs.size() << " image(s)\r" << flush;
    for (auto file : imgs) {
        curl_slist* h_list = headers(fs::hash_value(file), key);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, h_list);

        // TODO: add json check for success upload and duplicate upload.
        upload_file(curl, file);
        curl_slist_free_all(h_list);
    }

    curl_easy_cleanup(curl);

    img_queue.pop();
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
