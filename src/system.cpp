#include "System.h"

#include <windows.h>

void SystemInfo::Load()
{
    char buffer[256];
    DWORD size = sizeof(buffer);

    if (GetComputerNameA(buffer, &size))
    {
        computerName = buffer;
    }
    else
    {
        computerName = "Inconnu";
    }

    size = sizeof(buffer);

    if (GetUserNameA(buffer, &size))
    {
        userName = buffer;
    }
    else
    {
        userName = "Inconnu";
    }
}

const std::string& SystemInfo::GetPCName() const
{
    return computerName;
}

const std::string& SystemInfo::GetCurrentUser() const
{
    return userName;
}