cmake_minimum_required(VERSION 3.15)

project(VehicleController VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

set(EXAMPLE_CONFIG_FILES
    ./vsomeip.json
)


find_package(pigpio REQUIRED)
find_package(vsomeip3 3.5.3 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )


include_directories(
	./Adafruit_PCA9685
	./Gamepad
	./PiRacer
	${pigpio_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	${VSOMEIP_INCLUDE_DIRS}

)

set(PIRACER_SRC
	./Adafruit_PCA9685/Adafruit_PCA9685.cpp
	./PiRacer/PiRacer.cpp
)

set(GAMEPAD_SRC
	./Gamepad/Joystick.cpp
	./Gamepad/ShanwanGamepad.cpp
)

add_executable(vehicle-controller
	main.cpp
	${PIRACER_SRC}
	${GAMEPAD_SRC}
	${EXAMPLE_CONFIG_FILES}

)

target_link_libraries(vehicle-controller ${pigpio_LIBRARY} ${Boost_LIBRARIES})
