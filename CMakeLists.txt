cmake_minimum_required(VERSION 3.12)

project(DataComTCPClient)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwsock32")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_executable(DataComTCPClient
    "src/TCPClient.h"
    "src/TCPClient.c"
    "src/App.c"
)

if (WIN32)
    #target_link_libraries(DataComTCPClient "Ws2_32.lib")
    target_link_libraries(DataComTCPServer ws2_32)
endif ()