#include <github_api.hpp>
#include <sstream>
#include <google/protobuf/util/json_util.h>

namespace github {

api api::login(const std::string& token) {
    api _api;
    _api.set_user_agent("cpp_rest_api");
    _api.set_auth_header("token " + token);
    return std::move(_api);
}

std::future<AUTHENTICATED_USER> api::authenticated_user()
{
    return get<AUTHENTICATED_USER>("https://api.github.com/user");
}

}