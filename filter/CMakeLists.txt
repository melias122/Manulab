add_library(filter
	distance.cpp
    entropy.cpp
	filter.cpp
	frequency.cpp
    indexofcoincidence.cpp
    interface.cpp
	orientation.cpp
	reverse.cpp
	search.cpp
	select.cpp
	settingsDialog.cpp
    substitution.cpp
	worker.cpp)

target_include_directories(filter PRIVATE ../)

target_link_libraries(filter PRIVATE Qt5::Widgets)

add_clangformat(filter)
