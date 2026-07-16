#pragma once

#include <string>

class SystemInfo
{
public:
    void Load();

    const std::string& GetPCName() const;
    const std::string& GetCurrentUser() const;

private:
    std::string computerName;
    std::string userName;
};