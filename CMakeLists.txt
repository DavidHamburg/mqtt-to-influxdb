cmake_minimum_required(VERSION 3.10)
project(mqtt-to-influxdb LANGUAGES CXX VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 17)

#include(cmake/ClangFormat.cmake)

option(MQTT_TO_INFLUXDB_BUILD_SHARED "Enable building of unit tests" ON)
option(WITH_TESTS "Enable building of unit tests" ON)
option(WITH_SERVICE "Enable building of unit tests" ON)

if (NOT MQTT_TO_INFLUXDB_BUILD_SHARED)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

if (WITH_SERVICE)
  find_package(PahoMqttCpp CONFIG REQUIRED)
  find_package(stduuid REQUIRED)
  find_package(Microsoft.GSL CONFIG REQUIRED)
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
  find_package(Catch2 CONFIG REQUIRED)
endif()

