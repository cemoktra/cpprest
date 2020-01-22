#ifndef _REQUEST_RESOLVER_HPP_
#define _REQUEST_RESOLVER_HPP_


#include <future>
#include "request.hpp"


namespace rest_api {

class request_resolver
{
public:
    request_resolver()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    ~request_resolver()
    {
        curl_global_cleanup();   
    }

    std::future<std::string> resolve(const request* _req)
    {
        return std::async(std::launch::async, [this, _req]() {
            std::string response;
            
            auto curl = curl_easy_init();    
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, request_resolver::string_callback);
                _req->to_CURL_opt(curl);
                
                // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
            }
            return response;
        });
    }



private:
    static size_t string_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        auto response = static_cast<std::string*>(userdata);
        *response = ptr;
        return size * nmemb;
    }
};

}

#endif