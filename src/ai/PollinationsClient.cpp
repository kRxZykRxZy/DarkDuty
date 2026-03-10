#include "PollinationsClient.h"

#include <curl/curl.h>
#include <cstdio>
#include <cctype>
#include <sstream>
#include <iomanip>

size_t PollinationsClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t total = size * nmemb;
    std::string* output = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), total);
    return total;
}

std::string PollinationsClient::urlEncode(const std::string& s)
{
    std::ostringstream encoded;
    encoded << std::hex << std::uppercase;

    for (unsigned char c : s)
    {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded << c;
        }
        else
        {
            encoded << '%' << std::setw(2) << std::setfill('0') << (int)c;
        }
    }

    return encoded.str();
}

std::string PollinationsClient::query(const std::string& prompt)
{
    std::string response;

    static bool curlInitialized = false;
    if (!curlInitialized)
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curlInitialized = true;
    }

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        std::fprintf(stderr, "[AI] Failed to init CURL\n");
        return "";
    }

    std::string encodedPrompt = urlEncode(prompt);
    std::string url = "https://text.pollinations.ai/" + encodedPrompt;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Accept: text/plain");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "DarkDuty/1.0");

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::fprintf(stderr, "[AI] curl error: %s\n", curl_easy_strerror(res));
        response.clear();
    }
    else
    {
        long httpCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

        if (httpCode != 200)
        {
            std::fprintf(stderr, "[AI] HTTP error code: %ld\n", httpCode);
            response.clear();
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}
