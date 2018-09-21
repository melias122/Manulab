#include <QApplication>
#include <QObject>
#include <QThread>

#include "project/pages.h"
#include "ui/mainwindow.h"
#include "ui/newproject.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QApplication::setGraphicsSystem("raster");
#endif
	QApplication a(argc, argv);

	// init resources
	Q_INIT_RESOURCE(icons);

	// register types
	qRegisterMetaType<QList<Filter>>("QList<Filter>");
	qRegisterMetaType<QList<Page>>("QList<Page>");
	qRegisterMetaType<QList<Page>>("Page");

	// init pages controller
	Pages pages;

	// init mainwindow
	MainWindow w;
	w.show();

	// Pages connections
	QObject::connect(&w, &MainWindow::nextPage, &pages, &Pages::next);
	QObject::connect(&w, &MainWindow::previousPage, &pages, &Pages::prev);
	QObject::connect(&w, &MainWindow::jumpToPage, &pages, &Pages::setIndex);
	QObject::connect(&w, &MainWindow::saveProject, &pages, &Pages::save);
	QObject::connect(&w, &MainWindow::plainTextChanged, &pages, &Pages::on_plainText_changed);
	QObject::connect(&w, &MainWindow::executeFilters, &pages, &Pages::on_executeFilters);
	QObject::connect(&w, &MainWindow::reloadPages, &pages, &Pages::reload);
	QObject::connect(&pages, &Pages::info, &w, &MainWindow::setInfo);
	QObject::connect(&pages, &Pages::pageChanged, &w, &MainWindow::on_pageChanged);

	// NewProject connections
	QObject::connect(&w, &MainWindow::openProjectManager, [&w, &pages]() {
		NewProject project;
		QObject::connect(&project, &NewProject::configChanged, &w, &MainWindow::on_configChanged);
		QObject::connect(&project, &NewProject::configChanged, &pages, &Pages::on_configChanged);
		project.exec();
	});

	return a.exec();
}
