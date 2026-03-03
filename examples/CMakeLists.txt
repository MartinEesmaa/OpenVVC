cmake_minimum_required(VERSION 3.16)

project(openvvc_examples NONE)

add_executable(dectest dectest.c)
# Include libovvc source headers and generated headers from libovvc build directory
target_include_directories(dectest PRIVATE ${CMAKE_SOURCE_DIR}/libovvc ${CMAKE_BINARY_DIR}/libovvc)
target_link_libraries(dectest PRIVATE libovvc)

install(TARGETS dectest RUNTIME DESTINATION bin)
