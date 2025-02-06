cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

set(EXAMPLE_CONFIG_FILES
    "../config/vsomeip.json"
)


find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

# 서버. main.cpp, service-main.cpp, 스피드와 배터리
add_executable(server-draft ../src/main.cpp ../src/service-main.cpp ../src/service_base/service-base.cpp ../src/speed/speed.cpp ../src/speed/canreceiver.cpp ../src/battery/battery.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(server-draft vsomeip3 ${Boost_LIBRARIES})

# add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service-example.cpp ../src/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# speed server
# add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service_base/service-base.cpp ../src/speed/speed.cpp ../src/speed/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# speed client
add_executable(speed_client-example ../src/speed_client/client-main.cpp ../src/speed_client/client-example.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(speed_client-example vsomeip3 ${Boost_LIBRARIES})


# battery server
# add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service_base/service-base.cpp ../src/battery/battery.cpp ../src/speed/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# battery client
add_executable(battery_client-example ../src/battery_client/client-main.cpp ../src/battery_client/client-example.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(battery_client-example vsomeip3 ${Boost_LIBRARIES})
