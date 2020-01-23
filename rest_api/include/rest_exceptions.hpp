#ifndef _REST_EXCEPTIONS_HPP_
#define _REST_EXCEPTIONS_HPP_


#include <exception>
#include <sstream>


namespace rest_api {

class http_exception : public std::exception
{
public:
    http_exception(long httpcode, const std::string& response) 
        : m_httpcode(httpcode)
        , m_response(response)
    {
        std::ostringstream oss;
        oss << "[" << m_httpcode << "] " << m_response;
        m_what = oss.str();
    }
    
    const char* what() const throw()
    {
    	return m_what.c_str();
    }

    long http_code() const { return m_httpcode; }
    const std::string& http_response() const { return m_response; }

private:
    long m_httpcode;
    std::string m_response;
    std::string m_what;
};

class protobuf_exception : public std::exception
{
public:
    protobuf_exception(const std::string& message, const std::string& response) : m_what(message), m_response(response) {}
    
    const char* what() const throw()
    {
    	return m_what.c_str();
    }

    const std::string& response() { return m_response; }

private:
    std::string m_what;
    std::string m_response;
};

}

#endif