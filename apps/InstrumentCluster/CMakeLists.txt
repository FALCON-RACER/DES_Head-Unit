cmake_minimum_required(VERSION 3.16)

project(InstrumentCluster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(instrument-cluster
    main.cpp
)

qt_add_qml_module(instrument-cluster
    URI InstrumentCluster
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES Speedometer.qml
        QML_FILES Time.qml
        QML_FILES Battery.qml
        QML_FILES Gear.qml
        QML_FILES
        SOURCES instrumentclustercontroller.h instrumentclustercontroller.cpp
)

qt_add_resources(instrument-cluster "resources"
    PREFIX /
    FILES
        images/background.png
        images/lightning.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(instrument-cluster PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.instrument-cluster
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(instrument-cluster
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS instrument-cluster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
