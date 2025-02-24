cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

set(EXAMPLE_CONFIG_FILES
    "/home/frederik/Qt/Projects/DES_Head_Unit/apps/ServiceManager/config/vsomeip.json"
)

find_package (vsomeip3 3.4.10 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )


include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

message(STATUS "VSOMEIP_INCLUDE_DIRS: ${VSOMEIP_INCLUDE_DIRS}")
message(STATUS "VSOMEIP_LIBRARIES: ${VSOMEIP_LIBRARIES}")


# Server binary file
add_executable(server-draft 
        ../src/main.cpp 
        ../src/service-main.cpp 
        ../src/service_base/service-base.cpp 
        ../src/speed-src/speed/speed.cpp 
        ../src/speed-src/speed/canreceiver.cpp 
        ../src/battery-src/battery/battery.cpp 
        ../src/gear-src/gear/gear.cpp
        ./config/vsomeip.json
        ${EXAMPLE_CONFIG_FILES}
        )
target_link_libraries(server-draft vsomeip3 ${Boost_LIBRARIES})

# # add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service-example.cpp ../src/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# # target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# # speed receiving client
# add_executable(speed_client ../src/speed-src/speed_client/client-main.cpp ../src/speed-src/speed_client/client-example.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(speed_client vsomeip3 ${Boost_LIBRARIES})

# # battery receiving client
# add_executable(battery_client ../src/battery-src/battery_client/client-main.cpp ../src/battery-src/battery_client/client-example.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(battery_client vsomeip3 ${Boost_LIBRARIES})

# # joystick input
# add_executable(joystick-gear-client ../src/gear-src/J_gear_client/gear_client.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(joystick-gear-client vsomeip3 ${Boost_LIBRARIES})

# # HU input
# add_executable(HU-gear-client ../src/gear-src/HU_gear_client/gear_client.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(HU-gear-client vsomeip3 ${Boost_LIBRARIES})

# # gear client
# add_executable(gear_client 
#         ../src/gear-src/gear_data_receiving_client/client-main.cpp 
#         ../src/gear-src/gear_data_receiving_client/client-example.cpp 
#         ${EXAMPLE_CONFIG_FILES}
#         )
# target_link_libraries(gear_client vsomeip3 ${Boost_LIBRARIES})


# Req/Res testing
# add_executable(service-example ../src/service-example.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})
# add_executable(client-example ../src/client-example.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(client-example vsomeip3 ${Boost_LIBRARIES})

# -------------------
# speed server
# add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service_base/service-base.cpp ../src/speed/speed.cpp ../src/speed/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# battery server
# add_executable(service-example ../src/main.cpp ../src/service-main.cpp ../src/service_base/service-base.cpp ../src/battery/battery.cpp ../src/speed/canreceiver.cpp ${EXAMPLE_CONFIG_FILES})
# target_link_libraries(service-example vsomeip3 ${Boost_LIBRARIES})

# gear server
#add_executable(gear-service ../src/main.cpp ../src/service-main.cpp ../src/gear-src/gear/gear.cpp ../src/service_base/service-base.cpp ../src/speed-src/speed/speed.cpp ../src/speed-src/speed/canreceiver.cpp ../src/battery-src/battery/battery.cpp ${EXAMPLE_CONFIG_FILES})
#target_link_libraries(gear-service vsomeip3 ${Boost_LIBRARIES})
