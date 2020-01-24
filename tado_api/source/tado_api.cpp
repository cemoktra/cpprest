#include <tado_api.hpp>
#include <sstream>
#include <google/protobuf/util/json_util.h>

namespace tado {

api::api(const api& rhs) : rest_api::api_base(rhs)
{
    m_auth = rhs.m_auth;
}

std::shared_ptr<api> api::login(const OAUTHTOKEN& token) {
    auto _api = new api();
    _api->m_auth = token;
    _api->set_auth_header("Bearer " + _api->m_auth.access_token());
    return std::shared_ptr<api>(_api);
}

std::shared_ptr<api> api::login(const std::string& username, const std::string& password) {
    auto login_request = std::make_shared<rest_api::post_request> ("https://auth.tado.com/oauth/token");
    login_request->add_post_data("client_id", "tado-web-app");
    login_request->add_post_data("client_secret", "wZaRN7rpjn3FoNyF5IFuxg9uMzYJcvOoQ8QWiIqS3hfk6gLhVlG57j5YNoZL2Rtc");
    login_request->add_post_data("grant_type", "password");
    login_request->add_post_data("scope", "home.user");
    login_request->add_post_data("username", username);
    login_request->add_post_data("password", password);

    std::string response;
    {        
        rest_api::request_resolver resolver;
        auto future_response = resolver.resolve(login_request);
        response = future_response.get().second;
    }

    OAUTHTOKEN token;
    if (google::protobuf::util::JsonStringToMessage(response, &token).ok())
        return api::login(token);
    else {
        LOGINERROR error;
        if (google::protobuf::util::JsonStringToMessage(response, &error).ok())
            throw(login_exception(error.error(), error.error_description()));
        else
            throw(std::exception());
    }
}

void api::refresh_token()
{
    auto refresh_request = std::make_shared<rest_api::post_request> ("https://auth.tado.com/oauth/token");
    refresh_request->add_post_data("client_id", "tado-web-app");
    refresh_request->add_post_data("client_secret", "wZaRN7rpjn3FoNyF5IFuxg9uMzYJcvOoQ8QWiIqS3hfk6gLhVlG57j5YNoZL2Rtc");
    refresh_request->add_post_data("grant_type", "refresh_token");
    refresh_request->add_post_data("scope", "home.user");
    refresh_request->add_post_data("refresh_token", m_auth.refresh_token());

    auto refreshed_token_f = post<OAUTHTOKEN>(refresh_request);
    m_auth = refreshed_token_f.get();
    set_auth_header("Bearer " + m_auth.access_token());
}

std::future<ME> api::me()
{
    auto req = std::make_shared<rest_api::get_request>("https://my.tado.com/api/v2/me");
    return tado_get<ME>(req);
    // return get<ME>("https://my.tado.com/api/v2/me");
}

std::future<HOME> api::home(int homeid)
{
    std::ostringstream oss;
    oss << "https://my.tado.com/api/v2/homes/" << homeid;
    return get<HOME>(oss.str());
}

std::future<ZONES> api::zones(int homeid)
{
    std::ostringstream oss;
    oss << "https://my.tado.com/api/v2/homes/" << homeid << "/zones";
    return get<ZONES>(oss.str(), "{ \"zones\": ", "}");
}

std::future<ZONE_STATE> api::zone_state(int homeid, int zoneid)
{
    std::ostringstream oss;
    oss << "https://my.tado.com/api/v2/homes/" << homeid << "/zones/" << zoneid << "/state";
    return get<ZONE_STATE>(oss.str());
}

}