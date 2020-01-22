#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_


#include <curl/curl.h>
#include <string_view>
#include <map>



namespace rest_api {

class request {
public:
    request(const std::string& url)
        : m_url(url)
    {
    }

    std::string_view url() { return m_url; }
    

    void set_header(const std::string& name, const std::string& data)
    {
        m_header[name] = data;
    }

    virtual bool to_CURL_opt(CURL *curl) const
    {
        bool success = CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());

        struct curl_slist *chunk = nullptr;
        for (auto header : m_header)
            chunk = curl_slist_append(chunk, (header.first + ":" + header.second).c_str());
        return success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

private:
    std::string m_url;
    std::map<std::string, std::string> m_header;
};

}

#endif