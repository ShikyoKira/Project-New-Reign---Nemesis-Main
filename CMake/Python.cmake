################################################################################
# VARIABLES - Pre download
################################################################################

set(Python_VERSION 3.8.2)
set(Python_LIB_VERSION 38)


################################################################################
# Download Python
################################################################################

include(FetchContent)

if(NOT UseLocalPython)
    message(STATUS Downloading CPython)

    FetchContent_Declare(
        CPython
        GIT_REPOSITORY https://github.com/python/cpython.git
        GIT_TAG v${Python_VERSION}
        )

    if(NOT CPython_POPULATED)
      FetchContent_Populate(CPython)
    endif()

    set(PYTHON_ROOT ${cpython_SOURCE_DIR} CACHE PATH "Python local path" FORCE)
endif()

################################################################################
# VARIABLES - Post download
################################################################################
set(Python_INCLUDE_DIRS "${PYTHON_ROOT}/PC" "${PYTHON_ROOT}/Include")

set(Python_BUILD_DIR_ROOT ${PYTHON_ROOT}/PCBuild)
set(Python_BUILD_FILE ${Python_BUILD_DIR_ROOT}/build.bat)

set(Python_BUILD_DIR ${Python_BUILD_DIR_ROOT}/amd64)
set(Python_Args "-p" "x64")

#Configure based on build type

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(Python_LIBRARIES ${Python_BUILD_DIR}/python${Python_LIB_VERSION}_d.lib)
    set(Python_Args ${Python_Args} "-c" "Debug")
    set(Python_DLL_NAME Python${Python_LIB_VERSION}_d.dll)
else()
    set(Python_LIBRARIES ${Python_BUILD_DIR}/python${Python_LIB_VERSION}.lib)
    set(Python_Args ${Python_Args} "-c" "Release")
    set(Python_DLL_NAME Python${Python_LIB_VERSION}.dll)
endif()

message("PYTHON : lib ${Python_LIBRARIES}")

#Build
message("Building CPython:    File: ${Python_BUILD_FILE}   Args: ${Python_Args}")

execute_process(COMMAND ${Python_BUILD_FILE} ${Python_Args})


################################################################################
# Add target
################################################################################

add_library(PythonNemesis INTERFACE)

target_include_directories(PythonNemesis INTERFACE "${Python_INCLUDE_DIRS}")
target_link_libraries(PythonNemesis INTERFACE ${Python_LIBRARIES})

################################################################################
# Copy DLL
################################################################################

set(Python_DLL_PATH ${Python_BUILD_DIR}/${Python_DLL_NAME})
