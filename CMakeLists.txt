cmake_minimum_required(VERSION 3.10)
project(mqtt-to-influxdb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#include(cmake/ClangFormat.cmake)

option(WITH_TESTS "Enable building of unit tests" ON)

find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)

add_subdirectory(3rdparty)
add_subdirectory(src)
if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
  find_package(Catch2 CONFIG REQUIRED)
endif()

