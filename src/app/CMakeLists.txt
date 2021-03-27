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

if (MQTT_TO_INFLUXDB_BUILD_SHARED)
  target_link_libraries(mqtt-to-influxdb PRIVATE libmqtt-to-influxdb PahoMqttCpp::paho-mqttpp3 stduuid eventpp)
else()
  target_link_libraries(mqtt-to-influxdb PRIVATE libmqtt-to-influxdb PahoMqttCpp::paho-mqttpp3-static stduuid eventpp -static)
endif()

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

install(FILES "${CMAKE_SOURCE_DIR}/contrib/mqtt-to-influxdb.service.sample"
        DESTINATION "/usr/share/mqtt-to-influxdb/")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Hamburg")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)
else()
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
endif()
INCLUDE(CPack)
