find_package(Catch2 REQUIRED)

add_executable(tests main.cpp
	datatypefixture.cpp
	mqttmessageparserfixture.cpp
	stringhelperfixture.cpp
	config/configurationfixture.cpp
        config/documentfixture.cpp
        config/fieldfixture.cpp
        config/measurementfixture.cpp
        config/settingsfixture.cpp
        config/topicfixture.cpp)

target_link_libraries(tests Catch2::Catch2 libmqtt-to-influxdb)


include(CTest)
include(Catch)
catch_discover_tests(tests)

target_include_directories(tests PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tests/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/includes>
)
