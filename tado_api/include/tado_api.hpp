#ifndef _TADO_API_
#define _TADO_API_


#include <exception>
#include "rest_api.hpp"
#include "post_request.hpp"

#include "tado.pb.h"
#include "tado_exceptions.hpp"

namespace tado {

class api : public rest_api::api_base
{
public:
    api(const api& rhs);
    ~api() = default;

    static std::shared_ptr<api> login(const OAUTHTOKEN& token);
    static std::shared_ptr<api> login(const std::string& username, const std::string& password);

    std::future<ME> me();
    std::future<HOME> home(int homeid);
    std::future<ZONES> zones(int homeid);
    std::future<ZONE_STATE> zone_state(int homeid, int zoneid);

    inline OAUTHTOKEN auth() const { return m_auth; }

private:
    template<typename T>
    std::future<T> tado_get(std::shared_ptr<rest_api::get_request> req)
    {
        return std::async(std::launch::async, [this, req]() {
            auto res_f = get<T>(req);
            try {
                return res_f.get();
            } catch (rest_api::http_exception http_ex) {
                if (http_ex.http_code() != 401)
                    throw;
                res_f = tado_refresh_and_get<T>(req);
                return res_f.get();
            }
        });
    }

    template<typename T>
    std::future<T> tado_refresh_and_get(std::shared_ptr<rest_api::get_request> req)
    {
        return std::async(std::launch::async, [this, req]() {
            refresh_token();
            auto res_f = get<T>(req);
            return res_f.get();
        });
    }

    void refresh_token();

    api() = default;
    OAUTHTOKEN m_auth;
};

}

#endif
