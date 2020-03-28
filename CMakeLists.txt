cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(NemesisUnlimitedBehaviorEngine LANGUAGES CXX VERSION 0.84)

################################################################################
# VARIABLES
################################################################################
# This part is the only one that should be modified by the user wanting to compile Nemesis

set(Qt5_DIR "C:/IT/Qt/5.12.4/msvc2017/lib/cmake/Qt5")
set(Python_DIR "C:/IT/Python-3.5.6")
set(BOOST_ROOT "C:/IT/Boost_1.72.0" CACHE PATH "Boost library path" )

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Required packages
################################################################################

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${Boost_DIR})

set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )
set(Boost_USE_MULTITHREADED ON)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/boost)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib32-msvc-14.2)


find_package (Boost 1.72.0 REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

################################################################################
# Custom target for Python
################################################################################
add_library(Python STATIC IMPORTED)

#Finding 32 or 64bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    message("Nemesis: using 64bits Python")
    set(Python_LIB_DIR "${Python_DIR}/PCbuild/amd64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    message("Nemesis: using 32bits Python")
    set(Python_LIB_DIR "${Python_DIR}/PCbuild/win32")
endif()

set_target_properties(Python PROPERTIES IMPORTED_LOCATION_DEBUG "${Python_LIB_DIR}/python35_d.lib")
set_target_properties(Python PROPERTIES IMPORTED_LOCATION "${Python_LIB_DIR}/python35.lib")

################################################################################
# Qt support
################################################################################
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
#Ressources
set(CMAKE_AUTORCC ON)

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
    "include/generate/playerexclusive.h"
    )

source_group("Header Files\\generate" FILES ${Header_Files__generate})

set(Header_Files__generate__animation
    "include/generate/animation/animationinfo.h"
    "include/generate/animation/animationthread.h"
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
    )
source_group("Header Files\\ui" FILES ${Header_Files__ui})

set(Header_Files__update
    "include/update/dataunification.h"
    "include/update/functionupdate.h"
    "include/update/updateprocess.h"
    )
source_group("Header Files\\update" FILES ${Header_Files__update})

set(Header_Files__utilities
    "include/utilities/algorithm.h"
    "include/utilities/alphanum.hpp"
    "include/utilities/atomiclock.h"
    "include/utilities/bsasupport.h"
    "include/utilities/compute.h"
    "include/utilities/filechecker.h"
    "include/utilities/lastupdate.h"
    "include/utilities/readtextfile.h"
    "include/utilities/renew.h"
    "include/utilities/stringsplit.h"
    "include/utilities/writetextfile.h"
    "include/utilities/wstrconvert.h"
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
    "src/generate/playerexclusive.cpp"
    )
source_group("Source Files\\generate" FILES ${Source_Files__generate})

set(Source_Files__generate__animation
    "src/generate/animation/animationinfo.cpp"
    "src/generate/animation/animationthread.cpp"
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
    )
source_group("Source Files\\ui" FILES ${Source_Files__ui})

set(Source_Files__update
    "src/update/dataunification.cpp"
    "src/update/functionupdate.cpp"
    "src/update/updateprocess.cpp"
    )
source_group("Source Files\\update" FILES ${Source_Files__update})

set(Source_Files__utilities
    "src/utilities/algorithm.cpp"
    "src/utilities/compute.cpp"
    "src/utilities/filechecker.cpp"
    "src/utilities/lastupdate.cpp"
    "src/utilities/renew.cpp"
    "src/utilities/stringsplit.cpp"
    "src/utilities/wstrconvert.cpp"
    )
source_group("Source Files\\utilities" FILES ${Source_Files__utilities})

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
    )

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

################################################################################
# Include directories
################################################################################

#Ui
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}")

target_include_directories(${PROJECT_NAME} PUBLIC "${BOOST_ROOT}")
target_include_directories(${PROJECT_NAME} PUBLIC "${BOOST_INCLUDEDIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${Qt5Core_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PUBLIC "${Qt5Widgets_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME} PUBLIC "${Python_DIR}/Include")
target_include_directories(${PROJECT_NAME} PUBLIC "${Python_DIR}/PC")

target_link_directories(${PROJECT_NAME} PUBLIC "${BOOST_LIBRARYDIR}")
target_link_directories(${PROJECT_NAME} PUBLIC "${Boost_DIR}/lib32-msvc-14.2")
target_link_directories(${PROJECT_NAME} PUBLIC "${Qt5Core_LIBRARIES}")
target_link_directories(${PROJECT_NAME} PUBLIC "${Qt5Widgets_LIBRARIES}")

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "UNICODE;"
    "_UNICODE;"
    "WIN32;"
    "WIN64"
    )

################################################################################
# Compile and link options
################################################################################

#C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#Strictly follow the standard
target_compile_options(${PROJECT_NAME} PRIVATE "/permissive-")

if(MSVC)
    #Required
    target_compile_options(${PROJECT_NAME} PRIVATE "/bigobj")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Python Qt5::Core Qt5::Widgets)
