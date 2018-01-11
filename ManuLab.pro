#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T08:27:37
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ManuLab
TEMPLATE = app

win32 {
    DEFINES += Q_COMPILER_INITIALIZER_LISTS
}

SOURCES += main.cpp \
    ui/mainwindow.cpp \
    ui/newproject.cpp \
    project/config.cpp \
    ui/imageviewer.cpp \
    ui/texteditor.cpp \
    project/pages.cpp \
    filter/search.cpp \
    filter/reverse.cpp \
    filter/substitution.cpp \
    filter/coincidenceindex.cpp \
    ui/filterlist.cpp \
    ui/filtermanager.cpp \
    filter/filter.cpp \
    filter/settingsDialog.cpp \
    filter/worker.cpp \
    project/page.cpp \
    project/path.cpp \
    ui/ListView.cpp \
    filter/orientation.cpp \
    filter/select.cpp \
    filter/interface.cpp \
    filter/frequency.cpp \
    ui/thumbnaillabel.cpp \
    ui/thumbnailslider.cpp \
    helpers/imagepool.cpp \
    filter/distance.cpp \
    filter/entropy.cpp

HEADERS += \
    ui/mainwindow.h \
    ui/newproject.h \
    project/config.h \
    ui/imageviewer.h \
    ui/texteditor.h \
    project/pages.h \
    filter/search.h \
    filter/reverse.h \
    filter/substitution.h \
    filter/filter.h \
    filter/interface.h \
    ui/filterlist.h \
    ui/filtermanager.h \
    filter/settingsDialog.h \
    filter/worker.h \
    filter/coincidenceindex.h \
    project/page.h \
    project/path.h \
    ui/ListView.h \
    filter/orientation.h \
    filter/select.h \
    filter/frequency.h \
    ui/thumbnaillabel.h \
    ui/thumbnailslider.h \
    helpers/imagepool.h \
    filter/distance.h \
    filter/entropy.h

FORMS +=    ui/mainwindow.ui \
	    ui/newproject.ui

CONFIG += c++11

RESOURCES += resources/icons.qrc
