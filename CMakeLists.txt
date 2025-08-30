# SPDX-FileCopyrightText: Copyright (c) 2025-2026 Igal Alkon
# SPDX-FileCopyrightText: Copyright (c) 2026 ALKONTEK <git@alkontek.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.21)
project(vecmat LANGUAGES C VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(WITH_THREAD_SAFETY "Enable thread safety" OFF)
option(VECMAT_USE_F64 "Use double for floating-point vectors/matrices (VECMAT_USE_F64)" OFF)
option(VECMAT_USE_INT8 "Use int8_t for integer vectors/matrices (VECMAT_USE_INT8)" OFF)
option(VECMAT_USE_INT16 "Use int16_t for integer vectors/matrices (VECMAT_USE_INT16)" OFF)
option(VECMAT_USE_INT32 "Use int32_t for integer vectors/matrices (VECMAT_USE_INT32)" OFF)

# Only default-on when vecmat is the top-level project
option(VECMAT_BUILD_TESTS "Build vecmat tests" ${PROJECT_IS_TOP_LEVEL})
option(VECMAT_INSTALL "Generate install targets" ${PROJECT_IS_TOP_LEVEL})

set(_vecmat_int_opts 0)
if(VECMAT_USE_INT8)
    math(EXPR _vecmat_int_opts "${_vecmat_int_opts} + 1")
endif()
if(VECMAT_USE_INT16)
    math(EXPR _vecmat_int_opts "${_vecmat_int_opts} + 1")
endif()
if(VECMAT_USE_INT32)
    math(EXPR _vecmat_int_opts "${_vecmat_int_opts} + 1")
endif()
if(_vecmat_int_opts GREATER 1)
    message(FATAL_ERROR
            "VECMAT_USE_INT8, VECMAT_USE_INT16, and VECMAT_USE_INT32 are mutually exclusive. "
            "Pass only one, e.g. -DVECMAT_USE_INT16=ON")
endif()
unset(_vecmat_int_opts)

set(VECMAT_SOURCES
        src/vecmat.c
        src/vector2.c
        src/vector3.c
        src/vector4.c
        src/vector2i.c
        src/vector3i.c
        src/vector4i.c
        src/matrix2.c
        src/matrix3.c
        src/matrix4.c
        src/matrix2i.c
        src/matrix3i.c
        src/matrix4i.c
        src/quaternion.c
        src/vector2_ptr.c
        src/vector3_ptr.c
        src/vector4_ptr.c
        src/vector2i_ptr.c
        src/vector3i_ptr.c
        src/vector4i_ptr.c
        src/matrix2_ptr.c
        src/matrix3_ptr.c
        src/matrix4_ptr.c
        src/matrix2i_ptr.c
        src/matrix3i_ptr.c
        src/matrix4i_ptr.c
        src/quaternion_ptr.c
        src/easing.c
)

add_library(vecmat_shared SHARED ${VECMAT_SOURCES})
add_library(vecmat_static STATIC ${VECMAT_SOURCES})

set_target_properties(vecmat_shared PROPERTIES
        OUTPUT_NAME vecmat
        EXPORT_NAME vecmat         # exported as vecmat::vecmat
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)
set_target_properties(vecmat_static PROPERTIES
        OUTPUT_NAME vecmat
        EXPORT_NAME vecmat_static  # exported as vecmat::vecmat_static
)

# Namespaced aliases for in-tree / FetchContent consumers
add_library(vecmat::vecmat ALIAS vecmat_shared)
add_library(vecmat::vecmat_static ALIAS vecmat_static)

# Non-namespaced alias
add_library(vecmat ALIAS vecmat_shared)

target_compile_definitions(vecmat_shared PRIVATE _USE_MATH_DEFINES VEC_BUILD_SHARED)
target_compile_definitions(vecmat_static PRIVATE _USE_MATH_DEFINES)

target_compile_options(vecmat_shared PRIVATE -fno-strict-aliasing)
target_compile_options(vecmat_static PRIVATE -fno-strict-aliasing)

set(_vecmat_public_defs)
if(VECMAT_USE_F64)
    list(APPEND _vecmat_public_defs VECMAT_USE_F64)
endif()
if(VECMAT_USE_INT8)
    list(APPEND _vecmat_public_defs VECMAT_USE_INT8)
elseif(VECMAT_USE_INT16)
    list(APPEND _vecmat_public_defs VECMAT_USE_INT16)
elseif(VECMAT_USE_INT32)
    list(APPEND _vecmat_public_defs VECMAT_USE_INT32)
endif()

if(_vecmat_public_defs)
    target_compile_definitions(vecmat_shared PUBLIC ${_vecmat_public_defs})
    target_compile_definitions(vecmat_static PUBLIC ${_vecmat_public_defs})
endif()
unset(_vecmat_public_defs)

if(WITH_THREAD_SAFETY)
    target_compile_definitions(vecmat_shared PUBLIC DYNVEC_THREAD_SAFE)
    target_compile_definitions(vecmat_static PUBLIC DYNVEC_THREAD_SAFE)
endif()

target_include_directories(vecmat_shared
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_include_directories(vecmat_static
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(VECMAT_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(VECMAT_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS vecmat_shared vecmat_static
            EXPORT vecmatTargets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT vecmatTargets
            FILE vecmatTargets.cmake
            NAMESPACE vecmat::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vecmat
    )

    configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vecmatConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/vecmatConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vecmat
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/vecmatConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/vecmatConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/vecmatConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vecmat
    )
endif()
