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
target_link_libraries(BoostNemesis INTERFACE Boost::thread Boost::regex)
