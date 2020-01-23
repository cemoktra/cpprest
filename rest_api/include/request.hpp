#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_


#include <curl/curl.h>
#include <string_view>
#include <map>



namespace rest_api {

class request {
    friend class request_resolver;

public:
    request(const std::string& url)
        : m_url(url)
    {
    }

    std::string_view url() { return m_url; }
    
    const std::string& response_link() 
    { 
        return m_responseLink;
    }

    void set_header(const std::string& name, const std::string& data)
    {
        m_header.insert_or_assign(name, data);
    }

    void set_user_agent(const std::string& userAgent)
    {
        m_userAgent = userAgent;
    }

    virtual bool to_CURL_opt(CURL *curl) const
    {
        bool success = CURLE_OK == curl_easy_setopt(curl, CURLOPT_URL, m_url.c_str());
        success = success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_USERAGENT, m_userAgent.c_str());
        struct curl_slist *chunk = nullptr;
        for (auto header : m_header)
            chunk = curl_slist_append(chunk, (header.first + ":" + header.second).c_str());
        return success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

private:
    std::string m_url;
    std::string m_userAgent;
    std::string m_responseLink;
    std::map<std::string, std::string> m_header;
};

}

#endif