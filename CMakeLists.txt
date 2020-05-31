cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(NemesisUnlimitedBehaviorEngine LANGUAGES CXX VERSION 0.84)
################################################################################
# VARIABLES
################################################################################


# This part is the only one that should be modified by the user wanting to compile Nemesis

set(Qt5_DIR "C:/IT/Qt/5.14.1/msvc2017/lib/cmake/Qt5")

option(UseLocalBoost "Uses local copy of Boost instead of downloading it" ON)
set(BOOST_ROOT "C:/IT/Boost_1.72.0" CACHE PATH "Boost local path" )

#You have to use a local copy of Python source, not the binaries
#Note: It MUST match the version set in Python.cmake
option(UseLocalPython "Uses local copy of Python instead of downloading it" ON)
set(PYTHON_ROOT "C:/IT/cpython" CACHE PATH "Python local path" )

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Required packages
################################################################################

include(CMake/Python.cmake)
include(CMake/Boost.cmake)
include(CMake/Qt.cmake)
add_subdirectory(external/hkxcmd)

################################################################################
# Source groups
################################################################################

set(Header_Files
    "include/connector.h"
    "include/debuglog.h"
    "include/debugmsg.h"
    "include/externalscript.h"
    "include/Global.h"
    "include/nemesisinfo.h"
    "include/resource.h"
    "include/version.h"
    )
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__generate
    "include/generate/addanims.h"
    "include/generate/addevents.h"
    "include/generate/addvariables.h"
    "include/generate/alternateanimation.h"
    "include/generate/animationdata.h"
    "include/generate/animationdatatracker.h"
    "include/generate/animationsetdata.h"
    "include/generate/animationsetdatatype.h"
    "include/generate/animationutility.h"
    "include/generate/behaviorcheck.h"
    "include/generate/behaviorgenerator.h"
    "include/generate/behaviorprocess.h"
    "include/generate/behaviorprocess_utility.h"
    "include/generate/behaviorsubprocess.h"
    "include/generate/behaviortype.h"
    "include/generate/generator_utility.h"
    "include/generate/installscripts.h"
    "include/generate/papyruscompile.h"
    "include/generate/playerexclusive.h"
    )

source_group("Header Files\\generate" FILES ${Header_Files__generate})

set(Header_Files__generate__animation
    "include/generate/animation/animationinfo.h"
    "include/generate/animation/animationthread.h"
    "include/generate/animation/animthreadinfo.h"
    "include/generate/animation/grouptemplate.h"
    "include/generate/animation/import.h"
    "include/generate/animation/newanimation.h"
    "include/generate/animation/nodejoint.h"
    "include/generate/animation/optionlist.h"
    "include/generate/animation/paired.h"
    "include/generate/animation/registeranimation.h"
    "include/generate/animation/singletemplate.h"
    "include/generate/animation/templateinfo.h"
    "include/generate/animation/templateprocessing.h"
    "include/generate/animation/templatetree.h"
    )
source_group("Header Files\\generate\\animation" FILES ${Header_Files__generate__animation})

set(Header_Files__ui
    "include/ui/AnimProgressBar.h"
    "include/ui/BehaviorInfo.h"
    "include/ui/BehaviorListModel.h"
    "include/ui/BehaviorListView.h"
    "include/ui/CmdLaunch.h"
    "include/ui/ErrorMsgBox.h"
    "include/ui/MessageHandler.h"
    "include/ui/MultiInstanceCheck.h"
    "include/ui/NemesisEngine.h"
    "include/ui/NemesisEngine.ui"
    "include/ui/ProgressUp.h"
    "include/ui/ScrollBar.h"
    "include/ui/SettingsSave.h"
    "include/ui/Terminator.h"
    "include/ui/UiModInfo.h"
    )
source_group("Header Files\\ui" FILES ${Header_Files__ui})

set(Header_Files__update
    "include/update/dataunification.h"
    "include/update/functionupdate.h"
    "include/update/updateprocess.h"
    "include/update/animdata/animdatacond.h"
    "include/update/animdata/animdatapackcond.h"
    "include/update/animdata/animdataprojectcond.h"
    "include/update/animdata/infodatapackcond.h"
    )
source_group("Header Files\\update" FILES ${Header_Files__update})

set(Header_Files__update__animdata
    "include/update/animdata/animdatacond.h"
    "include/update/animdata/animdatapackcond.h"
    "include/update/animdata/animdataprojectcond.h"
    "include/update/animdata/infodatapackcond.h"
    )
source_group("Header Files\\update\\animdata" FILES ${Header_Files__update})

set(Header_Files__utilities
    "include/utilities/algorithm.h"
    "include/utilities/alphanum.hpp"
    "include/utilities/animationsconvert.h"
    "include/utilities/atomiclock.h"
    "include/utilities/bsasupport.h"
    "include/utilities/compute.h"
    "include/utilities/conditions.h"
    "include/utilities/crc32.h"
    "include/utilities/filechecker.h"
    "include/utilities/lastupdate.h"
    "include/utilities/linkedvar.h"
    "include/utilities/readtextfile.h"
    "include/utilities/regex.h"
    "include/utilities/renew.h"
    "include/utilities/scope.h"
    "include/utilities/stringsplit.h"
    "include/utilities/writetextfile.h"
    "include/utilities/threadpool.h"
    )
source_group("Header Files\\utilities" FILES ${Header_Files__utilities})

set(Source_Files
    "src/connector.cpp"
    "src/debuglog.cpp"
    "src/debugmsg.cpp"
    "src/externalscript.cpp"
    "src/Global.cpp"
    "src/main.cpp"
    "src/nemesisinfo.cpp"
    )
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__generate
    "src/generate/addanims.cpp"
    "src/generate/addevents.cpp"
    "src/generate/addvariables.cpp"
    "src/generate/alternateanimation.cpp"
    "src/generate/animationdata.cpp"
    "src/generate/animationdatatracker.cpp"
    "src/generate/animationsetdata.cpp"
    "src/generate/animationutility.cpp"
    "src/generate/animdatasubprocess.cpp"
    "src/generate/animsetdatasubprocess.cpp"
    "src/generate/behaviorcheck.cpp"
    "src/generate/behaviorgenerator.cpp"
    "src/generate/behaviorprocess.cpp"
    "src/generate/behaviorprocess_utility.cpp"
    "src/generate/behaviorsubprocess.cpp"
    "src/generate/generator_utility.cpp"
    "src/generate/installscripts.cpp"
    "src/generate/papyruscompile.cpp"
    "src/generate/playerexclusive.cpp"
    )
source_group("Source Files\\generate" FILES ${Source_Files__generate})

set(Source_Files__generate__animation
    "src/generate/animation/animationinfo.cpp"
    "src/generate/animation/animationthread.cpp"
    "src/generate/animation/animthreadinfo.cpp"
    "src/generate/animation/grouptemplate.cpp"
    "src/generate/animation/import.cpp"
    "src/generate/animation/newanimation.cpp"
    "src/generate/animation/nodejoint.cpp"
    "src/generate/animation/optionlist.cpp"
    "src/generate/animation/paired.cpp"
    "src/generate/animation/registeranimation.cpp"
    "src/generate/animation/singletemplate.cpp"
    "src/generate/animation/templateinfo.cpp"
    "src/generate/animation/templateprocessing.cpp"
    )
source_group("Source Files\\generate\\animation" FILES ${Source_Files__generate__animation})

set(Source_Files__ui
    "src/ui/AnimProgressBar.cpp"
    "src/ui/BehaviorInfo.cpp"
    "src/ui/BehaviorListModel.cpp"
    "src/ui/BehaviorListView.cpp"
    "src/ui/CmdLaunch.cpp"
    "src/ui/ErrorMsgBox.cpp"
    "src/ui/MessageHandler.cpp"
    "src/ui/MultiInstanceCheck.cpp"
    "src/ui/NemesisEngine.cpp"
    "src/ui/ProgressUp.cpp"
    "src/ui/ScrollBar.cpp"
    "src/ui/SettingsSave.cpp"
    "src/ui/Terminator.cpp"
    "src/ui/UiModInfo.cpp"
    )
source_group("Source Files\\ui" FILES ${Source_Files__ui})

set(Source_Files__update
    "src/update/dataunification.cpp"
    "src/update/functionupdate.cpp"
    "src/update/updateprocess.cpp"
    "src/update/animdata/animdatacond.cpp"
    "src/update/animdata/animdatapackcond.cpp"
    "src/update/animdata/animdataprojectcond.cpp"
    "src/update/animdata/infodatacond.cpp"
    )
source_group("Source Files\\update" FILES ${Source_Files__update})

set(Source_Files__update__animdata
    "src/update/animdata/animdatacond.cpp"
    "src/update/animdata/animdatapackcond.cpp"
    "src/update/animdata/animdataprojectcond.cpp"
    "src/update/animdata/infodatapackcond.cpp"
    )
source_group("Source Files\\update\\animdata" FILES ${Source_Files__update})

set(Source_Files__utilities
    "src/utilities/algorithm.cpp"
    "src/utilities/animationsconvert.cpp"
    "src/utilities/compute.cpp"
    "src/utilities/conditions.cpp"
    "src/utilities/crc32.cpp"
    "src/utilities/filechecker.cpp"
    "src/utilities/lastupdate.cpp"
    "src/utilities/linkedvar.cpp"
    "src/utilities/regex.cpp"
    "src/utilities/renew.cpp"
    "src/utilities/scope.cpp"
    "src/utilities/stringsplit.cpp"
    "src/utilities/threadpool.cpp"
    )
source_group("Source Files\\utilities" FILES ${Source_Files__utilities})

set(Resources_Files
    "resources/Nemesis Engine.rc"
    "resources/NemesisEngine.qrc"
    )

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__generate}
    ${Header_Files__generate__animation}
    ${Header_Files__ui}
    ${Header_Files__update}
    ${Header_Files__utilities}
    ${Source_Files}
    ${Source_Files__generate}
    ${Source_Files__generate__animation}
    ${Source_Files__ui}
    ${Source_Files__update}
    ${Source_Files__utilities}
    ${Resources_Files}
    )
################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})

################################################################################
# Include directories
################################################################################

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}")

#Windows API
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{UniversalCRTSdkDir}/Include/ucrt)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "UNICODE;"
    "_UNICODE;"
    "WIN32;"
    "WIN64"
    "_WIN32_WINNT=0x0601"
    "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING" #There are no alternatives
    )

################################################################################
# Copy Python DLL
################################################################################

message(STATUS "Copying Python DLL: ${Python_DLL_PATH}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${Python_DLL_PATH}
    ${CMAKE_BINARY_DIR}/${Python_DLL_NAME})

################################################################################
# Compile and link options
################################################################################

#C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(MSVC)
    #Strictly follow the standard
    target_compile_options(${PROJECT_NAME} PRIVATE "/permissive-")
    #Required
    target_compile_options(${PROJECT_NAME} PRIVATE "/bigobj")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE BoostNemesis PythonNemesis Qt5Nemesis hkxcmd#add_subdirectory(external/hkxcmd)
)
