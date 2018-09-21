#ifndef FREQUENCE_H
#define FREQUENCE_H

#include "interface.h"

#include <QButtonGroup>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QTableWidget>

class Frequency : public FilterInterface, public QObject {
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
	void set_abs();
	void set_rel();

private:
	QTableWidget table;
	QHeaderView *header_view;
	QLineEdit le_file_path;
	QPalette pallete;
	QPushButton btn_open_file_browser;
	QPushButton btn_export_data;
	QButtonGroup btn_grp;
	QRadioButton btn_option_rel;
	QRadioButton btn_option_abs;
	QPushButton btn_show_graph; //button declaration

	float total;
	QMap<QString, quint32> data;
	qint32 ngram_len;
	bool sort_asc;
	QString delimitter;

	static QString arg1;
	static QString arg2;

	bool is_abs;

	class Graph : public QWidget //widget-window and graph declaration
	{
	public:
		Graph(const QMap<QString, quint32> &data, bool is_abs);
		void paintEvent(QPaintEvent *event) override;

	private:
		void getMaxOffset(QPainter &painter);
		void getMaxWidth(QPainter &painter);
		void drawAxis(QPainter &painter);
		void drawBars(QPainter &painter);
		QBrush barColor;
		QMap<QString, quint32> data;
		QMap<QString, float> data_float;
		quint32 max;
		float max_float;
		bool is_abs;
		int max_offset;
		int max_width;
		int offsets[10];
	};
};

#endif // FREQUENCE_H
