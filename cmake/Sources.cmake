set(SYSINFO_SOURCES
    src/main.cpp
    src/application.cpp
    src/system.cpp

    src/system/CPUInfo.cpp
    src/system/OSInfo.cpp
    src/system/Uptime.cpp
    src/system/MemoryInfo.cpp
    src/system/DiskInfo.cpp
    src/system/DomainInfo.cpp
    src/system/HardwareInfo.cpp
    include/graphics/TextureLoader.cpp

    src/network/NetworkInfo.cpp

    src/printer/PrinterInfo.cpp

    external/imgui/imgui-master/imgui.cpp
    external/imgui/imgui-master/imgui_draw.cpp
    external/imgui/imgui-master/imgui_tables.cpp
    external/imgui/imgui-master/imgui_widgets.cpp
    external/imgui/imgui-master/backends/imgui_impl_win32.cpp
    external/imgui/imgui-master/backends/imgui_impl_dx11.cpp
)
