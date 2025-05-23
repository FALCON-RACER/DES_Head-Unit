cmake_minimum_required(VERSION 3.16)

project(HeadUnit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Qml)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(head-unit

    main.cpp
    resources.qrc

    shared/utils/envmanager.h
    shared/utils/envmanager.cpp
    shared/utils/utils.h
    shared/utils/utils.cpp

    modules/spotify/spotify.h
    modules/spotify/spotify.cpp
)

qt_add_qml_module(head-unit
    URI HeadUnit
    VERSION 1.0

    QML_FILES
        Main.qml
        modules/menu/Menu.qml
        modules/home/Home.qml
        modules/settings/Settings.qml
        modules/media/Media.qml
        modules/spotify/Spotify_main.qml
        modules/spotify/Spotify_player.qml
        modules/spotify/Spotify_playlists.qml
        modules/youtube/Youtube_main.qml

    SOURCES
        modules/spotify/spotify.h
        modules/spotify/spotify.cpp
        shared/utils/envmanager.h
        shared/utils/envmanager.cpp
    DEPENDENCIES
        QtQuick
        QtQml
        QtBluetooth
    PLUGIN_IMPORTS
        QtBluetooth
)


find_package(Qt6 REQUIRED COMPONENTS Quick WebEngineWidgets Qml)
target_link_libraries(head-unit PRIVATE  Qt6::Quick Qt6::WebEngineWidgets Qt6::Qml)
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Qml Bluetooth)
target_link_libraries(head-unit PRIVATE Qt6::Bluetooth)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(head-unit PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHeadUnit
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(head-unit
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS head-unit
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
