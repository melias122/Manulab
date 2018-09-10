#ifndef SEARCH_H
#define SEARCH_H

#include "interface.h"

#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QGridLayout>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPair>
#include <QTableWidget>
#include <QHeaderView>

class Search : public FilterInterface, public QObject
{
public:
	Search();
    QList<Page> processPages(QList<Page> &pages) override;
	QString process(QString &text) override;
	void settingsUi(QWidget *parent) override;
    void resultUi(QWidget *parent) override;
    void reset() override;

private slots:
    QString export_result(void);
    void open_file_browser(void);
    void sort_by_colum(int column);

private:
    QTableWidget table;
    QHeaderView *header_view;
    QLineEdit le_file_path;
    QPalette pallete;
    QPushButton btn_open_file_browser;
    QPushButton btn_export_data;
    bool sort_asc;

	static QString arg1;
    QList<QPair<quint32, quint32>> data; // page/line

};

#endif // SEARCH_H
