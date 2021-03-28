# eventpp
set(EVENTPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eventpp/include/)
add_library(eventpp INTERFACE)
target_include_directories(eventpp SYSTEM INTERFACE $<BUILD_INTERFACE:${EVENTPP_INCLUDE_DIR}>)
