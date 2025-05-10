set(PROJECT_NAME RetroDrop)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../audio/audiobuf.h"
    "../audio/cleanup.h"
    "../audio/common.h"
    "../audio/log.h"
    "../audio/loopback-capture.h"
    "../audio/prefs.h"
    "AudioDeviceManager.h"
)
source_group("" FILES ${no_group_source_files})

set(library__headers
    "AutoCharFn.h"
    "AutoWide.h"
    "defines.h"
    "dxcontext.h"
    "fft.h"
    "md_defines.h"
    "menu.h"
    "plugin.h"
    "pluginshell.h"
    "resource.h"
    "shell_defines.h"
    "state.h"
    "support.h"
    "texmgr.h"
    "textmgr.h"
    "utility.h"
    "wasabi.h"
)
source_group("library\\headers" FILES ${library__headers})

set(library__resources
    "text1.bin"
)
source_group("library\\resources" FILES ${library__resources})

set(library__sources
    "dxcontext.cpp"
    "fft.cpp"
    "menu.cpp"
    "milkdropfs.cpp"
    "plugin.cpp"
    "pluginshell.cpp"
    "state.cpp"
    "support.cpp"
    "texmgr.cpp"
    "textmgr.cpp"
    "utility.cpp"
    "wasabi.cpp"
)
source_group("library\\sources" FILES ${library__sources})

set(musikcube
    "../audio/audiobuf.cpp"
    "../audio/guid.cpp"
    "../audio/log.cpp"
    "../audio/loopback-capture.cpp"
    "../audio/prefs.cpp"
    "AudioDeviceManager.cpp"
    "Milkdrop2PcmVisualizer.cpp"
)
source_group("musikcube" FILES ${musikcube})

set(ALL_FILES
    ${no_group_source_files}
    ${library__headers}
    ${library__resources}
    ${library__sources}
    ${musikcube}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE vis_retro)

################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}_Debug"
        TARGET_NAME_RELEASE "${PROJECT_NAME}_Release"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}_Debug"
        TARGET_NAME_RELEASE "${PROJECT_NAME}_Release"
    )
endif()
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/./Release/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/./Release/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/./Release"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/./Release"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG   "vis_milk2"
        ARCHIVE_OUTPUT_NAME_RELEASE "vis_milk2"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG   "vis_milk2"
        ARCHIVE_OUTPUT_NAME_RELEASE "vis_milk2"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PDB_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        PDB_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        PDB_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
        PDB_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../ns-eel2"
        "$<$<CONFIG:Debug>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Include"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Include"
        ">"
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Lib/x86"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Lib/x86"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../ns-eel2"
        "$<$<CONFIG:Debug>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Include"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Include"
        ">"
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Lib/x64"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_PROGRAM_FILES_X86}/Microsoft DirectX SDK (June 2010)/Lib/x64"
        ">"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "STRSAFE_NO_DEPRECATE;"
            "NSEEL_REENTRANT_EXECUTION"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "USE_VIS_HDR_HWND;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_CRT_NON_CONFORMING_SWPRINTFS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "STRSAFE_NO_DEPRECATE;"
            "NSEEL_REENTRANT_EXECUTION"
        ">"
        "WIN32;"
        "_WINDOWS;"
        "USE_VIS_HDR_HWND;"
        "_CRT_SECURE_NO_WARNINGS;"
        "_CRT_NON_CONFORMING_SWPRINTFS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:Release>:
                /O1;
                /Ob2;
                /GF;
                /Oy;
                /Gy;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
                /GS-
            >
            /W3;
            /wd4996;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Zc:forScope-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:Release>:
                /O1;
                /Ob2;
                /GF;
                /Oy;
                /Gy;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
                /GS-
            >
            /W3;
            /wd4996;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Zc:forScope-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /SAFESEH:NO
            >
            $<$<CONFIG:Release>:
                /MANIFEST:NO;
                /OPT:REF;
                /OPT:ICF
            >
            /NOLOGO;
            /MACHINE:X86;
            /DYNAMICBASE:NO;
            /INCREMENTAL:NO
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /SAFESEH:NO
            >
            $<$<CONFIG:Release>:
                /MANIFEST:NO;
                /OPT:REF;
                /OPT:ICF
            >
            /NOLOGO;
            /MACHINE:X86;
            /DYNAMICBASE:NO;
            /INCREMENTAL:NO
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND   $<CONFIG:Debug> xcopy $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\ /Y
        COMMAND   $<CONFIG:Debug> if not exist "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2" mkdir "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2"
        COMMAND   $<CONFIG:Debug> xcopy $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>resources\\Milkdrop2\\* $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2 /Y /e
        COMMAND $<CONFIG:Release> xcopy $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\ /Y
        COMMAND $<CONFIG:Release> if not exist "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2" mkdir "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2"
        COMMAND $<CONFIG:Release> xcopy $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>resources\\Milkdrop2\\* $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2 /Y /e
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND   $<CONFIG:Debug> xcopy $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\ /Y
        COMMAND   $<CONFIG:Debug> if not exist "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2" mkdir "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2"
        COMMAND   $<CONFIG:Debug> xcopy $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>resources\\Milkdrop2\\* $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\debug\\plugins\\Milkdrop2 /Y /e
        COMMAND $<CONFIG:Release> xcopy $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\ /Y
        COMMAND $<CONFIG:Release> if not exist "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2" mkdir "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2"
        COMMAND $<CONFIG:Release> xcopy $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>resources\\Milkdrop2\\* $<SHELL_PATH:${CMAKE_SOURCE_DIR}/>..\\musikcube\\bin\\release\\plugins\\Milkdrop2 /Y /e
    )
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "user32"
        ">"
        "d3d9;"
        "d3dx9;"
        "Shlwapi"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "user32"
        ">"
        "d3d9;"
        "d3dx9;"
        "Shlwapi"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d9
    $<$<CONFIG:Debug>:d3dx9d>   # D3DX9 debug library for Debug builds
    $<$<NOT:$<CONFIG:Debug>>:d3dx9> # D3DX9 release library for other builds (Release, RelWithDebInfo, etc.)
    Shlwapi
    nseel2_library # Link against the ns-eel2 library built in the ns-eel2 subdirectory
    "${ADDITIONAL_LIBRARY_DEPENDENCIES}"
)
    # Optional: Set output directories for executables and libraries (useful for VSCode)
    set_target_properties(RetroDrop PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}" # Puts binaries in build/Debug, build/Release etc.
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
    )

# --- Optional: MSVC Specific Settings ---
if(MSVC)
    # Example: Set a higher warning level
    # target_compile_options(RetroDrop PRIVATE /W4)

    # Example: To use the static runtime library (optional, use with understanding)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
