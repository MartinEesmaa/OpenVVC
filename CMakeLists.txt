cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0048 NEW)

project(openVVC VERSION 1.1.0 LANGUAGES C)

option(ENABLE_SIMD "Enable SIMD optimizations" ON)
option(ENABLE_ARM_ASM "Enable ARM assembly optimizations" OFF)
option(ENABLE_ARM_SIMDE "Enable ARM SIMDE optimizations" OFF)
option(ENABLE_SLHDR "Enable SLHDR support" OFF)
option(ENABLE_WERROR "Treat warnings as errors" OFF)

# Choose static by default for library builds. Set -DBUILD_SHARED_LIBS=ON to build shared libs.
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(ENABLE_WERROR)
  if(MSVC)
    add_compile_options(/W3 /WX)
  else()
    add_compile_options(-Wall -Wextra -Werror)
  endif()
else()
  if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
  endif()
endif()

find_package(Threads REQUIRED)

add_subdirectory(libovvc)
add_subdirectory(examples)

install(FILES VERSION DESTINATION share/openvvc)
