#ifndef _GITHUB_API_
#define _GITHUB_API_


#include <exception>
#include "request_resolver.hpp"
#include "post_request.hpp"
#include "rest_api.hpp"
#include "get_request.hpp"
#include "github.pb.h"


namespace github {


enum repo_type {
    _all,
    _owner,
    _public,
    _private,
    _member,
};

enum repo_sort {
    _created,
    _updated,
    _pushed,
    _full_name,
};

enum repo_sort_dir {
    _asc,
    _desc,
};

static std::map<repo_type, std::string> REPO_TYPE_MAP = {
    {_all, "all"},
    {_owner, "owner"},
    {_public, "public"},
    {_private, "private"},
    {_member, "member"},
};

static std::map<repo_sort, std::string> REPO_SORT_MAP = {
    {_created, "created"},
    {_updated, "updated"},
    {_pushed, "pushed"},
    {_full_name, "full_name"},
};

static std::map<repo_sort_dir, std::string> REPO_SORT_DIR_MAP = {
    {_asc, "asc"},
    {_desc, "desc"},
};



class api : public rest_api::api_base
{
public:
    api(const api& rhs) = default;
    ~api() = default;

    static std::shared_ptr<api> login(const std::string& token);

    std::future<GITHUB_USER> authenticated_user();
    std::future<GITHUB_USER> user(const std::string& username);
    std::future<REPOSITORIES> authenticated_user_repos(repo_type type = _all, repo_sort sort = _full_name, repo_sort_dir sort_dir = _asc);
    std::future<REPOSITORIES> user_repos(const std::string& username, repo_type type = _all, repo_sort sort = _full_name, repo_sort_dir sort_dir = _asc);
    std::future<std::pair<REPOSITORIES, std::string>> repos(std::optional<std::string> since = std::nullopt);

private:
    api() = default;
};

}

#endif