find_package(Catch2 REQUIRED)

add_executable(tests main.cpp)

target_link_libraries(tests Catch2::Catch2 libmqtt-to-influxdb)

target_include_directories(tests PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tests/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/includes>
)
