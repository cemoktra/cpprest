#ifndef _REST_API_HPP_
#define _REST_API_HPP_


#include "request_resolver.hpp"
#include "get_request.hpp"
#include "post_request.hpp"
#include "rest_exceptions.hpp"
#include <string>
#include <future>
#include <google/protobuf/util/json_util.h>


namespace rest_api {

class api_base {
public:
    api_base() : m_userAgent("cpprest") {}
    api_base(const api_base& rhs)
    {
        m_authHeader = rhs.m_authHeader;
        m_userAgent = rhs.m_userAgent;
    }
    ~api_base() = default;

protected:
    template<typename T>
    std::future<T> get(const std::string& url, const std::string& response_prefix = "", const std::string& response_suffix = "")
    {
        // create get request
        return get<T>(std::make_shared<get_request>(url), response_prefix, response_suffix);
    }

    template<typename T>
    std::future<T> get(std::shared_ptr<get_request> req, const std::string& response_prefix = "", const std::string& response_suffix = "")
    {
        return std::async(std::launch::async, [this, req, response_prefix, response_suffix]() {
            // execute request 
            auto response = execute_request(req);
            response = response_prefix + response + response_suffix;
            
            // try to parse string into message T
            T message;
            auto status = google::protobuf::util::JsonStringToMessage(response, &message);
            if (!status.ok())
                throw(protobuf_exception(status.message(), response));
            return message;
        });    
    }

    template<typename T>
    std::future<T> post(const std::string& url)
    {
        // create get request
        return post<T>(std::make_shared<post_request>(url));
    }

    template<typename T>
    std::future<T> post(std::shared_ptr<post_request> req)
    {
        return std::async(std::launch::async, [this, req]() {
            // execute request 
            auto response = execute_request(req);

            // try to parse string into message T
            T message;
            auto status = google::protobuf::util::JsonStringToMessage(response, &message);
            if (!status.ok())
                throw(protobuf_exception(status.message(), response));
            return message;
        });    
    }




    virtual std::string execute_request(std::shared_ptr<request> req)
    {
        req->set_header("Authorization", m_authHeader);
        req->set_user_agent(m_userAgent);
        auto response_f = m_resolver.resolve(req);
        response_f.wait();
        auto [httpstatus, response] = response_f.get();

        if (httpstatus == 200)
            return response;
        else
            throw(http_exception(httpstatus, response));
    }

    const std::string& auth_header() const { return m_authHeader; }
    const std::string& user_agent() const { return m_userAgent; }

    void set_auth_header(const std::string& authHeader)
    {
        m_authHeader = authHeader;
    }

    void set_user_agent(const std::string& userAgent)
    {
        m_userAgent = userAgent;
    }

private:
    request_resolver m_resolver;
    std::string m_authHeader;
    std::string m_userAgent;
};

}

#endif