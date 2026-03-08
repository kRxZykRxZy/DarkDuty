#pragma once
#include "AICache.h"
#include "PollinationsClient.h"
#include <string>
#include <functional>
#include <future>
#include <vector>
#include <mutex>

class AsyncAIRequest {
public:
    void submit(const std::string& prompt, std::function<void(const std::string&)> callback) {
        // Check cache first
        if (cache_.has(prompt)) {
            callback(cache_.get(prompt));
            return;
        }

        std::lock_guard<std::mutex> lock(mtx_);
        Task t;
        t.prompt = prompt;
        t.cb     = std::move(callback);
        t.fut    = std::async(std::launch::async, PollinationsClient::query, prompt);
        tasks_.push_back(std::move(t));
    }

    void update() {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto it = tasks_.begin(); it != tasks_.end(); ) {
            if (it->fut.valid() &&
                it->fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            {
                std::string result = it->fut.get();
                cache_.set(it->prompt, result);
                it->cb(result);
                it = tasks_.erase(it);
            } else {
                ++it;
            }
        }
    }

    void cancelAll() {
        std::lock_guard<std::mutex> lock(mtx_);
        tasks_.clear();
    }

    bool hasPending() {
        std::lock_guard<std::mutex> lock(mtx_);
        return !tasks_.empty();
    }

private:
    struct Task {
        std::string prompt;
        std::function<void(const std::string&)> cb;
        std::future<std::string> fut;
    };

    std::vector<Task> tasks_;
    std::mutex        mtx_;
    AICache           cache_;
};
