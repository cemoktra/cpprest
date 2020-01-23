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
    auto auth_user_f = github.authenticated_user();
    auth_user_f.wait();
    auto auth_user = auth_user_f.get();

    return 0;
}