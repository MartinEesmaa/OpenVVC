cmake_minimum_required(VERSION 3.16)

project(libovvc_arm NONE)

# Collect C and assembly sources in this directory
file(GLOB ARM_SOURCES
    "*.c"
    "*.S"
)

add_library(libarmoptim STATIC ${ARM_SOURCES})
target_include_directories(libarmoptim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
