#ifndef WORKER_H
#define WORKER_H

#include "qobject.h"

#include "filter/filter.h"
#include "project/page.h"

class worker : public QObject
{
	Q_OBJECT
public:
	explicit worker(const QList<Page> &pages, const QList<Filter> &filters);

signals:
	void finished();
	void result(const QList<Page> &filtered);

public slots:
	void process();

private:
	QList<Page> pages;
	QList<Filter> filters;
};

#endif // WORKER_H
