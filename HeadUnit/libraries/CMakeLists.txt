cmake_minimum_required(VERSION 3.5)

# Project Definition
project(library LANGUAGES CXX)

# C++ Standard and Flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++0x")

# Set Installation Path (If not set in Upper CMakeLists.txt)
if (NOT INSTALL_DIR)
    set(INSTALL_DIRECTORY /usr/bin/app-hu)
endif()

# Define the list of interfaces for which to build proxy and stub libraries
set(proxy_stub_list "battery")

# Build Library
if (BUILD_LIB)

    # Find Required Packages
    find_package(CommonAPI REQUIRED)
    find_package(CommonAPI-SomeIP REQUIRED)
    find_package(vsomeip3 REQUIRED)

    # Include necessary directories for the found packages
    include_directories(
        ${COMMONAPI_INCLUDE_DIRS}
        ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
        ${VSOMEIP_INCLUDE_DIRS}
    )

    # Function to include the required directories for the interface
    function(proxy_stub_include interface)
        include_directories(
            src-gen/core/common/${interface}
            src-gen/core/proxy/${interface}
            src-gen/core/stub/${interface}
            src-gen/someip/common/${interface}
            src-gen/someip/proxy/${interface}
            src-gen/someip/stub/${interface}
        )
    endfunction()

    # Function to generate a library for the given type (stub or proxy) and interface name
    function(library_generator type name)
        # Adjust file path to match the correct structure with "v1/commonapi"
        file(GLOB_RECURSE SOMEIP_TYPE_LIB_SRCS 
        src-gen/someip/${type}/${name}/v1/commonapi/*.cpp
        )

        # Create the library name (lowercase)
        set(SOMEIP_TYPE_LIB_NAME ${name}someip${type})
        string(TOLOWER ${SOMEIP_TYPE_LIB_NAME} SOMEIP_TYPE_LIB_NAME)

        # Check if source files were found
        if (SOMEIP_TYPE_LIB_SRCS)
            # Add the library if source files are found
            add_library(${SOMEIP_TYPE_LIB_NAME} SHARED ${SOMEIP_TYPE_LIB_SRCS})
            target_link_libraries(${SOMEIP_TYPE_LIB_NAME} CommonAPI-SomeIP vsomeip3)

            # Install the generated library
            install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${SOMEIP_TYPE_LIB_NAME}.so
                DESTINATION ${INSTALL_DIRECTORY}/${PROJECT_NAME}
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
            )
        else()
            # Error message if no source files were found
            message(FATAL_ERROR "No source files found for ${SOMEIP_TYPE_LIB_NAME}")
        endif()
    endfunction()

    # Loop through each interface in the proxy_stub_list and generate the libraries
    foreach(interface ${proxy_stub_list})
        # Include the necessary directories for this interface
        proxy_stub_include(${interface})
        # Generate the stub and proxy libraries for this interface
        library_generator("stub" ${interface})
        library_generator("proxy" ${interface})
    endforeach()

endif()
