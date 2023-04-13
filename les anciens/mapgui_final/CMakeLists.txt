cmake_minimum_required(VERSION 2.16)
project(mapgui LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6
    REQUIRED COMPONENTS Core Gui Widgets
)

qt_add_executable(mapgui
    main.cpp
    mainwindow.cpp mainwindow.h
    view.cpp view.h
    vertexitem.h vertexitem.cpp
    graph.h
    vertex.h
    edge.h
    graph_dc_area.txt

)

set_target_properties(mapgui PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(mapgui PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# Resources:
set(resource_files
    "rotateleft.png"
    "rotateright.png"
    "zoomin.png"
    "zoomout.png"
)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)


qt_add_resources(mapgui "images"
    PREFIX
        "/"
    FILES
        ${resource_files}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.lib-debug)
    set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/.lib-debug)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
else() 
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.lib)
    set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/.lib)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
endif()
