cmake_minimum_required(VERSION 3.12)
project(xash_redirect VERSION 0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# ============================================================================
# Architecture detection (Metamod naming convention)
# ============================================================================
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)")
        set(PLATFORM_ARCH "arm64")
    else()
        set(PLATFORM_ARCH "amd64")
    endif()
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM)")
        set(PLATFORM_ARCH "arm")
    else()
        set(PLATFORM_ARCH "i386")
    endif()
endif()

# ============================================================================
# Source files
# ============================================================================
set(SOURCES
    dllapi.cpp
    engine_api.cpp
    ex_rehlds_api.cpp
    h_export.cpp
    meta_api.cpp
    public_amalgamation.cpp
    util.cpp
    redirect.cpp		
)	

# ============================================================================
# Main library target
# ============================================================================
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# ============================================================================
# Output name (Metamod convention)
# ============================================================================
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        PREFIX ""
        SUFFIX ".dll"
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}_mm_${PLATFORM_ARCH}"
        PREFIX ""
        SUFFIX ".so"
    )
endif()

# ============================================================================
# C++ standard
# ============================================================================
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ============================================================================
# Include directories
# ============================================================================
set(HLSDK_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/hlsdk)
set(METAMOD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/metamod)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${METAMOD_ROOT}
    ${HLSDK_ROOT}/common
    ${HLSDK_ROOT}/dlls
    ${HLSDK_ROOT}/engine
    ${HLSDK_ROOT}/game_shared
    ${HLSDK_ROOT}/pm_shared
    ${HLSDK_ROOT}/public
)

# ============================================================================
# NDEBUG for release builds
# ============================================================================
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:NDEBUG>
)

# ============================================================================
# Platform / compiler settings
# ============================================================================
if(MSVC)
    # --- MSVC (including clang-cl) ---
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/xash_redirect.def"
    )

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/Oi>
        $<$<CONFIG:Release>:/GS->
        $<$<CONFIG:Release>:/GL>
    )

    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:/arch:SSE2>
        )
    endif()
else()
    # --- GCC / Clang (Linux and MinGW) ---
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(${PROJECT_NAME} PRIVATE
            linux
            __linux__
            __USE_GNU
        )
    endif()

    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wno-unused-result
        -fvisibility=hidden
        -fomit-frame-pointer
        -fno-stack-protector
        -funroll-loops
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-flto>
    )

    if(UNIX)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
    endif()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86_64|AMD64|amd64)")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:-msse3>
        )
    endif()
endif()

# ============================================================================
# Install
# ============================================================================
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
