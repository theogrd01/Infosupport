#pragma once

#include <string>

class HardwareInfo
{
public:
    void Load();

    const std::string& GetManufacturer() const;
    const std::string& GetModel() const;
    const std::string& GetSerialNumber() const;

private:
    std::string manufacturer;
    std::string model;
    std::string serialNumber;
};