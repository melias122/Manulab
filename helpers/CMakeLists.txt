add_library(helpers
	imagepool.cpp)

target_include_directories(helpers PRIVATE ../)

target_link_libraries(helpers PRIVATE Qt5::Widgets)
