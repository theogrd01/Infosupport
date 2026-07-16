#pragma once


#include "System.h"


// System
#include "system/OSInfo.h"
#include "system/Uptime.h"
#include "system/MemoryInfo.h"
#include "system/DomainInfo.h"


// Hardware
#include "hardware/HardwareInfo.h"
#include "hardware/DiskInfo.h"


// Network
#include "network/NetworkInfo.h"


// Printer
#include "printer/PrinterInfo.h"



class MainWindow
{

public:

    MainWindow() = default;
    ~MainWindow() = default;


    // Charge toutes les informations du poste
    void Load();


    // Affiche la fenêtre ImGui
    void Render();



private:


    // Informations système

    SystemInfo system;

    OSInfo os;

    Uptime uptime;

    MemoryInfo memory;

    DomainInfo domain;



    // Informations matériel

    HardwareInfo hardware;

    DiskInfo disk;



    // Informations réseau

    NetworkInfo network;



    // Informations imprimante

    PrinterInfo printer;

};