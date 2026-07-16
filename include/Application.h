#pragma once

#include "System.h"
#include "system/OSInfo.h"
#include "system/Uptime.h"
#include "system/MemoryInfo.h"
#include "hardware/HardwareInfo.h"
#include "hardware/DiskInfo.h"
#include "network/NetworkInfo.h"
#include "printer/PrinterInfo.h"
#include "system/DomainInfo.h"


class Application
{

public:

    Application();
    ~Application();


    void LoadInfos();
    void Render();
    void SetLogoTexture(void* texture, int width, int height);


private:

    SystemInfo system;

    OSInfo os;

    Uptime uptime;

    MemoryInfo memory;

    HardwareInfo hardware;

    DiskInfo disk;

    NetworkInfo network;

    PrinterInfo printer;

    DomainInfo domain;

    void* m_logoTexture = nullptr;
    int m_logoWidth = 0;
    int m_logoHeight = 0;

};