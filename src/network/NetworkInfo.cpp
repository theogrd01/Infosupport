#include "network/NetworkInfo.h"

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#include <windows.h>
#include <iphlpapi.h>

#include <string>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


void NetworkInfo::Load()
{
    ULONG size = 15000;

    IP_ADAPTER_ADDRESSES* adapters =
        (IP_ADAPTER_ADDRESSES*)malloc(size);


    DWORD result = GetAdaptersAddresses(
        AF_INET,
        GAA_FLAG_INCLUDE_GATEWAYS,
        nullptr,
        adapters,
        &size
    );


    if (result == ERROR_BUFFER_OVERFLOW)
    {
        free(adapters);

        adapters =
            (IP_ADAPTER_ADDRESSES*)malloc(size);


        result = GetAdaptersAddresses(
            AF_INET,
            GAA_FLAG_INCLUDE_GATEWAYS,
            nullptr,
            adapters,
            &size
        );
    }


    if (result == NO_ERROR)
    {
        IP_ADAPTER_ADDRESSES* adapter = adapters;


        while (adapter)
        {
            if (adapter->OperStatus == IfOperStatusUp)
            {

                // Adresse IP
                if (adapter->FirstUnicastAddress)
                {
                    char buffer[INET_ADDRSTRLEN];


                    sockaddr_in* addr =
                        (sockaddr_in*)
                        adapter->FirstUnicastAddress
                        ->Address.lpSockaddr;


                    inet_ntop(
                        AF_INET,
                        &addr->sin_addr,
                        buffer,
                        sizeof(buffer)
                    );


                    ipAddress = buffer;
                }



                // Passerelle
                if (adapter->FirstGatewayAddress)
                {
                    char buffer[INET_ADDRSTRLEN];


                    sockaddr_in* gatewayAddr =
                        (sockaddr_in*)
                        adapter->FirstGatewayAddress
                        ->Address.lpSockaddr;


                    inet_ntop(
                        AF_INET,
                        &gatewayAddr->sin_addr,
                        buffer,
                        sizeof(buffer)
                    );


                    gateway = buffer;
                }



                // DNS
                dns.clear();

                IP_ADAPTER_DNS_SERVER_ADDRESS* dnsServer =
                    adapter->FirstDnsServerAddress;


                while (dnsServer)
                {
                    char buffer[INET_ADDRSTRLEN];


                    sockaddr_in* dnsAddr =
                        (sockaddr_in*)
                        dnsServer->Address.lpSockaddr;


                    inet_ntop(
                        AF_INET,
                        &dnsAddr->sin_addr,
                        buffer,
                        sizeof(buffer)
                    );


                    if (!dns.empty())
                    {
                        dns += ", ";
                    }


                    dns += buffer;


                    dnsServer = dnsServer->Next;
                }



                // DHCP
                dhcpEnabled =
                    (adapter->Flags & IP_ADAPTER_DHCP_ENABLED);


                break;
            }


            adapter = adapter->Next;
        }
    }
    else
    {
        ipAddress = "Inconnue";
        gateway = "Inconnue";
        dns = "Inconnue";
        dhcpEnabled = false;
    }


    free(adapters);
}



const std::string& NetworkInfo::GetIPAddress() const
{
    return ipAddress;
}


const std::string& NetworkInfo::GetGateway() const
{
    return gateway;
}


const std::string& NetworkInfo::GetDNS() const
{
    return dns;
}


bool NetworkInfo::IsDHCPEnabled() const
{
    return dhcpEnabled;
}