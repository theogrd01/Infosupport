cmake_minimum_required(VERSION 3.20)

project(info-supportHCC)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Sources.cmake)

add_executable(info-supportHCC WIN32

    ${SYSINFO_SOURCES}
    ${PROJECT_SOURCE_DIR}/SysInfo.rc
)


target_include_directories(info-supportHCC PRIVATE

    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui-master
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui-master/backends

)

target_link_libraries(info-supportHCC

    d3d11
    dxgi
    windowscodecs
    ole32

)

add_custom_command(TARGET info-supportHCC POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:info-supportHCC>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/cmake/assets/logopasteur.png"
        "$<TARGET_FILE_DIR:info-supportHCC>/assets/logopasteur.png"
)