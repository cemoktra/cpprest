#ifndef _TADO_API_
#define _TADO_API_


#include <exception>
#include "request_resolver.hpp"
#include "post_request.hpp"
#include "get_request.hpp"

#include "tado.pb.h"
#include "tado_exceptions.hpp"


namespace tado {

class api
{
public:
    api(const api& rhs);
    ~api() = default;

    static api login(const OAUTHTOKEN& token);
    static api login(const std::string& username, const std::string& password);

    std::future<ME> fetch_me();
    std::future<HOME> fetch_home(int homeid);
    std::future<ZONES> fetch_zones(int homeid);
    std::future<ZONE_STATE> fetch_zone_state(int homeid, int zoneid);

private:
    api() = default;

    enum fetch_status {
        no_error,
        access_token_expired,
    };

    fetch_status refresh_token();
    std::pair<fetch_status, std::string> execute_request(rest_api::request *req);
    fetch_status get_status(const std::string& status);
    void add_auth_header(rest_api::request *req, const std::string& access_token);
    void handle_error(fetch_status status);

    rest_api::request_resolver m_resolver;
    OAUTHTOKEN m_auth;
};

}

#endif