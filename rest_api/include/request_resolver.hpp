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

    std::future<std::pair<long, std::string>> resolve(const std::shared_ptr<request> req)
    {
        return std::async(std::launch::async, [this, req]() {
            std::string response;
            long http_status;
            
            auto curl = curl_easy_init();    
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, request_resolver::string_callback);
                req->to_CURL_opt(curl);
                
                // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                curl_easy_perform(curl);
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status);
                curl_easy_cleanup(curl);
            }
            return std::make_pair<>(http_status, response);
        });
    }



private:
    static size_t string_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        auto response = static_cast<std::string*>(userdata);
        response->append(ptr);
        return size * nmemb;
    }
};

}

#endif