#ifndef _TADO_DATA_
#define _TADO_DATA_


#include <vector>

namespace tado {

class oauth_token
{
public:
    oauth_token()
    {}

    oauth_token(const std::string& access_token, const std::string& refresh_token)
        : m_refresh_token(refresh_token)
        , m_access_token(access_token)
    {}

    oauth_token(const oauth_token& rhs)
    {
        *this = rhs;
    }

    oauth_token& operator=(const oauth_token& rhs)
    {
        m_access_token = rhs.access_token();
        m_refresh_token = rhs.refresh_token();
        return *this;
    }

    const std::string& access_token() const { return m_access_token; }
    const std::string& refresh_token() const { return m_refresh_token; }

private:
    std::string m_access_token;
    std::string m_refresh_token;
};

class me {
    friend class api;

public:
    me(const me&) = default;
    ~me() = default;

    const std::string& email() { return m_email; }
    const std::string& name() { return m_name; }
    const std::string& username() { return m_username; }
    const std::vector<int>& homes() { return m_homes; }

private:
    me() = default;

    std::string m_email;
    std::string m_name;
    std::string m_username;
    std::vector<int> m_homes;
};

class home {
    friend class api;

public:
    home(const home&) = default;
    ~home() = default;
    
    int id () { return m_id; }
    const std::string& name() { return m_name; }

private:
    home() = default;

    int m_id;
    std::string m_name;
};

class zone {
    friend class api;

public:
    zone(const zone&) = default;
    ~zone() = default;
    
    int id () { return m_id; }
    const std::string& name() { return m_name; }
    const std::string& type() { return m_type; }

private:
    zone() = default;

    int m_id;
    std::string m_name;
    std::string m_type;
};

class zone_state {
    friend class api;

public:
    zone_state(const zone_state&) = default;
    ~zone_state() = default;

    std::optional<bool> isWindowOpen() { return m_openWindowDetected; }
    std::optional<double> humidity() { return m_humidity; }
    std::optional<double> insideTemperature() { return m_insideTemperature; }
    const std::string& tadoMode() { return m_tadoMode; }

private:
    zone_state() = default;

    std::optional<bool> m_openWindowDetected;
    std::optional<double> m_humidity;
    std::optional<double> m_insideTemperature;
    std::string m_tadoMode;

};



}

#endif