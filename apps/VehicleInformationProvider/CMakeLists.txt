cmake_minimum_required(VERSION 3.16)

project(VehicleInformationProvider LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VehicleInformationProvider main.cpp)

include(GNUInstallDirs)
install(TARGETS VehicleInformationProvider
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
