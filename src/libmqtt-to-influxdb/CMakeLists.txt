add_library(libmqtt-to-influxdb
       	datatype.hpp
       	mqttparseresult.hpp
       	mqttmessageparser.cpp
       	mqttmessageparser.hpp
       	imqttmessageparser.hpp
       	stringhelper.cpp
       	stringhelper.hpp
       	config/configuration.cpp
       	config/configuration.hpp
       	config/device.hpp
       	config/duplicatedeviceexception.cpp
       	config/duplicatedeviceexception.hpp
       	config/document.hpp
       	config/field.hpp
       	config/topic.hpp
       	config/measurement.hpp)

target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB stdc++fs)
set_target_properties(libmqtt-to-influxdb PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(libmqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
else()
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wnull-dereference -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()
