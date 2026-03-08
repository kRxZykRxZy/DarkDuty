#include "PollinationsClient.h"
#include <curl/curl.h>
#include <cstdio>
#include <cctype>
#include <sstream>
#include <iomanip>

size_t PollinationsClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total = size * nmemb;
    output->append(static_cast<char*>(contents), total);
    return total;
}

std::string PollinationsClient::urlEncode(const std::string& s) {
    std::ostringstream encoded;
    encoded << std::hex << std::uppercase;
    for (unsigned char c : s) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::setw(2) << std::setfill('0') << (int)c;
        }
    }
    return encoded.str();
}

std::string PollinationsClient::query(const std::string& prompt) {
    std::string response;
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    std::string encoded = urlEncode(prompt);
    std::string url = "https://text.pollinations.ai/" + encoded;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "DarkDuty/1.0");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::fprintf(stderr, "[AI] curl error: %s\n", curl_easy_strerror(res));
        response = "";
    }

    curl_easy_cleanup(curl);
    return response;
}
