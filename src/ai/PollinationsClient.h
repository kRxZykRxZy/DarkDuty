#pragma once
#include <string>

class PollinationsClient {
public:
    // Blocking GET to https://text.pollinations.ai/{url_encoded_prompt}
    // Returns response body text, or "" on error.
    static std::string query(const std::string& prompt);

    // Percent-encode a string for use in a URL path segment
    static std::string urlEncode(const std::string& s);

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};
