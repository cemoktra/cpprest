#include <curl/curl.h>
#include <future>
#include <thread>
#include <iostream>
#include <argcpp17.h>

#include "github_api.hpp"

int main(int argc, char **args)
{
    std::string token;
    
    parser p;
    p.add_mandatory_argument({"token","t"}, "github token");

    try {
        p.parse(argc, args);
        token = p.get_value<std::string>({"token"}).value();
    } catch (...) {
        p.usage("github.exe");
        return -1;
    }

    auto github = github::api::login(token);

    auto user_f = github.authenticated_user();
    auto user_repos_f = github.authenticated_user_repos();
    user_f.wait();
    user_repos_f.wait();

    auto user = user_f.get();
    auto user_repos = user_repos_f.get();
    for (auto repo : user_repos.repositories())
        std::cout << repo.full_name() << std::endl;

    return 0;
}