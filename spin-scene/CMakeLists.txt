cmake_minimum_required(VERSION 3.5)

project(spin-scene VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_definitions(-DV_HAS_QT)
set(VLIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/../vlibs2")
include(${VLIBS_DIR}/cmake/vlog.cmake)

set(raspi_src
    ../test_raspi/engine.cpp ../test_raspi/engine.h
    ../test_raspi/usonic.cpp ../test_raspi/usonic.h
    ../test_raspi/gpio_init.cpp ../test_raspi/gpio_init.h
    )

set(PROJECT_SOURCES
        ${raspi_src}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${V_SOURCES}
        ${V_HEADERS}
        test_rounds_reader.cpp
        test_rounds_reader.h
        lawn.cpp lawn.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(spin-scene
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET spin-scene APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(spin-scene SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(spin-scene
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries( ${PROJECT_NAME} rt pigpio ${V_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)

