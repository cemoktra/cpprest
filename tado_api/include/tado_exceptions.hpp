#ifndef _TADO_EXCEPTIONS_HPP_
#define _TADO_EXCEPTIONS_HPP_


#include <exception>
#include <string>


namespace tado {

class login_exception : public std::exception
{
public:
    login_exception(const std::string& error, const std::string& description) : m_error("[" + error + "] " + description) {}
    
    const char* what() const throw()
    {
    	return m_error.c_str();
    }

private:
    std::string m_error;
};

class auth_token_expired : public std::exception
{
public:
    const char* what() const throw()
    {
    	return "Access token expired";
    }
};

}

#endif