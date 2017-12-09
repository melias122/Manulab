#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include "qlistwidget.h"
#include "filter/filter.h"

class FilterManager : public QListWidget
{
	Q_OBJECT
public:
	FilterManager(QWidget *parent = 0);
	void setFilters(const QList<Filter> &filters);

signals:
	void filterInserted(int index, const QString &name);
	void filterEdited(int index);
	void filterRemoved(int index);

public slots:
	QList<Filter> filters() const { return mFilters; }
	void on_addFilter(const QString &filter) { insertFilter(filter); }
	void on_removeAllFilters();

private slots:
	void showContextMenuForFilter(const QPoint &pos);
	void on_itemDoubleClicked(QListWidgetItem *item);
	void on_removeShortcut_activated();

protected:
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void startDrag(Qt::DropActions);

private:
	void insertFilter(const QString &name);
	void removeFilter(int index);

private:
	QList<Filter> mFilters;
};

#endif // FILTERMANAGER_H
