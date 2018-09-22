add_library(filter
	distance.cpp
	distance.h
	entropy.cpp
	entropy.h
	filter.cpp
	filter.h
	frequency.cpp
	frequency.h
	indexofcoincidence.cpp
	indexofcoincidence.h
	interface.cpp
	interface.h
	orientation.cpp
	orientation.h
	reverse.cpp
	reverse.h
	search.cpp
	search.h
	select.cpp
	select.h
	settingsDialog.cpp
	settingsDialog.h
	substitution.cpp
	substitution.h
	worker.cpp
	worker.h)

target_link_libraries(filter PRIVATE Qt5::Widgets)

add_clangformat(filter)
