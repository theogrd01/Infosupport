#pragma once

#include <string>


class NetworkInfo
{
public:

    void Load();

    const std::string& GetIPAddress() const;
    const std::string& GetGateway() const;
    const std::string& GetDNS() const;

    bool IsDHCPEnabled() const;


private:

    std::string ipAddress;
    std::string gateway;
    std::string dns;

    bool dhcpEnabled = false;
};