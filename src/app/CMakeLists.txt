add_executable(mqtt-to-influxdb
        main.cpp

        mqtt/imqttlistenerfactory.hpp
        mqtt/mqttlistenerfactory.cpp
        mqtt/mqttlistenerfactory.hpp

        mqtt/imqttlistener.hpp
        mqtt/mqttlistener.cpp
        mqtt/mqttlistener.hpp

        mqtt/internal/logactionlistener.cpp
        mqtt/internal/logactionlistener.hpp
        mqtt/internal/mqttclientcallback.cpp
        mqtt/internal/mqttclientcallback.hpp
        )

target_link_libraries(mqtt-to-influxdb PRIVATE libmqtt-to-influxdb PahoMqttCpp::paho-mqttpp3 eventpp uuid)

target_include_directories(mqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(mqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
else()
  target_compile_options(mqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wnull-dereference -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()

install(TARGETS mqtt-to-influxdb DESTINATION bin)
