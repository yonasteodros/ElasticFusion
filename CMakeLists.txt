cmake_minimum_required(VERSION 3.1.0)

# c++ version
set(CMAKE_CXX_STANDARD 14)

# GCC warning / errors
if (CMAKE_COMPILER_IS_GNUCC)
  add_compile_options(-Wall -Wno-unused-function -Wno-unused-variable
    -Wno-unused-but-set-variable -Wno-unused-but-set-variable -Wno-write-strings
    -Wno-deprecated -Wno-deprecated-declarations -Werror -Wno-unknown-pragmas)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.1)
    add_compile_options(-Wno-misleading-indentation)
  endif()
endif()

# Don't follow symlinks when FILE GLOB_RECURSE (and don't warn)
cmake_policy(SET CMP0009 NEW)

project(VicarioFusionTop)

set(BUILD_TESTS OFF CACHE BOOL "Build unit tests")


# -- CORE
add_subdirectory(Core/src)

# -- GUI
add_subdirectory(VicariosFusion/src)

# -- show all files
file(GLOB_RECURSE showfiles Core/* VicariosFusion/* Scripts/* README.md)
add_custom_target(showthings SOURCES ${showfiles})


