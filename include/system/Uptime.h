#pragma once

#include <string>

class Uptime
{
public:
    void Load();

    const std::string& GetUptime() const;

private:
    std::string uptime;
};