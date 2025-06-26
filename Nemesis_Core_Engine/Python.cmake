################################################################################
# Variables
################################################################################

set(Python_VERSION 3.12.11)
set(Python_LIB_VERSION 312)


################################################################################
# Fetch
################################################################################
include(FetchContent)

FetchContent_Declare(
    CPython
    GIT_REPOSITORY https://github.com/python/cpython.git
    GIT_TAG v${Python_VERSION}
)
FetchContent_MakeAvailable(CPython)

set(PYTHON_ROOT ${cpython_SOURCE_DIR} CACHE PATH "Python local path" FORCE)

################################################################################
# Build
################################################################################
set(Python_INCLUDE_DIRS "${PYTHON_ROOT}/PC" "${PYTHON_ROOT}/Include")

set(Python_BUILD_DIR_ROOT ${PYTHON_ROOT}/PCBuild)
set(Python_BUILD_FILE ${Python_BUILD_DIR_ROOT}/build.bat)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(Python_BUILD_DIR ${Python_BUILD_DIR_ROOT}/amd64)
    set(Python_Args "-p" "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(Python_BUILD_DIR ${Python_BUILD_DIR_ROOT}/win32)
    set(Python_Args "-p" "x86")
else()
    message(ERROR "Unknown architecture for Python_Args. CMAKE_SIZEOF_VOID_P is ${CMAKE_SIZEOF_VOID_P}")
endif()

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(Python_LIBRARIES ${Python_BUILD_DIR}/python${Python_LIB_VERSION}_d.lib)
    set(Python_Args ${Python_Args} "-c" "Debug")
    set(Python_DLL_NAME Python${Python_LIB_VERSION}_d.dll)
else()
    set(Python_LIBRARIES ${Python_BUILD_DIR}/python${Python_LIB_VERSION}.lib)
    set(Python_Args ${Python_Args} "-c" "Release")
    set(Python_DLL_NAME Python${Python_LIB_VERSION}.dll)
endif()

message("Building CPython:    File: ${Python_BUILD_FILE}   Args: ${Python_Args}")
# execute_process(COMMAND ${Python_BUILD_FILE} ${Python_Args})

################################################################################
# Add target
################################################################################

add_library(PythonLib INTERFACE)
target_include_directories(PythonLib INTERFACE "${Python_INCLUDE_DIRS}")
target_link_libraries(PythonLib INTERFACE ${Python_LIBRARIES})
add_library(Nemesis::Python ALIAS PythonLib)

################################################################################
# Copy DLL
################################################################################

set(Python_DLL_PATH ${Python_BUILD_DIR}/${Python_DLL_NAME})
