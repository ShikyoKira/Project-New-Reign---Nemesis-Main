#hkxcmd
Project(hkxcmd)

set(HAVOK_LIBS_DIR_RELEASE
    ${PROJECT_SOURCE_DIR}/havok_2010_2_0/Lib/win32_net_9-0/release_multithreaded)

set(HAVOK_LIBS_DIR_DEBUG
    ${PROJECT_SOURCE_DIR}/havok_2010_2_0/Lib/win32_net_9-0/debug_multithreaded)

set(HAVOK_LIBS
    hkaAnimation
    hkaInternal
    hkaRagdoll
    hkBase
    hkCompat
    hkgBridge
    hkgCommon
    hkgDx10
    hkgDx9
    hkgDx9s
    hkGeometryUtilities
    hkgOgl
    hkgOglES
    hkgOglES2
    hkgOgls
    hkInternal
    hkpCollide
    hkpConstraintSolver
    hkpDynamics
    hkpInternal
    hkpUtilities
    hkpVehicle
    hkSceneData
    hksCommon
    hkSerialize
    hksXAudio2
    hkVisualize
    )

foreach(havok_lib IN LISTS HAVOK_LIBS)
    add_library(
        ${havok_lib}
        STATIC
        IMPORTED GLOBAL
        )

    set_target_properties(
        ${havok_lib}
        PROPERTIES IMPORTED_LOCATION
        "${HAVOK_LIBS_DIR_RELEASE}/${havok_lib}.lib"
        )

    set_target_properties(
        ${havok_lib}
        PROPERTIES IMPORTED_LOCATION_DEBUG
        "${HAVOK_LIBS_DIR_DEBUG}/${havok_lib}.lib"
        )
endforeach()

#Useless files that causes compilation errors
file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/Source/ContentTools)

file(GLOB_RECURSE HAVOK_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/*.cpp"
    )

add_library(hkxcmd STATIC
    hkfutils.cpp
    hkfutils.h
    hkxcore.cpp
    hkxcore.h
    hkxcore.hxx
    hkxutils.cpp
    hkxutils.h
    stdafx.h
    ${HAVOK_HEADERS})

#MSVC
IF (MSVC)
target_compile_options(hkxcmd PRIVATE /bigobj /EHsc)
target_compile_definitions(hkxcmd PRIVATE "-D_CRT_SECURE_NO_WARNINGS=1" "D_CRT_NONSTDC_NO_DEPRECATE=1")
target_link_options(hkxcmd PRIVATE "/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:MSVCRTD")
ENDIF ()

target_link_libraries(hkxcmd PUBLIC ${HAVOK_LIBS} Shlwapi.lib legacy_stdio_definitions.lib)

target_include_directories(hkxcmd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hkxcmd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/Source)
target_include_directories(hkxcmd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/havok_2010_2_0/compat)

target_compile_features(hkxcmd PUBLIC cxx_std_17)

