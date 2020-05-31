cmake_minimum_required(VERSION 3.15)
project(sensor)
set(CMAKE_CXX_STANDARD 20)
add_executable(
        sensor src/main.cpp
        ../Common/SslClientsHandler.cpp
        ../Common/DoubleMeasurement.cpp
        ../Common/SystemUtils.cpp
        ../Common/NetworkUtils.cpp
        ../Common/UserPrefs.cpp
        ../Common/ConnectionException.cpp
        ../Common/IClientsHandler.cpp
        ../Common/ClientsHandler.cpp
        ../Common/IRequestListener.cpp
        src/MeasureReader.cpp
        src/MeasureReader.h
        src/NetworkListener.cpp
        src/NetworkListener.h)
target_include_directories(sensor PRIVATE ../Common)
if(WIN32)
    target_link_libraries(sensor wsock32 ws2_32)
endif()

set(I "/usr/local/Cellar/openssl/1.0.2q/include")
set(L "/usr/local/Cellar/openssl/1.0.2q/lib")
include_directories(${I})

set(CMAKE_CXX_STANDARD_LIBRARIES "-lssl -lcrypto")