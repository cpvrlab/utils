# 
# Platform and architecture setup
# 

# Get upper case system name
string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_UPPER)

# Determine architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

set(DEFAULT_PROJECT_OPTIONS
    DEBUG_POSTFIX             "-debug"
    RELEASE_POSTFIX           "-release"
    CXX_STANDARD              11
    LINKER_LANGUAGE           "CXX"
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET     "hidden"
    CXX_EXTENSIONS            Off
    ENABLE_EXPORTS            ON
    )

set(DEFAULT_INCLUDE_DIRECTORIES)
set(DEFAULT_LIBRARIES)
set(DEFAULT_COMPILE_DEFINITIONS
    SYSTEM_${SYSTEM_NAME_UPPER}
    WAI_ROOT="${WAI_ROOT}"
    SL_GIT_BRANCH="${GitBranch}"
    SL_GIT_COMMIT="${GitCommit}"
    SL_GIT_DATE="${GitDate}"
    )
#message(STATUS "DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}")

# MSVC compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
        _SCL_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the Standard C++ Library
        _CRT_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the CRT Library
    )
endif ()

set(DEFAULT_COMPILE_OPTIONS)

# MSVC compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        /MP           # -> build with multiple processes
        /W3           # -> warning level 3
        /wd4251       # -> disable warning: 'identifier': class 'type' needs to have dll-interface to be used by clients of class 'type2'
        /wd4592       # -> disable warning: 'identifier': symbol will be dynamically initialized (implementation limitation)
        /bigobj
        
        $<$<CONFIG:Release>: 
        /Gw           # -> whole program global optimization
        /GS-          # -> buffer security check: no 
        /GL           # -> whole program optimization: enable link-time code generation (disables Zi)
        /GF           # -> enable string pooling
        >
        
        # No manual c++11 enable for MSVC as all supported MSVC versions for cmake-init have C++11 implicitly enabled (MSVC >=2013)
    )
endif ()

# GCC and Clang compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
		-Werror
        -Wall
        -Wno-sign-compare
        -Wno-reorder
        -Wno-reserved-id-macro
        -Wno-old-style-cast
        -Wno-zero-as-null-pointer-constant
        -Wno-documentation-unknown-command
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-cast-align
        -Wno-documentation
        -Wno-float-equal
        -Wno-shift-sign-overflow
        -Wno-cast-qual
        -Wno-sign-conversion
        -Wno-weak-vtables
        -Wno-padded
        -Wno-deprecated
        -Wno-deprecated-dynamic-exception-spec
        -Wno-double-promotion
        -Wno-unused-parameter
        -Wno-null-dereference
        -Wno-unused-function

        $<$<CXX_COMPILER_ID:Clang>:
            -Weverything
        >
    )
endif ()

set(EXTERNAL_LIB_COMPILE_OPTIONS)

# MSVC compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(EXTERNAL_LIB_COMPILE_OPTIONS ${EXTERNAL_LIB_COMPILE_OPTIONS}
        /MP           # -> build with multiple processes
        /W0           # -> warning level 0 all off
        /w

        $<$<CONFIG:Release>:
        /Gw           # -> whole program global optimization
        /GS-          # -> buffer security check: no
        /GL           # -> whole program optimization: enable link-time code generation (disables Zi)
        /GF           # -> enable string pooling
        >

        # No manual c++11 enable for MSVC as all supported MSVC versions for cmake-init have C++11 implicitly enabled (MSVC >=2013)
    )
endif ()

# GCC and Clang compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(EXTERNAL_LIB_COMPILE_OPTIONS ${EXTERNAL_LIB_COMPILE_OPTIONS}

        -w

        $<$<CXX_COMPILER_ID:GNU>:
            $<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
            >
        >

        $<$<CXX_COMPILER_ID:Clang>:
            -Wno-everything
        >
    )
endif ()

# 
# Linker options
# 

set(DEFAULT_LINKER_OPTIONS)
