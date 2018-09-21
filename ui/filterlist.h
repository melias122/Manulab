#ifndef FILTERLIST_H
#define FILTERLIST_H

#include "qlistwidget.h"

class FilterList : public QListWidget {
	Q_OBJECT

public:
	FilterList(QWidget *parent = 0);

signals:
	void addFilterToManager(const QString &filter);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void startDrag(Qt::DropActions);

private slots:
	void on_itemDoubleClicked(QListWidgetItem *item);

private:
	void addFilter(QString name);
};

#endif // FILTERLIST_H
