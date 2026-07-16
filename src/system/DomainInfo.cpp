#include "system/DomainInfo.h"

#include <Windows.h>
#include <Lm.h>

#pragma comment(lib, "Netapi32.lib")


void DomainInfo::Load()
{
    LPWSTR domain = nullptr;

    NETSETUP_JOIN_STATUS status;

    if (NetGetJoinInformation(
        nullptr,
        &domain,
        &status) == NERR_Success)
    {
        if (status == NetSetupDomainName)
        {
            int size = WideCharToMultiByte(
                CP_UTF8,
                0,
                domain,
                -1,
                nullptr,
                0,
                nullptr,
                nullptr
            );

            std::string result(size, '\0');

            WideCharToMultiByte(
                CP_UTF8,
                0,
                domain,
                -1,
                result.data(),
                size,
                nullptr,
                nullptr
            );

            m_domain = result;
        }
        else
        {
            m_domain = "Pas de domaine Active Directory";
        }

        NetApiBufferFree(domain);
    }
    else
    {
        m_domain = "Inconnu";
    }
}


std::string DomainInfo::GetDomain() const
{
    return m_domain;
}