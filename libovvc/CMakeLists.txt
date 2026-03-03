cmake_minimum_required(VERSION 3.16)

project(libovvc NONE)

option(ENABLE_SSE4_1 "Enable SSE4.1 optimizations" ON)
option(ENABLE_NEON "Enable NEON optimizations" ON)

# If parent project disabled SIMD, force-disable arch-specific optimizations
if(DEFINED ENABLE_SIMD)
  if(NOT ENABLE_SIMD)
    set(ENABLE_SSE4_1 OFF CACHE BOOL "Enable SSE4.1 optimizations" FORCE)
    set(ENABLE_NEON OFF CACHE BOOL "Enable NEON optimizations" FORCE)
  endif()
endif()

set(LIBOVVC_PUBLIC_HEADERS
    ovdefs.h
    ovunits.h
    ovlog.h
    ovdec.h
    ovframe.h
    ovio.h
    ovdmx.h
    ovversion.h
)

# Ensure the libovvc source dir is on include path so our local stdatomic.h
# (compat fallback) is picked up before a missing system header.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Generate ovconfig.h from template
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ovconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/ovconfig.h @ONLY)

# Make generated header available to targets
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Generate ovversion.h from VERSION file and git info (similar to version.sh)
file(READ ${CMAKE_SOURCE_DIR}/VERSION _ver_content)
string(STRIP "${_ver_content}" _ver_content)
string(REPLACE "." ";" _ver_semi "${_ver_content}")
string(REGEX MATCHALL "[0-9]+" _ver_parts "${_ver_content}")
list(GET _ver_parts 0 VER_MAJOR)
list(GET _ver_parts 1 VER_MINOR)
list(GET _ver_parts 2 VER_REVISION)

execute_process(COMMAND git rev-parse --short HEAD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE _git_rev OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET)
set(VER_BUILD "${_git_rev}")
execute_process(COMMAND git update-index --refresh
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE _git_index_result
                OUTPUT_QUIET ERROR_QUIET)
if(NOT _git_index_result EQUAL 0)
  if(VER_BUILD STREQUAL "")
    set(VER_BUILD "-dirty")
  else()
    set(VER_BUILD "${VER_BUILD}-dirty")
  endif()
endif()

set(OV_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_REVISION}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ovversion.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/ovversion.h @ONLY)

# DSP template sources (used for bitdepth-specific static libs)
set(DSP_TEMPLATE_SOURCES
    rcn_alf.c
    rcn_ctu.c
    rcn_df.c
    rcn_intra.c
    rcn_inter.c
    rcn_intra_cclm.c
    rcn_prof_bdof.c
    rcn_transform.c
    rcn_transform_scale.c
    rcn_transform_tree.c
    rcn_fill_ref.c
    rcn_intra_angular.c
    rcn_intra_dc_planar.c
    rcn_ibc.c
    rcn_lmcs.c
    rcn_residuals.c
    rcn_dequant.c
    rcn_intra_mip.c
    rcn_mc.c
    rcn_sao.c
)

add_library(libovvc8bit OBJECT ${DSP_TEMPLATE_SOURCES})
target_compile_definitions(libovvc8bit PRIVATE BITDEPTH=8)

add_library(libovvc10bit OBJECT ${DSP_TEMPLATE_SOURCES})
target_compile_definitions(libovvc10bit PRIVATE BITDEPTH=10)

# Main library sources
set(LIBOVVC_SOURCES
    ctudec.c
    data_rcn_angular.c
    data_rcn_mip.c
    data_rcn_transform.c
    data_scan_lut.c
    dec_init.c
    dpb.c
    dpb_internal.c
    mempool.c
    mvpool.c
    nvcl.c
    nvcl_dpb.c
    nvcl_nal_ph.c
    nvcl_nal_aps.c
    nvcl_nal_pps.c
    nvcl_nal_sps.c
    nvcl_nal_sei.c
    nvcl_ptl.c
    nvcl_rpl.c
    ovdec.c
    ovutils.c
    ovmem.c
    overror.c
    ovdmx.c
    ovio.c
    ovlog.c
    ovannexb.c
    ovunits.c
    ovframe.c
    ovframepool.c
    pp_film_grain.c
    post_proc.c
    pp_pic_scale.c
    ovthreads.c
    drv_affine_mvp.c
    drv_lines.c
    drv_lfnst.c
    drv_intra.c
    drv_mvp.c
    drv_ibc.c
    rcn.c
    rcn_lfnst.c
    rcn_gpm.c
    compat_old.c
    slicedec.c
    vcl_alf.c
    vcl_sao.c
    vcl_sh.c
    vcl_coding_unit.c
    vcl_coding_tree.c
    vcl_transform_unit.c
    vcl_residual_coding.c
    vcl_cabac.c
)

add_library(libovvc ${LIBOVVC_SOURCES})
# Include object files from the bitdepth object libraries so final archive contains all symbols
target_sources(libovvc PRIVATE $<TARGET_OBJECTS:libovvc8bit> $<TARGET_OBJECTS:libovvc10bit>)
target_link_libraries(libovvc PRIVATE Threads::Threads)
target_include_directories(libovvc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(ENABLE_SSE4_1)
  add_subdirectory(x86)
endif()

if(ENABLE_NEON)
  add_subdirectory(arm)
endif()

# Link architecture-optimized static libraries when available
if(TARGET libx86optim)
  target_link_libraries(libovvc PRIVATE libx86optim)
endif()

if(TARGET libarmoptim)
  target_link_libraries(libovvc PRIVATE libarmoptim)
endif()

install(TARGETS libovvc
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(TARGETS libovvc8bit libovvc10bit
        ARCHIVE DESTINATION lib)

install(FILES ${LIBOVVC_PUBLIC_HEADERS} DESTINATION include/openvvc)
