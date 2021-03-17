add_library(libmqtt-to-influxdb stub.cpp)

target_link_libraries(libmqtt-to-influxdb PUBLIC spdlog::spdlog yaml-cpp cxxopts::cxxopts nlohmann_json INFLUXDB)

target_include_directories(libmqtt-to-influxdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/>
    $<INSTALL_INTERFACE:include>
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
else()
  target_compile_options(libmqtt-to-influxdb PUBLIC -Wall -Wno-sign-compare -Wextra -Wnull-dereference -Wno-double-promotion -Wformat=2 -fno-builtin "$<$<CONFIG:DEBUG>:-DDEBUG>")
endif()
