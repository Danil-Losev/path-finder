cmake_minimum_required(VERSION 3.16)
project(lab2 LANGUAGES CXX)


file(GLOB SRC_FILES "engine/filters/*.cpp" "engine/filters/*.h" "engine/generators/*.h" "engine/generators/*.cpp" 
            "engine/binary/*.h" "engine/binary/*.cpp" "engine/searcher/*.h" "engine/searcher/*.cpp"
             "engine/painter/*.h" "engine/painter/*.cpp" "ui/*.h" "ui/*.cpp")


include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    MainWindow.h
    MainWindow.cpp

    ${SRC_FILES}
)

qt_add_resources(MY_APP_RESOURCES resources.qrc)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_sources(${PROJECT_NAME} PRIVATE ${MY_APP_RESOURCES})
