CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

PROJECT(UdpServerClient)

# Remove unwanted project types in MSVC
if(MSVC)
    list(REMOVE_ITEM CMAKE_CONFIGURATION_TYPES MinSizeRel RelWithDebInfo)
endif()

if(MSVC)
    FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        IF(${flag_var} MATCHES "/MD")
            STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        ENDIF()
    ENDFOREACH(flag_var)
endif()

    
set(SRC_client UdpClient/client.cpp)
set(SRC_server UdpServer/server.cpp)
set(SRC_lib src/UdpServer.cpp src/UdpServer.h src/UdpClient.cpp src/UdpClient.h)

add_library(udpserverclient ${SRC_lib})
set_property(TARGET udpserverclient PROPERTY CXX_STANDARD 17)
set_property(TARGET udpserverclient PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_definitions(udpserverclient PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_definitions(udpserverclient PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

add_executable(server ${SRC_server})
add_executable(client ${SRC_client})
set_property(TARGET client PROPERTY CXX_STANDARD 17)
set_property(TARGET client PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET server PROPERTY CXX_STANDARD 17)
set_property(TARGET server PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(server PRIVATE $<$<CONFIG:Debug>:_DEBUG> _CONSOLE)
target_compile_definitions(client PRIVATE $<$<CONFIG:Debug>:_DEBUG> _CONSOLE)
target_link_libraries(server PRIVATE udpserverclient)
target_link_libraries(client PRIVATE udpserverclient)