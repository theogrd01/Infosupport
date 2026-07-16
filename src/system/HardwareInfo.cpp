#include "hardware/HardwareInfo.h"

#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")


std::string GetWMIValue(IWbemServices* services, const char* query, const wchar_t* property)
{
    IEnumWbemClassObject* enumerator = nullptr;

    HRESULT result = services->ExecQuery(
        bstr_t("WQL"),
        bstr_t(query),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &enumerator
    );


    if (FAILED(result))
    {
        return "Inconnu";
    }


    IWbemClassObject* object = nullptr;
    ULONG returned = 0;


    if (enumerator->Next(
            WBEM_INFINITE,
            1,
            &object,
            &returned) == S_OK)
    {
        VARIANT value;

        VariantInit(&value);

        object->Get(
            property,
            0,
            &value,
            nullptr,
            nullptr);


        std::string output;


        if (value.vt == VT_BSTR)
        {
            _bstr_t text(value.bstrVal);

            output = (const char*)text;
        }
        else
        {
            output = "Inconnu";
        }


        VariantClear(&value);
        object->Release();

        return output;
    }


    return "Inconnu";
}



void HardwareInfo::Load()
{
    CoInitializeEx(
        nullptr,
        COINIT_MULTITHREADED);


    CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr);



    IWbemLocator* locator = nullptr;
    IWbemServices* services = nullptr;


    HRESULT result = CoCreateInstance(
        CLSID_WbemLocator,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&locator);



    if (SUCCEEDED(result))
    {
        result = locator->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"),
            nullptr,
            nullptr,
            nullptr,
            0,
            nullptr,
            nullptr,
            &services);
    }



    if (SUCCEEDED(result))
    {
        manufacturer = GetWMIValue(
            services,
            "SELECT Manufacturer FROM Win32_ComputerSystem",
            L"Manufacturer");


        model = GetWMIValue(
            services,
            "SELECT Model FROM Win32_ComputerSystem",
            L"Model");


        serialNumber = GetWMIValue(
            services,
            "SELECT SerialNumber FROM Win32_BIOS",
            L"SerialNumber");
    }
    else
    {
        manufacturer = "Inconnu";
        model = "Inconnu";
        serialNumber = "Inconnu";
    }


    if (services)
        services->Release();

    if (locator)
        locator->Release();


    CoUninitialize();
}



const std::string& HardwareInfo::GetManufacturer() const
{
    return manufacturer;
}


const std::string& HardwareInfo::GetModel() const
{
    return model;
}


const std::string& HardwareInfo::GetSerialNumber() const
{
    return serialNumber;
}