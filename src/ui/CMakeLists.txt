add_library(ui
	ListView.cpp
	ListView.h
	filterlist.cpp
	filterlist.h
	filtermanager.cpp
	filtermanager.h
	imageviewer.cpp
	imageviewer.h
	mainwindow.cpp
	mainwindow.h
	newproject.cpp
	newproject.h
	texteditor.cpp
	texteditor.h
	thumbnaillabel.cpp
	thumbnaillabel.h
	thumbnailslider.cpp
	thumbnailslider.h)

target_link_libraries(ui PRIVATE Qt5::Widgets)

add_clangformat(ui)
