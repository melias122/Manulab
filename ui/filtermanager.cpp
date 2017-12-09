#include "ui/filtermanager.h"

#include "qevent.h"
#include "qmimedata.h"
#include "qmenu.h"
#include "qshortcut.h"
#include "qdebug.h"

#include "filter/filter.h"

FilterManager::FilterManager(QWidget *parent)
	: QListWidget(parent)
{
	setViewMode(QListView::IconMode);
	setIconSize(QSize(width(), width()));
	setSpacing(10);
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	setSelectionMode(QAbstractItemView::SingleSelection);
	setMovement(QListView::Free);
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
//	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &FilterManager::customContextMenuRequested, this, &FilterManager::showContextMenuForFilter);
	connect(this, &FilterManager::itemDoubleClicked, this, &FilterManager::on_itemDoubleClicked);

	QShortcut *remove = new QShortcut(this);
	remove->setKey(Qt::Key_Delete);
	connect(remove, &QShortcut::activated, this, &FilterManager::on_removeShortcut_activated);
}

void FilterManager::setFilters(const QList<Filter> &filters)
{
	// remove filters
	while (mFilters.size() > 0) {
		removeFilter(0);
	}

	for (const Filter &filter : filters) {
		QListWidgetItem *item = new QListWidgetItem(this);
		item->setText(filter->Name);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

		// add filter to list
		addItem(item);

		// add filter
		mFilters.append(filter);
	}
}

void FilterManager::on_removeAllFilters()
{
	while (mFilters.size() > 0) {
		removeFilter(0);
	}
}

void FilterManager::showContextMenuForFilter(const QPoint &pos)
{
	auto item = itemAt(pos);
	// check we really clicked on item
	if (item == nullptr) {
		return;
	}

	Filter filter = mFilters[row(item)];

	QMenu contextMenu("Context menu", this);

	QAction remove("Remove", this);
	connect(&remove, &QAction::triggered, [=](){ removeFilter(row(item)); });
	contextMenu.addAction(&remove);

	QAction edit("Edit", this);
	if (filter->hasUi(FilterInterface::UiFlag::settings)) {
		connect(&edit, &QAction::triggered, [=](){
			mFilters[row(item)]->settingsUi(this);
		});
		contextMenu.addAction(&edit);
	}

	QAction result("Result", this);
	if (filter->hasUi(FilterInterface::UiFlag::result)) {
		connect(&result, &QAction::triggered, [=](){
			mFilters[row(item)]->resultUi(this);
		});
		contextMenu.addAction(&result);
	}

	contextMenu.exec(mapToGlobal(pos));
}

void FilterManager::on_itemDoubleClicked(QListWidgetItem *item)
{
	Filter filter = mFilters[row(item)];

	if (filter->hasUi(FilterInterface::UiFlag::settings)) {
		filter->settingsUi(this);
	}
}

void FilterManager::on_removeShortcut_activated()
{
	QListWidgetItem *item = currentItem();
	if (!item) {
		return;
	}
	removeFilter(row(item));
}

void FilterManager::dropEvent(QDropEvent *event)
{
	if (!event->mimeData()->hasFormat("manulab/x-filter")) {
		event->ignore();
		return;
	}

	// get data, data is just QString with filter name
	QByteArray data = event->mimeData()->data("manulab/x-filter");
	QDataStream dataStream(&data, QIODevice::ReadOnly);

	// read filter name
	QString filter;
	dataStream >> filter;

	// TODO: insert at index where filter was droped
	insertFilter(filter);
}

void FilterManager::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("manulab/x-filter")) {
		event->accept();
	}
}

void FilterManager::dragMoveEvent(QDragMoveEvent *event)
{}

void FilterManager::startDrag(Qt::DropActions)
{
	// TODO: support filter reordering
}

void FilterManager::insertFilter(const QString &name)
{
	QListWidgetItem *item = new QListWidgetItem(this);
	item->setText(name);
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

	// add filter to list
	addItem(item);

	// create filter
	mFilters << FilterFactory::create(name);
}

void FilterManager::removeFilter(int index)
{
	// according to qt docs, we need manualy free memory
	delete takeItem(index);
	mFilters.removeAt(index);
}
