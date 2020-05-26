cmake_minimum_required(VERSION 3.15)
project(server)
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    find_path(OCCI occi.h $ENV{INSTANT_CLIENT}/sdk/include)
    include_directories(${OCCI})
else ()
    set(OCCI "/Users/robert/Downloads/instantclient_12_2 2/sdk/include")
    include_directories(${OCCI})
endif ()

add_executable(
        server
        src/main.cpp
        ../Common/ClientsHandler.cpp
        ../Common/IClientsHandler.cpp
        ../Common/ConnectionException.cpp
        ../Common/NetworkUtils.cpp
        ../Common/SslClientsHandler.cpp
        src/listeners/AdministratorListener.cpp
        src/listeners/AdministratorListener.h
        src/Sensor.h
        ../Common/IRequestListener.cpp
#        src/database/DatabaseManager.cpp
#        src/database/DatabaseConnection.cpp
        src/database/MockDatabaseManager.cpp
        src/database/MockDatabaseConnection.cpp
        src/SensorMeasurement.h
        src/Measurement.h
        src/listeners/MonitoringListener.cpp src/listeners/MonitoringListener.h

        src/serverModel/ServerModel.cpp
        src/serverModel/ServerModel.h
        src/serverModel/IModelForSensor.h
        src/serverModel/IModelForAdministrator.h
        src/serverModel/IModelForMonitoring.h

        src/serializers/SerializerMessage.h
        src/serializers/SerializerMessage.cpp
        src/serializers/SerializerSensorMessage.h
        src/serializers/SerializerAdministratorMessage.h
        src/serializers/SerializerAdministratorMessage.cpp
        src/serializers/SerializerMonitoringMessage.cpp
        src/commandTypes/AdministratorCommandTypes.h
        src/commandTypes/MonitoringCommandTypes.h
        src/requests/AdministratorRequest.h
        src/requests/MonitoringRequest.h
        src/requests/SensorRequest.h
        src/requests/Request.h
        src/commandTypes/SensorCommandTypes.h
        src/listeners/SensorListener.cpp
        src/listeners/SensorListener.h
        src/listeners/MonitoringListener.cpp
        src/listeners/MonitoringListener.h
        src/serializers/SerializerSensorMessage.cpp)
target_include_directories(server PRIVATE ../Common ../Server)
if (WIN32)
    target_link_libraries(server wsock32 ws2_32)
endif ()

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(server $ENV{INSTANT_CLIENT}/sdk/lib/msvc/vc14/oraocci12d.lib)
    endif ()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_link_libraries(server $ENV{INSTANT_CLIENT}/sdk/lib/msvc/vc14/oraocci12.lib)
    endif ()
else ()

#    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#        target_link_libraries(server "/Users/robert/Downloads/instantclient_12_2")
#    endif ()
#
#    if (CMAKE_BUILD_TYPE STREQUAL "Release")
#        target_link_libraries(server "/Users/robert/Downloads/instantclient_12_2")
#    endif ()

    target_link_libraries(server "/Users/robert/Downloads/instantclient_12_2")

    set(I "/usr/local/Cellar/openssl/1.0.2q/include")
    #set(L "/usr/local/Cellar/openssl/1.0.2q/lib")

    include_directories(${I})
endif ()

set(CMAKE_CXX_STANDARD_LIBRARIES "-lssl -lcrypto")
#-locci -lclntsh

#
#src/DatabaseConnection.h
#
#src/DatabaseManager.h
#src/IDatabaseManager.h
#src/IDatabaseConnection.h
