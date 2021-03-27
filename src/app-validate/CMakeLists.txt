add_executable(mqtt-to-influxdb-validate main.cpp)

if (MQTT_TO_INFLUXDB_BUILD_SHARED)
  target_link_libraries(mqtt-to-influxdb-validate PRIVATE libmqtt-to-influxdb)
else()
  target_link_libraries(mqtt-to-influxdb-validate PRIVATE libmqtt-to-influxdb -static)
endif()


target_include_directories(mqtt-to-influxdb-validate PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(mqtt-to-influxdb-validate PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()

install(TARGETS mqtt-to-influxdb-validate DESTINATION bin)

