cmake_minimum_required(VERSION 3.16)

project(Nemesis_App VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Qt6 6.9 REQUIRED COMPONENTS Quick ShaderTools Multimedia Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(HeaderFiles
    "include/AppConfig.h"
    "include/AppLauncher.h"
    "include/ModInfo.h"
    "include/ModModelHandler.h"
    )
source_group("Header Files" FILES ${HeaderFiles})

set(SourceFiles
    "src/AppConfig.cpp"
    "src/AppLauncher.cpp"
    "src/ModInfo.cpp"
    "src/ModModelHandler.cpp"
    )
source_group("Source Files" FILES ${SourceFiles})

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    "src/main.cpp"
)

qt_add_shaders(${PROJECT_NAME} "AppShaders"
    FILES
        "shaders/BlurEffect.frag"
        "shaders/BlurEffect.vert"
        "shaders/ProgressBarCorners.frag"
        "shaders/ProgressBarCorners.vert"
        "shaders/ProgressBarFill.frag"
        "shaders/ProgressBarFill.vert"
)

qt_add_resources(${PROJECT_NAME} "AppResources"
    FILES
        "resources/exit-button.png"
        "resources/logo-text.png"
        "resources/logo.png"
        "resources/icon.png"
)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        "qml/Main.qml"
        "qml/NemesisButton.qml"
        "qml/NemesisCheckBox.qml"
        "qml/NemesisScrollBar.qml"
    SOURCES
        ${HeaderFiles}
        ${SourceFiles}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
        Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")

    find_program(WINDEPLOYQT_EXE windeployqt6
        PATHS "$ENV{QtPath}/bin" "$ENV{QT6_INSTALL_DIR}/bin"
        NO_DEFAULT_PATH
        DOC "Path to windeployqt6.exe"
    )

    if(NOT WINDEPLOYQT_EXE)
        message(FATAL_ERROR "windeployqt6.exe not found. Please ensure it's in your system PATH or defined via QtPath/QT6_INSTALL_DIR environment variables, or update your CMake configuration.")
    endif()

    set(APP_EXE_PATH "$<TARGET_FILE:${PROJECT_NAME}>")

    get_filename_component(APP_BUILD_DIR "${APP_EXE_PATH}" DIRECTORY)

    # You have two main options:
    # Option A: Deploy into the *same* directory as the executable (common for dev/testing)
    # set(DEPLOY_DIR "${APP_BUILD_DIR}")

    # Option B: Deploy into a *separate* dedicated folder (better for final distribution)
    # Uncomment the following lines if you prefer a separate deployment folder:
    set(DEPLOY_ROOT "${CMAKE_BINARY_DIR}/Deploy") # Base for deployment, typically in build folder
    set(DEPLOY_DIR "${DEPLOY_ROOT}/${PROJECT_NAME}") # Create a subfolder for your app
    file(MAKE_DIRECTORY "${DEPLOY_DIR}") # Ensure the directory exists

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        # COMMAND ${WINDEPLOYQT_EXE} --dir "${DEPLOY_DIR}" ${APP_EXE_PATH}
        # The --qmldir argument is useful for Qt Quick projects with QML files
        COMMAND ${WINDEPLOYQT_EXE} --dir "${DEPLOY_DIR}" --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/qml" ${APP_EXE_PATH}
        COMMENT "Running windeployqt6 to prepare deployment files for ${PROJECT_NAME}"
    )
elseif(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        BUNDLE DESTINATION .
    )
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.png"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/resources/" # Or specific sizes, e.g., 128x128
        RENAME "icon.png" # Rename to match the Icon field in .desktop
    )

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/resources/app.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/resources/app.desktop" @ONLY)

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/resources/app.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME Nemesis)
endif()
