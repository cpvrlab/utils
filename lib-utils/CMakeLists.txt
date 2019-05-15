# 
# CMake configuration for lib-Utils
#

set(target lib-Utils)

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")
set(platform_specific_link_libraries)
set(platform_specific_include_directories)

set(compile_definitions
    ${compile_definitions}
	ASIO_STANDALONE
	ASIO_HAS_STD_TYPE_TRAITS
	ASIO_HAS_STD_SHARED_PTR
	ASIO_HAS_STD_ADDRESSOF
)

set(headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/source/Utils.h)
set(sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/source/Utils.cpp)

#==============================================================================
if("${SYSTEM_NAME_UPPER}" STREQUAL "WINDOWS")
    set(platform_specific_include_dirs
        ${UTILS_ROOT}/thirdparty/dirent)
    set(headers ${headers} ${UTILS_ROOT}/thirdparty/dirent/dirent.h)
    set(sources ${sources} ${UTILS_ROOT}/thirdparty/dirent/dirent.c)
elseif("${SYSTEM_NAME_UPPER}" STREQUAL "ANDROID") #----------------------------
    set(platform_specific_include_dirs
        ${UTILS_ROOT}/thirdparty/dirent)
endif()
#==============================================================================

add_library(${target}
    ${headers}
    ${sources}
    )

add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "external"
    )

target_include_directories(${target}
    PRIVATE

    PUBLIC
    ${platform_specific_include_dirs}
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/asio/include
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    )

target_compile_definitions(${target}
  PRIVATE
  
    PUBLIC
    ${compile_definitions}
    ${DEFAULT_COMPILE_DEFINITIONS}

    INTERFACE
    )

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${EXTERNAL_LIB_COMPILE_OPTIONS}

    INTERFACE
    )

include(${UTILS_ROOT}/cmake/PlatformLinkLibs.cmake)

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${PlatformLinkLibs}
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
    )
