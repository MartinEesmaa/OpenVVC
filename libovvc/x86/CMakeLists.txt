cmake_minimum_required(VERSION 3.16)

project(libovvc_x86 NONE)

set(X86_SOURCES
    rcn_alf_avx2.c
    rcn_alf_sse.c
    rcn_dequant_sse.c
    rcn_df_sse.c
    rcn_dmvr_avx2.c
    rcn_dmvr_sse.c
    rcn_intra_angular_avx2.c
    rcn_intra_angular_sse.c
    rcn_intra_cclm_sse.c
    rcn_intra_dc_planar_sse.c
    rcn_intra_mip_avx2.c
    rcn_intra_mip_sse.c
    rcn_lfnst_sse.c
    rcn_mc_avx2.c
    rcn_mc_sse.c
    rcn_prof_bdof_avx2.c
    rcn_prof_bdof_sse.c
    rcn_sao_avx2.c
    rcn_sao_sse.c
    rcn_transform_add_avx2.c
    rcn_transform_add_sse.c
    rcn_transform_sse.c
    vvc_utils_sse.c
)

add_library(libx86optim STATIC ${X86_SOURCES})
target_include_directories(libx86optim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
