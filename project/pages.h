#ifndef PAGES_H
#define PAGES_H

#include <QFutureWatcher>

#include "config.h"
#include "page.h"
#include "filter/filter.h"

class Pages : public QObject
{
	Q_OBJECT
public:
	explicit Pages();

signals:
	void pageChanged(const Page &page);
	void info(const QString &info);
	void preload(int index);

public slots:
	void next();
	void prev();
	void setIndex(int index);
	void save();
	void on_executeFilters(const QList<Filter> &filters);
	void reload();

	// initializes Pages with config
	void on_configChanged(Config *config);
	void on_plainText_changed(const QString &text);

private slots:
	void on_workerFinished(const QList<Page> &pages);
	void on_preload(int index);

private:
	void setPages(const QList<Page> &pages);

private:
	int m_index{0};
	Config *m_config{0};

	int m_maxPagePreload{5};
	QFutureWatcher<void> *m_pagePreloadWatcher{new QFutureWatcher<void>};
	QList<Page> m_pages;
};

#endif  // PAGES_H
