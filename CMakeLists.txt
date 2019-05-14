#
# CMake top-level configuration of the cpvrlab/utils repository
#

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# The FOLDER properties are needed in Visual Studio and XCode generated projects for nested folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

set(META_PROJECT_NAME utils)
set(UTILS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Declare project
project(${META_PROJECT_NAME} C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/CompileOptions.cmake)

add_subdirectory(lib-utils)
