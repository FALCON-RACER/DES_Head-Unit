cmake_minimum_required(VERSION 3.16)

project(VehicleController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VehicleController main.cpp)

include(GNUInstallDirs)
install(TARGETS VehicleController
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
