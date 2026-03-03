cmake_minimum_required(VERSION 3.16)

project(Feroxills VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appFeroxills
    src/main.cpp
    src/core/FBuffer.cpp
    src/core/FBuffer.h
        src/Backend.cpp
        src/Backend.h
        src/ui/DisplayMode.h
        src/ui/mode/Continu.cpp
        src/ui/mode/Continu.h
        src/ui/mode/Trigger.cpp
        src/ui/mode/Trigger.h
        src/ui/mode/Auto.cpp
        src/ui/mode/Auto.h
        src/ui/DisplayContext.h
        src/core/analyser/SamplesAnalyser.cpp
        src/core/analyser/SamplesAnalyser.h
        src/core/event/EventBus.cpp
        src/core/event/EventBus.h
        src/core/debug/debug.cpp
        src/core/debug/debug.h
)

qt_add_qml_module(appFeroxills
    URI Feroxills
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        SOURCES src/core/FBuffer.h
        QML_FILES
        QML_FILES
        QML_FILES qml/FerroGridPannel.qml
        QML_FILES qml/FerroHeaderPannel.qml
        QML_FILES qml/FerroRightPannel.qml
        QML_FILES qml/FerroFooterPannel.qml
        QML_FILES qml/components/FerroSlider.qml
        QML_FILES qml/components/FerroOneValuePopup.qml
        RESOURCES
           qml/font/SAKUNA.ttf
           qml/font/Paper_Scratch.otf
           qml/font/SPORT_GAME_PERSONAL_USE_ONLY!.ttf
           QML_FILES qml/FerroComboxMenu.qml
           SOURCES src/core/signalgenerator.h src/core/signalgenerator.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFeroxills PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFeroxills
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFeroxills
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appFeroxills
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
