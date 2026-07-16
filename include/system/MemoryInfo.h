#pragma once

#include <string>

class MemoryInfo
{
public:
    void Load();

    const std::string& GetMemory() const;

private:
    std::string memory;
};