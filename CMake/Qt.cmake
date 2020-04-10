find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_library(Qt5Nemesis INTERFACE)

target_include_directories(Qt5Nemesis INTERFACE "${Qt5Core_INCLUDE_DIRS}")
target_include_directories(Qt5Nemesis INTERFACE "${Qt5Widgets_INCLUDE_DIRS}")

target_link_directories(Qt5Nemesis INTERFACE "${Qt5Core_LIBRARIES}")
target_link_directories(Qt5Nemesis INTERFACE "${Qt5Widgets_LIBRARIES}")

target_link_libraries(Qt5Nemesis INTERFACE Qt5::Core Qt5::Widgets)

################################################################################
# Qt support
################################################################################
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
#Ressources
set(CMAKE_AUTORCC ON)
