find_package(Python COMPONENTS Development REQUIRED)

add_library(PythonNemesis INTERFACE)

target_include_directories(PythonNemesis INTERFACE "${Python_INCLUDE_DIRS}")
target_link_libraries(PythonNemesis INTERFACE ${Python_LIBRARIES})

set(Python_DLL_NAME Python${Python_VERSION_MAJOR}${Python_VERSION_MINOR}.dll)
set(Python_DLL_PATH ${Python_RUNTIME_LIBRARY_DIRS}/${Python_DLL_NAME})
