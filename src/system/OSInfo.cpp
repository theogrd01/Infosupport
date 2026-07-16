#include "system/OSInfo.h"

#include <windows.h>

void OSInfo::Load()
{
    HKEY hKey;

    char productName[256] = {};
    char displayVersion[256] = {};
    char currentBuild[256] = {};

    DWORD sizeProduct = sizeof(productName);
    DWORD sizeVersion = sizeof(displayVersion);
    DWORD sizeBuild = sizeof(currentBuild);

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
            0,
            KEY_READ,
            &hKey) == ERROR_SUCCESS)
    {
        RegQueryValueExA(
            hKey,
            "ProductName",
            nullptr,
            nullptr,
            (LPBYTE)productName,
            &sizeProduct);

        RegQueryValueExA(
            hKey,
            "DisplayVersion",
            nullptr,
            nullptr,
            (LPBYTE)displayVersion,
            &sizeVersion);

        RegQueryValueExA(
            hKey,
            "CurrentBuild",
            nullptr,
            nullptr,
            (LPBYTE)currentBuild,
            &sizeBuild);

        RegCloseKey(hKey);

        osName =
            std::string(productName) +
            " - Version " +
            std::string(displayVersion) +
            " (Build " +
            std::string(currentBuild) +
            ")";
    }
    else
    {
        osName = "Inconnu";
    }
}

const std::string& OSInfo::GetOSName() const
{
    return osName;
}