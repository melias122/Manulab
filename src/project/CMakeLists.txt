add_library(project
	config.cpp
	config.h
	page.cpp
	page.h
	pages.cpp
	pages.h
	path.cpp
	path.h)

target_link_libraries(project PRIVATE Qt5::Widgets)

add_clangformat(project)
