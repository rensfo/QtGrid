cmake_minimum_required(VERSION 3.20)
project(QGrid LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets LinguistTools)

set(QGRID_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(QGRID_PUBLIC_HEADERS
    ${QGRID_INCLUDE_DIR}/QGrid/GridView.h
    ${QGRID_INCLUDE_DIR}/QGrid/GridColumn.h
    ${QGRID_INCLUDE_DIR}/QGrid/ActiveListBase.h
)

add_library(QGrid SHARED
    ${QGRID_PUBLIC_HEADERS}

    ${CMAKE_CURRENT_SOURCE_DIR}/GridView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GridColumn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ColumnsSerializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderTableFilterWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderTableFilterWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderTableView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderTableView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticFilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticFilterItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticFilterItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TableView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TableView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TableViewBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TableViewBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Filter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Nullable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/NullableBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/NullableBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Operation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/delegates/HeaderTableDelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/delegates/HeaderTableDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/delegates/UniversalDelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/delegates/UniversalDelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/HeaderTableModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/HeaderTableModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/models/UniversalTableModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/models/UniversalTableModel.cpp
)

target_compile_definitions(QGrid PRIVATE QGRID_LIB)

target_include_directories(QGrid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QGrid
    PUBLIC
        Qt5::Core
        Qt5::Widgets
)

set_target_properties(QGrid PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${QGRID_PUBLIC_HEADERS}"
)


