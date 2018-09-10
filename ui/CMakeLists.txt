add_library(ui
	ListView.cpp
	filterlist.cpp
    filtermanager.cpp
	imageviewer.cpp
	mainwindow.cpp
    newproject.cpp
    texteditor.cpp
    thumbnaillabel.cpp
    thumbnailslider.cpp)

target_include_directories(ui PRIVATE ..)

target_link_libraries(ui PRIVATE Qt5::Widgets)
