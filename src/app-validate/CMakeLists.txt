add_executable(mqtt-to-influxdb-validate main.cpp)

target_link_libraries(mqtt-to-influxdb-validate PRIVATE libmqtt-to-influxdb)

target_include_directories(mqtt-to-influxdb-validate PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS mqtt-to-influxdb-validate DESTINATION bin)

