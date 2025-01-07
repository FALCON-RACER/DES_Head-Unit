cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

set(EXAMPLE_CONFIG_FILES
    "../config/vsomeip.json"
    "../config/vsomeip-local.json"
    "../config/vsomeip-tcp-client.json"
    "../config/vsomeip-tcp-service.json"
    "../config/vsomeip-udp-client.json"
    "../config/vsomeip-udp-service.json"
)


find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(service-example ../src/service-main.cpp ../src/service-example.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

add_executable(client-example ../src/client-main.cpp ../src/client-example.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(client-example vsomeip3 ${Boost_LIBRARIES})