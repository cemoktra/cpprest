#ifndef _POST_REQUEST_HPP_
#define _POST_REQUEST_HPP_


#include "request.hpp"


namespace rest_api {

class post_request : public request
{
public:
    post_request(const std::string& url)
        : request(url)
    {
    }

    void add_post_data(const std::string& name, const std::string& value)
    {
        if (m_post_data.length())
            m_post_data += "&";
        m_post_data += name + "=" + value;
    }

    virtual bool to_CURL_opt(CURL *curl) const override
    {
        bool success = request::to_CURL_opt(curl);
        success = success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_HTTPPOST, 0);
        if (m_post_data.length())
            success = success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_post_data.c_str());
        return success;
    }

private:
    std::string m_post_data;

};

}

#endif