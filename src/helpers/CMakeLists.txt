add_library(helpers
	imagepool.cpp
	imagepool.h)

target_link_libraries(helpers PRIVATE Qt5::Widgets)

add_clangformat(helpers)
