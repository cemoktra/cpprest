#ifndef _GITHUB_API_
#define _GITHUB_API_


#include <exception>
#include "request_resolver.hpp"
#include "post_request.hpp"
#include "rest_api.hpp"
#include "get_request.hpp"
#include "github.pb.h"


namespace github {

class api : public rest_api::api_base
{
public:
    api(const api& rhs) = default;
    ~api() = default;

    static api login(const std::string& token);

    std::future<AUTHENTICATED_USER> authenticated_user();

private:
    api() = default;
};

}

#endif