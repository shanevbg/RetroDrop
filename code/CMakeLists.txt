cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(RetroDrop C CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
set(CMAKE_VS_PLATFORM_NAME "Win32")
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
    message(STATUS "CMAKE_VS_PLATFORM_NAME not set. Defaulting to Win32.")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()
# --- DirectX 9 SDK (June 2010) Setup ---
# Attempt to find the DirectX SDK using the DXSDK_DIR environment variable
if(DEFINED ENV{DXSDK_DIR})
    set(DXSDK_ROOT_GUESS "$ENV{DXSDK_DIR}")
else()
    set(DXSDK_ROOT_GUESS "")
endif()

# Provide a CMake option to override or set the path if the environment variable isn't found/sufficient.
# This allows you to set it via -DDXSDK_ROOT="C:/Path/To/SDK" during CMake configuration.
set(DXSDK_ROOT "${DXSDK_ROOT_GUESS}" CACHE PATH "Path to the DirectX SDK (June 2010). Example: C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")

# Validate the DXSDK_ROOT path
if(NOT DXSDK_ROOT OR NOT IS_DIRECTORY "${DXSDK_ROOT}")
    message(FATAL_ERROR "DirectX SDK (June 2010) root directory not found or not specified. "
                        "Current DXSDK_ROOT: \"${DXSDK_ROOT}\".\n"
                        "Please ensure the SDK is installed and either:\n"
                        "1. The DXSDK_DIR environment variable points to the SDK's root directory, OR\n"
                        "2. The DXSDK_ROOT CMake variable is set correctly (e.g., -DDXSDK_ROOT=\"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)\").\n"
                        "Download SDK from: https://www.microsoft.com/en-us/download/details.aspx?id=6812")
endif()

# Define include and library directories based on DXSDK_ROOT
set(DIRECTX_INCLUDE_DIR "${DXSDK_ROOT}/Include")
if(NOT EXISTS "${DIRECTX_INCLUDE_DIR}/d3dx9.h")
    message(FATAL_ERROR "d3dx9.h not found in DirectX SDK Include directory: ${DIRECTX_INCLUDE_DIR}.\n"
                        "Please check your DXSDK_ROOT path: \"${DXSDK_ROOT}\".")
endif()

message(STATUS "Using DirectX SDK (June 2010) from: ${DXSDK_ROOT}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit build
    set(DIRECTX_LIBRARY_DIR "${DXSDK_ROOT}/Lib/x64")
    if(NOT IS_DIRECTORY "${DIRECTX_LIBRARY_DIR}")
        message(FATAL_ERROR "DirectX SDK x64 Library directory not found: ${DIRECTX_LIBRARY_DIR}. Check SDK installation and DXSDK_ROOT.")
    endif()
else() # 32-bit build
    set(DIRECTX_LIBRARY_DIR "${DXSDK_ROOT}/Lib/x86")
    if(NOT IS_DIRECTORY "${DIRECTX_LIBRARY_DIR}")
        message(FATAL_ERROR "DirectX SDK x86 Library directory not found: ${DIRECTX_LIBRARY_DIR}. Check SDK installation and DXSDK_ROOT.")
    endif()
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

# --- DirectX SDK ---
# Try to find the DirectX SDK. Users should set DXSDK_DIR environment variable
# to the root of their DirectX SDK (e.g., C:/Program Files (x86)/Microsoft DirectX SDK (June 2010))
set(DXSDK_DIR_ENV "$ENV{DXSDK_DIR}")
if(NOT DXSDK_DIR_ENV STREQUAL "" AND EXISTS "${DXSDK_DIR_ENV}/Include/d3d9.h")
    set(DXSDK_DIR ${DXSDK_DIR_ENV} CACHE PATH "Path to DirectX SDK")
    message(STATUS "Found DirectX SDK at: ${DXSDK_DIR}")
else()
    # Fallback if environment variable is not set or path is invalid
    # You might need to adjust this path or provide it via CMake GUI/command line
    find_path(DXSDK_INCLUDE_DIR d3d9.h
        PATHS "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
              "D:/SDKs/Microsoft DirectX SDK (June 2010)/Include"
        DOC "DirectX SDK Include directory")
    find_library(DXSDK_D3D9_LIBRARY d3d9
        PATHS "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
              "D:/SDKs/Microsoft DirectX SDK (June 2010)/Lib/x86"
        DOC "DirectX SDK d3d9.lib directory")
    find_library(DXSDK_D3DX9_LIBRARY d3dx9
        PATHS "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
              "D:/SDKs/Microsoft DirectX SDK (June 2010)/Lib/x86"
        DOC "DirectX SDK d3dx9.lib directory")

    if(DXSDK_INCLUDE_DIR AND DXSDK_D3D9_LIBRARY AND DXSDK_D3DX9_LIBRARY)
        get_filename_component(DXSDK_LIB_DIR ${DXSDK_D3D9_LIBRARY} DIRECTORY)
        set(DXSDK_DIR_FOUND TRUE)
        message(STATUS "Found DirectX SDK Include Dir: ${DXSDK_INCLUDE_DIR}")
        message(STATUS "Found DirectX SDK Lib Dir: ${DXSDK_LIB_DIR}")
    else()
        message(FATAL_ERROR "DirectX SDK not found. Please set the DXSDK_DIR environment variable or CMake variable, or update paths in CMakeLists.txt.")
    endif()
endif()

# Add subdirectories for the libraries/components
add_subdirectory(ns-eel2)
add_subdirectory(vis_milk2)

