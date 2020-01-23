#include <github_api.hpp>
#include <sstream>
#include <google/protobuf/util/json_util.h>

namespace github {

api api::login(const std::string& token) {
    api _api;
    _api.set_user_agent("cpprest_github_api");
    _api.set_auth_header("token " + token);
    return std::move(_api);
}

std::future<GITHUB_USER> api::authenticated_user()
{
    return get<GITHUB_USER>("https://api.github.com/user");
}

std::future<GITHUB_USER> api::user(const std::string& username)
{
    return get<GITHUB_USER>("https://api.github.com/users/" + username);
}

std::future<REPOSITORIES> api::authenticated_user_repos(repo_type type, repo_sort sort, repo_sort_dir sort_dir)
{
    std::ostringstream oss;
    oss << "https://api.github.com/user/repos?type=" << REPO_TYPE_MAP[type] << "&sort=" << REPO_SORT_MAP[sort] << "&direction=" << REPO_SORT_DIR_MAP[sort_dir];
    return get<REPOSITORIES>(oss.str(), "{ \"repositories\": ", "}");
}

std::future<REPOSITORIES> api::user_repos(const std::string& username, repo_type type, repo_sort sort, repo_sort_dir sort_dir)
{
    std::ostringstream oss;
    oss << "https://api.github.com/users/" << username << "/repos?type=" << REPO_TYPE_MAP[type] << "&sort=" << REPO_SORT_MAP[sort] << "&direction=" << REPO_SORT_DIR_MAP[sort_dir];
    return get<REPOSITORIES>(oss.str(), "{ \"repositories\": ", "}");
}

std::future<std::pair<REPOSITORIES, std::string>> api::repos(std::optional<std::string> since)
{
    std::ostringstream oss;
    oss << "https://api.github.com/repositories";
    if (since.has_value())
        oss << "?since=" << since.value();
    std::string url = oss.str();

    return std::async(std::launch::async, [this, url]() {
        auto req = std::make_shared<rest_api::get_request>(url);
        auto repos_f = get<REPOSITORIES>(req, "{ \"repositories\": ", "}");
        repos_f.wait();
        auto link = req->response_link();
        auto next = link.substr(link.find_first_of('=') + 1);
        return std::make_pair(repos_f.get(), next);
    });
}

}