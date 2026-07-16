#include "system/MemoryInfo.h"

#include <windows.h>


void MemoryInfo::Load()
{
    MEMORYSTATUSEX status;

    status.dwLength = sizeof(status);


    if (GlobalMemoryStatusEx(&status))
    {
        unsigned long long totalRAM =
            status.ullTotalPhys / (1024 * 1024 * 1024);


        unsigned long long availableRAM =
            status.ullAvailPhys / (1024 * 1024 * 1024);


        memory =
            std::to_string(availableRAM)
            + " Go disponibles / "
            + std::to_string(totalRAM)
            + " Go";
    }
    else
    {
        memory = "Inconnu";
    }
}


const std::string& MemoryInfo::GetMemory() const
{
    return memory;
}