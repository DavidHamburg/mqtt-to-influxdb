SET (files
       	datatype.hpp
       	mqttparseresult.hpp

       	imqttmessageparser.hpp
       	mqttmessageparser.cpp
       	mqttmessageparser.hpp

       	stringhelper.cpp
       	stringhelper.hpp

       	config/configuration.cpp
       	config/configuration.hpp
       	config/device.hpp
       	config/duplicatedeviceexception.cpp
       	config/duplicatedeviceexception.hpp
       	config/document.hpp
       	config/field.hpp
       	config/measurement.hpp
       	config/settings.hpp
       	config/topic.hpp

        database/iiotrepository.hpp
        database/iotrepository.cpp
        database/iotrepository.hpp
)

set(libraries spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json InfluxData::InfluxDB OpenSSL::SSL OpenSSL::Crypto)

if (NOT MSVC)
  set(libraries ${libraries} stdc++fs)
endif()

add_library(libmqtt-to-influxdb ${files})

if (NOT MQTT_TO_INFLUXDB_BUILD_SHARED)
  if (MSVC)
    target_link_options(libmqtt-to-influxdb PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
    target_compile_options(libmqtt-to-influxdb PUBLIC /MT)
  else()
    set(libraries ${libraries} ${NGHTTP2_LIBRARIES} ZLIB::ZLIB -static)
  endif()
endif()

target_link_libraries(libmqtt-to-influxdb PUBLIC ${libraries})

set_target_properties(libmqtt-to-influxdb PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(libmqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()
