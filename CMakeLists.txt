cmake_minimum_required(VERSION 3.7)

project(supersnakes_libretro)

set (CMAKE_CXX_STANDARD 14)

# Enable OpenMP for multithreading in Engine
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    ENDIF()
ENDIF()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O2")

# Include engine files
file(GLOB ENGINE_SOURCE "engine/*.hpp" "engine/*.cpp")

# Set source files
add_library(${PROJECT_NAME} SHARED
        libretro.hpp
        libretro.cpp
        game.cpp game.hpp
        input.hpp
        ${ENGINE_SOURCE})

# Removes the 'lib' prefix on Linux
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")