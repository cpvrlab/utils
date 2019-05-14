#
# CMake configuration of the cpvrlab/utils repository
#

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# The FOLDER properties are needed in Visual Studio and XCode generated projects for nested folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

set(META_PROJECT_NAME Utils)
set(UTILS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Declare project
project(${META_PROJECT_NAME} C CXX)

include(cmake/CompileOptions.cmake)
include(cmake/PlatformLinkLibs.cmake)

add_subdirectory(lib-Utils)
