cmake_minimum_required(VERSION 3.20)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Designer)

add_library(QGridDesignerPlugin MODULE
    QGridDesignerPlugin.h
    QGridDesignerPlugin.cpp
    QGridDesignerPluginExport.h
    ColumnsPropertyEditor.h
    ColumnsPropertyEditor.cpp
    ColumnsActiveList.h
    ColumnsActiveList.cpp
)

target_compile_definitions(QGridDesignerPlugin PRIVATE QGRIDDESIGNERPLUGIN_LIB)

target_include_directories(QGridDesignerPlugin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(QGridDesignerPlugin
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Designer
        QGrid
)

add_dependencies(QGridDesignerPlugin QGrid)

set_target_properties(QGridDesignerPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/designer
)

if(WIN32)
    set_target_properties(QGridDesignerPlugin PROPERTIES
        PREFIX ""
    )
endif()
