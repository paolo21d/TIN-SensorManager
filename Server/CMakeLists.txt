cmake_minimum_required(VERSION 3.15)
project(server)
set(CMAKE_CXX_STANDARD 20)

find_path(OCCI occi.h $ENV{INSTANT_CLIENT}/sdk/include)

include_directories(${OCCI})

add_executable(server src/main.cpp
        src/ClientsHandler.cpp
        src/IClientsHandler.cpp
        ../Common/ConnectionException.cpp
        ../Common/NetworkUtils.cpp
        src/AdministratorListener.cpp
        src/AdministratorListener.h
        src/Sensor.h
        ../Common/IRequestListener.cpp
        src/DatabaseManager.cpp
        src/DatabaseConnection.cpp
        )

target_include_directories(server PRIVATE ../Common)
if(WIN32)
    target_link_libraries(server wsock32 ws2_32)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(server $ENV{INSTANT_CLIENT}/sdk/lib/msvc/vc14/oraocci12d.lib)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(server $ENV{INSTANT_CLIENT}/sdk/lib/msvc/vc14/oraocci12.lib)
endif ()

#
#src/DatabaseConnection.h
#
#src/DatabaseManager.h
#src/IDatabaseManager.h
#src/IDatabaseConnection.h
