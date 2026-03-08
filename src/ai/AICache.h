#pragma once
#include <string>
#include <map>
#include <mutex>

class AICache {
public:
    bool has(const std::string& prompt) {
        std::lock_guard<std::mutex> lock(mtx_);
        return cache_.count(prompt) > 0;
    }

    std::string get(const std::string& prompt) {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = cache_.find(prompt);
        if (it != cache_.end()) return it->second;
        return "";
    }

    void set(const std::string& prompt, const std::string& response) {
        std::lock_guard<std::mutex> lock(mtx_);
        cache_[prompt] = response;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx_);
        cache_.clear();
    }

private:
    std::map<std::string, std::string> cache_;
    std::mutex mtx_;
};
