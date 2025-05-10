cmake_minimum_required(VERSION 3.15)
project(nseel2_library C) # Use one project name, specify language

# List all source files for the ns-eel2 library.
# The original add_executable had an incomplete list.
# This list is based on what vis_milk2/CMakeLists.txt was compiling,
# assuming these files are directly in the current directory (code/ns-eel2/).
set(NSEEL2_SOURCES
    "nseel-caltab.c"
    "nseel-cfunc.c"
    "nseel-compiler.c"
    "nseel-eval.c"
    "nseel-lextab.c"
    "nseel-ram.c"
    "nseel-yylex.c"
    # Add other .c files if they exist and are needed, e.g., platform-specific assembly:
    # "asm-nseel-ppc-gcc.c"
    # "asm-nseel-x86-gcc.c"
    # "asm-nseel-x86-msvc.c"
    # "ns-eel.c" # If ns-eel.c exists (often the main API file), add it here.
    "ns-eel.h"   # Add header for IDEs; not compiled into static lib directly
)

# Define nseel2_library as a STATIC library
add_library(nseel2_library STATIC ${NSEEL2_SOURCES})

# Make the header file (ns-eel.h) available to other targets
# PUBLIC ensures that targets linking against nseel2_library also get these include directories.
target_include_directories(nseel2_library PUBLIC
    # This makes ns-eel.h findable via #include "ns-eel.h" for linked targets
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> 
    $<INSTALL_INTERFACE:include> # For potential installation
)
