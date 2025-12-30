set(GRID_EXAMPLE_SOURCES
    main.cpp
    MainWindow.cpp
)

set(GRID_EXAMPLE_HEADERS
    MainWindow.h
)

set(GRID_EXAMPLE_UI_FILES
    MainWindow.ui
)

add_executable(GridExample
    ${GRID_EXAMPLE_SOURCES}
    ${GRID_EXAMPLE_HEADERS}
    ${GRID_EXAMPLE_UI_FILES}
)

target_link_libraries(GridExample
    PRIVATE
        QGrid
        Qt5::Core
        Qt5::Widgets
)


