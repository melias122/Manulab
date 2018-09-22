add_subdirectory(filter)
add_subdirectory(helpers)
add_subdirectory(project)
add_subdirectory(ui)

add_executable(manulab
	main.cpp
	resources/resources.qrc)

target_link_libraries(manulab
	Qt5::Widgets
	filter
	helpers
	project
	ui)

add_clangformat(manulab)
