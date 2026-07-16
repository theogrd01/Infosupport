#include "hardware/DiskInfo.h"

#include <Windows.h>
#include <sstream>
#include <iomanip>


void DiskInfo::Load()
{
    ULARGE_INTEGER freeBytes;
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER totalFreeBytes;


    if (GetDiskFreeSpaceExW(
            L"C:\\",
            &freeBytes,
            &totalBytes,
            &totalFreeBytes))
    {
        double totalGB =
            totalBytes.QuadPart /
            (1024.0 * 1024 * 1024);

        double freeGB =
            totalFreeBytes.QuadPart /
            (1024.0 * 1024 * 1024);

        double usedGB =
            totalGB - freeGB;


        double percent =
            (usedGB / totalGB) * 100;


        std::ostringstream output;

        output << std::fixed
               << std::setprecision(1);

        output
            << usedGB << " Go utilises / "
            << totalGB << " Go ("
            << percent
            << "%)";


        m_diskUsage = output.str();
    }
    else
    {
        m_diskUsage = "Impossible de lire le disque C:";
    }
}


std::string DiskInfo::GetDiskUsage() const
{
    return m_diskUsage;
}