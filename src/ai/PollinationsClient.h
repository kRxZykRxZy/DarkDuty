#pragma once
#include <string>

class PollinationsClient
{
public:
    static std::string query(const std::string& prompt);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static std::string urlEncode(const std::string& s);
};
