#include <curl/curl.h>
#include <future>
#include <thread>
#include <iostream>
#include <argcpp17.h>

#include "tado_api.hpp"

int main(int argc, char **args)
{
    parser p;
    p.add_positional("username", "username")
     .add_positional("password", "password");
    p.parse(argc, args);
    auto username = p.get_value<std::string>({"username"}).value();
    auto password = p.get_value<std::string>({"password"}).value();

    tado::api tado = tado::api::login(username, password);

    auto me_future = tado.fetch_me();
    me_future.wait();
    auto me = me_future.get();

    for (auto homeid : me.homes())
    {
        auto home_future = tado.fetch_home(homeid.id());
        auto zones_future = tado.fetch_zones(homeid.id());

        home_future.wait();        
        auto home = home_future.get();
        std::cout << home.name() << ":" << std::endl;

        zones_future.wait();        
        auto zones = zones_future.get();        

       for (auto zone : zones.zones()) {
            auto zone_state_future = tado.fetch_zone_state(home.id(), zone.id());
            zone_state_future.wait();
            auto zone_state = zone_state_future.get();

            std::cout << "  " << zone.name() << " - " << zone_state.sensordatapoints().insidetemperature().celsius() << "°" << " - " << zone_state.sensordatapoints().humidity().percentage() << "%" << std::endl;
       }
    }

            
    return 0;
}