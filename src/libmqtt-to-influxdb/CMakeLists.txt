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

if (MQTT_TO_INFLUXDB_BUILD_SHARED)
  add_library(libmqtt-to-influxdb ${files})
  if (MSVC)
    target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB)
  else()
    target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB stdc++fs)
  endif()
else()
  add_library(libmqtt-to-influxdb STATIC ${files})
  if (MSVC)
    target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog_header_only yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB)
    target_link_options(libmqtt-to-influxdb PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
    target_compile_options(libmqtt-to-influxdb PUBLIC /MT)
  else()
    target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB stdc++fs -static)
  endif()
endif()

set_target_properties(libmqtt-to-influxdb PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(libmqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()
