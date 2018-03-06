#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "filter/interface.h"
#include "filter/worker.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QGridLayout>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QPainter>
#include <QScrollArea>

class Frequency : public FilterInterface, public QObject
{
public:
    Frequency();
    void reset() override;
	void setArgs(const Args &args) override;
    QList<Page> processPages(QList<Page> &pages) override;
	QString process(QString &text) override;

	void settingsUi(QWidget *parent) override;
	void resultUi(QWidget *parent) override;

private slots:
    QString export_histogram(void);
    void open_file_browser(void);
    void sort_by_colum(int column);
    void show_graph(void); //show_graph declaration

private:
    QTableWidget table;
    QHeaderView *header_view;
    QLineEdit le_file_path;
    QPalette pallete;
    QPushButton btn_open_file_browser;
    QPushButton btn_export_data;
    QPushButton btn_show_graph;//button declaration

    float total;
    QMap<QString, quint32> data;
    qint32 ngram_len;
    bool sort_asc;
    QString delimitter;

	static QString arg1;
    static QString arg2;

    class Graph : public QWidget//widget-window and graph declaration
    {
    public:
        Graph(const QMap<QString, quint32>& data);
        void paintEvent(QPaintEvent *event) override;
    private:
        void drawAxis(QPainter& painter);
        void drawBars(QPainter& painter);
        QBrush barColor;
        QMap<QString, quint32> data;
        quint32 max;
    };



};
#endif // FREQUENCE_H
