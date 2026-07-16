#pragma once

#include <string>


class DiskInfo
{

public:

    void Load();

    std::string GetDiskUsage() const;


private:

    std::string m_diskUsage;

};