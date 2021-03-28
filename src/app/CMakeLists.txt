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

set(libraries libmqtt-to-influxdb eventpp)

if (WITH_MQTT_NAME_UUID)
  set(libraries ${libraries} stduuid)
endif()

if (MQTT_TO_INFLUXDB_BUILD_SHARED)
  set(libraries ${libraries} PahoMqttCpp::paho-mqttpp3)
else()
  set(libraries ${libraries} PahoMqttCpp::paho-mqttpp3-static)
endif()

target_link_libraries(mqtt-to-influxdb PRIVATE ${libraries})

target_include_directories(mqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)


install(TARGETS mqtt-to-influxdb DESTINATION bin)

install(FILES "${CMAKE_SOURCE_DIR}/contrib/mqtt-to-influxdb.service.sample"
        DESTINATION "/usr/share/mqtt-to-influxdb/")
install(FILES "${CMAKE_SOURCE_DIR}/contrib/example.yaml"
        DESTINATION "/usr/share/mqtt-to-influxdb/")
install(FILES "${CMAKE_SOURCE_DIR}/contrib/example-json.yaml"
        DESTINATION "/usr/share/mqtt-to-influxdb/")

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Hamburg")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)
else()
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
endif()
INCLUDE(CPack)
