# 
# CMake configuration for lib-Utils
#

set(target lib-Utils)

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}")
set(platform_specific_link_libraries)
set(platform_specific_include_directories)

file(GLOB headers
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Utils.h
    )

file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Utils.cpp
)


set(compile_definitions
    ${compile_definitions}
	ASIO_STANDALONE
	ASIO_HAS_STD_TYPE_TRAITS
	ASIO_HAS_STD_SHARED_PTR
	ASIO_HAS_STD_ADDRESSOF
)

#==============================================================================
if("${SYSTEM_NAME_UPPER}" STREQUAL "LINUX")

elseif("${SYSTEM_NAME_UPPER}" STREQUAL "WINDOWS") #----------------------------
    set(platform_specific_include_dirs
        ${UTILS_ROOT}/thirdparty/dirent)

elseif("${SYSTEM_NAME_UPPER}" STREQUAL "DARWIN") #-----------------------------

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
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
    )
