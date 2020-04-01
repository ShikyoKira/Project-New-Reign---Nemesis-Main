#[[SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${Boost_DIR})

set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )
set(Boost_USE_MULTITHREADED ON)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/boost)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib32-msvc-14.2)

target_include_directories(${PROJECT_NAME} PUBLIC "${BOOST_ROOT}")
target_include_directories(${PROJECT_NAME} PUBLIC "${BOOST_INCLUDEDIR}")

target_link_directories(${PROJECT_NAME} PUBLIC "${BOOST_LIBRARYDIR}")
target_link_directories(${PROJECT_NAME} PUBLIC "${Boost_DIR}/lib32-msvc-14.2")
]]

include (FetchContent)

if(${UseLocalBoost})
    message("Boost: Using local copy of Boost at ${BOOST_ROOT}")
    set(FETCHCONTENT_SOURCE_DIR_BOOST ${BOOST_ROOT})
endif()

FetchContent_Declare(
  BoostCMake
  GIT_REPOSITORY https://github.com/Guekka/boost-cmake.git
)

FetchContent_MakeAvailable(BoostCMake)

add_library(BoostNemesis INTERFACE)
target_link_libraries(BoostNemesis INTERFACE Boost::thread)
