add_library(project
	config.cpp
    page.cpp
    pages.cpp
    path.cpp)

target_include_directories(project PRIVATE ../)

target_link_libraries(project PRIVATE Qt5::Widgets)
