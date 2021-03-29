cmake_minimum_required(VERSION 3.10)
project(mqtt-to-influxdb LANGUAGES CXX VERSION 0.1.1)

set(CMAKE_CXX_STANDARD 17)

#include(cmake/ClangFormat.cmake)

option(MQTT_TO_INFLUXDB_BUILD_SHARED "Enable building of unit tests" ON)
option(WITH_TESTS "Enable building of unit tests" ON)
option(WITH_SERVICE "Enable building of unit tests" ON)
option(WITH_MQTT_NAME_UUID "use random uuid to generate unique mqtt listener name" ON)

if (MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

if (NOT MQTT_TO_INFLUXDB_BUILD_SHARED AND NOT MSVC)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(InfluxDB CONFIG REQUIRED)


if (WITH_MQTT_NAME_UUID)
  add_compile_definitions(WITH_MQTT_NAME_UUID)
endif()

if (WITH_SERVICE)
  find_package(PahoMqttCpp CONFIG REQUIRED)
  find_package(OpenSSL REQUIRED)
  if (WITH_MQTT_NAME_UUID)
    find_package(stduuid REQUIRED)
    find_package(Microsoft.GSL CONFIG REQUIRED)
  endif()

  if (NOT MQTT_TO_INFLUXDB_BUILD_SHARED)
    if (NOT MSVC)
      find_package(PkgConfig REQUIRED)
      find_package(ZLIB REQUIRED)
      pkg_search_module(NGHTTP2 libnghttp2)
    endif()
  endif()
endif()

add_subdirectory(3rdparty)
add_subdirectory(src)

if (WITH_TESTS)
  enable_testing()
  add_subdirectory(tests)
  find_package(Catch2 CONFIG REQUIRED)
endif()

