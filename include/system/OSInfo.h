#pragma once

#include <string>

class OSInfo
{
public:
    void Load();

    const std::string& GetOSName() const;

private:
    std::string osName;
};