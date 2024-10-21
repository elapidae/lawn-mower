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
include( ${VLIBS_DIR}/cmake/vlog.cmake )
include( ${VLIBS_DIR}/cmake/threads.cmake )
include( ${VLIBS_DIR}/cmake/vtimer.cmake )
include( ${VLIBS_DIR}/cmake/vsignal.cmake )
set( sources ${V_HEADERS} ${V_SOURCES} )

set(raspi_src
    ../test_raspi/engine.cpp ../test_raspi/engine.h
    ../test_raspi/usonic.cpp ../test_raspi/usonic.h
    ../test_raspi/gpio_init.cpp ../test_raspi/gpio_init.h
    )
include_directories( ../test_raspi/ )

set(PROJECT_SOURCES
        ${raspi_src}
        ${sources}
        main.cpp
        mainwindow.cpp       mainwindow.h       mainwindow.ui
        interactmoveform.cpp interactmoveform.h interactmoveform.ui
        ${V_SOURCES}
        ${V_HEADERS}
        test_rounds_reader.cpp
        test_rounds_reader.h
        lawn.cpp lawn.h
)

add_executable( spin-scene ${PROJECT_SOURCES} )


target_link_libraries( ${PROJECT_NAME} rt pigpio ${V_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)

