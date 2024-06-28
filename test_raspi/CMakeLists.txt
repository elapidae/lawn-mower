cmake_minimum_required(VERSION 3.5)

project( test_raspi LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( VLIBS_DIR ../vlibs2 )
include( ${VLIBS_DIR}/cmake/vlog.cmake )
include( ${VLIBS_DIR}/cmake/threads.cmake )
include( ${VLIBS_DIR}/cmake/vtimer.cmake )
include( ${VLIBS_DIR}/cmake/vsignal.cmake )
set( sources ${V_HEADERS} ${V_SOURCES} )

set( sources ${sources}
     main.cpp
     engine.cpp engine.h
     gpio_init.h gpio_init.cpp
     usonic.h usonic.cpp
     )

add_executable( ${PROJECT_NAME} main.cpp ${sources} )

target_link_libraries( ${PROJECT_NAME} rt pigpio ${V_LIBRARIES} )
