#include "system/Uptime.h"

#include <windows.h>

void Uptime::Load()
{
    unsigned long long milliseconds = GetTickCount64();

    unsigned long long seconds = milliseconds / 1000;

    unsigned long long days = seconds / 86400;
    seconds %= 86400;

    unsigned long long hours = seconds / 3600;
    seconds %= 3600;

    unsigned long long minutes = seconds / 60;

    uptime =
        std::to_string(days) + " jours " +
        std::to_string(hours) + " heures " +
        std::to_string(minutes) + " minutes";
}

const std::string& Uptime::GetUptime() const
{
    return uptime;
}