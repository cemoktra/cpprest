#ifndef _GET_REQUEST_HPP_
#define _GET_REQUEST_HPP_


#include "request.hpp"


namespace rest_api {

class get_request : public request
{
public:
    get_request(const std::string& url)
        : request(url)
    {
    }

    virtual bool to_CURL_opt(CURL *curl) const override
    {
        bool success = request::to_CURL_opt(curl);
        success = success && CURLE_OK == curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
        return success;
    }
};

}

#endif