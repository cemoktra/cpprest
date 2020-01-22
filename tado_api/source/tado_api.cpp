#include <tado_api.hpp>
#include <sstream>
#include <google/protobuf/util/json_util.h>

namespace tado {

api::api(const api& rhs)
{
    m_auth = rhs.m_auth;
}

api api::login(const OAUTHTOKEN& token) {
    api _a;
    _a.m_auth = token;
    return std::move(_a);
}

api api::login(const std::string& username, const std::string& password) {
    rest_api::post_request login_request("https://auth.tado.com/oauth/token");
    login_request.add_post_data("client_id", "tado-web-app");
    login_request.add_post_data("client_secret", "wZaRN7rpjn3FoNyF5IFuxg9uMzYJcvOoQ8QWiIqS3hfk6gLhVlG57j5YNoZL2Rtc");
    login_request.add_post_data("grant_type", "password");
    login_request.add_post_data("scope", "home.user");
    login_request.add_post_data("username", username);
    login_request.add_post_data("password", password);

    std::string response;
    {
        rest_api::request_resolver resolver;
        auto future_response = resolver.resolve(&login_request);
        future_response.wait();
        response = future_response.get();
    }

    OAUTHTOKEN token;
    if (google::protobuf::util::JsonStringToMessage(response, &token).ok())
        return std::move(api::login(token));        
    else {
        LOGINERROR error;
        if (google::protobuf::util::JsonStringToMessage(response, &error).ok())
            throw(login_exception(error.error(), error.error_description()));
        else
            throw(std::exception());
    }
}

api::fetch_status api::refresh_token()
{
    rest_api::post_request login_request("https://auth.tado.com/oauth/token");
    login_request.add_post_data("client_id", "tado-web-app");
    login_request.add_post_data("client_secret", "wZaRN7rpjn3FoNyF5IFuxg9uMzYJcvOoQ8QWiIqS3hfk6gLhVlG57j5YNoZL2Rtc");
    login_request.add_post_data("grant_type", "refresh_token");
    login_request.add_post_data("scope", "home.user");
    login_request.add_post_data("refresh_token", m_auth.refresh_token());
    auto [status, response] = execute_request(&login_request);

    OAUTHTOKEN refreshed_token;
    if (google::protobuf::util::JsonStringToMessage(response, &refreshed_token).ok())
        m_auth = refreshed_token;
    return status;
}

std::future<ME> api::fetch_me()
{
    return std::async(std::launch::async, [this]() {
        rest_api::get_request me_request("https://my.tado.com/api/v2/me");
        auto [status, response] = execute_request(&me_request);

        ME result;
        if (status == no_error) {
            if (!google::protobuf::util::JsonStringToMessage(response, &result).ok())
                throw(std::exception());
        } else
            handle_error(status);
        return result;
    });
}

std::future<HOME> api::fetch_home(int homeid)
{
    return std::async(std::launch::async, [this, homeid]() {
        std::ostringstream oss;
        oss << "https://my.tado.com/api/v2/homes/" << homeid;
        rest_api::get_request me_request(oss.str());
        auto [status, response] = execute_request(&me_request);

        HOME result;
        if (status == no_error) {
            if (!google::protobuf::util::JsonStringToMessage(response, &result).ok())
                throw(std::exception());
        } else
            handle_error(status);
        return result;
    });
}

std::future<ZONES> api::fetch_zones(int homeid)
{
    return std::async(std::launch::async, [this, homeid]() {
        std::ostringstream oss;
        oss << "https://my.tado.com/api/v2/homes/" << homeid << "/zones";
        rest_api::get_request me_request(oss.str());
        auto [status, response] = execute_request(&me_request);

        // handle unnamed array
        response = "{ \"zones\": " + response + "}";

        ZONES result;
        if (status == no_error) {
            if (!google::protobuf::util::JsonStringToMessage(response, &result).ok())
                throw(std::exception());
        } else
            handle_error(status);
        return result;
    });    
}

std::future<ZONE_STATE> api::fetch_zone_state(int homeid, int zoneid)
{
    return std::async(std::launch::async, [this, homeid, zoneid]() {
        std::ostringstream oss;
        oss << "https://my.tado.com/api/v2/homes/" << homeid << "/zones/" << zoneid << "/state";
        rest_api::get_request me_request(oss.str());
        auto [status, response] = execute_request(&me_request);
        ZONE_STATE result;
        if (status == no_error) {
            auto proto_status = google::protobuf::util::JsonStringToMessage(response, &result);
            if (!google::protobuf::util::JsonStringToMessage(response, &result).ok())
                throw(std::exception());
        } else
            handle_error(status);
        return result;
    }); 
}


std::pair<api::fetch_status, std::string> api::execute_request(rest_api::request *req)
{
    add_auth_header(req, m_auth.access_token());
    auto future_response = m_resolver.resolve(req);
    future_response.wait();
    auto response_json = future_response.get();    
    auto status = get_status(response_json);

    switch (status)
    {
    case access_token_expired:
        {
            auto refresh_status = refresh_token();
            if (refresh_status == no_error)
                return execute_request(req);
            else
                handle_error(refresh_status);
        }
        break;
    case no_error:
        break;
    default:
        break;
    }
    return std::make_pair<>(status, response_json);
}

void api::handle_error(api::fetch_status status)
{
    switch (status)
    {
    case no_error:
        break;
    case access_token_expired:
        throw(auth_token_expired()); break;
    default:
        throw(std::exception()); break;
    }
}

api::fetch_status api::get_status(const std::string& status)
{
    REQUEST_ERRORS errors;
    if (google::protobuf::util::JsonStringToMessage(status, &errors).ok()) {
        if (errors.errors(0).code() == "unauthorized" && errors.errors(0).title().find("Access token expired") != std::string::npos)
            return access_token_expired;         
    }
    return no_error;
}

void api::add_auth_header(rest_api::request *req, const std::string& access_token)
{
    req->set_header("Authorization", "Bearer " + access_token);
}

}