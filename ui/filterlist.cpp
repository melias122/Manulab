#include "filterlist.h"

#include <QDebug>
#include <QDrag>
#include <QLabel>
#include <QMimeData>
#include <QtEvents>

#include "filter/filter.h"

FilterList::FilterList(QWidget *parent)
	: QListWidget(parent)
{
	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setIconSize(QSize(height(), height()));
	setSpacing(10);
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	QStringList filters = FilterFactory::filters();
	for (const QString &filter : filters) {
		addFilter(filter);
	}
	sortItems();

	//	itemDoubleClicked();
	connect(this, &FilterList::itemDoubleClicked, this, &FilterList::on_itemDoubleClicked);
}

void FilterList::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("manulab/x-filter")) {
		event->accept();
	} else {
		event->ignore();
	}
}

void FilterList::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat("manulab/x-filter")) {
		event->setDropAction(Qt::MoveAction);
		event->accept();
	} else {
		event->ignore();
	}
}

void FilterList::dropEvent(QDropEvent *event)
{
	event->ignore();
}

void FilterList::addFilter(QString name)
{
	QListWidgetItem *item = new QListWidgetItem(this);
	item->setText(name);
	item->setData(Qt::UserRole, QVariant(name));
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	addItem(item);
}

void FilterList::startDrag(Qt::DropActions /*supportedActions*/)
{
	QListWidgetItem *item = currentItem();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QString filter = item->data(Qt::UserRole).toString();
	dataStream << filter;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("manulab/x-filter", itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);

	//drag->setPixmap(pixmap);
	drag->exec(Qt::MoveAction);
}

void FilterList::on_itemDoubleClicked(QListWidgetItem *item)
{
	emit addFilterToManager(item->data(Qt::UserRole).toString());
}
