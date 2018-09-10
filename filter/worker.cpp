#include "worker.h"

worker::worker(const QList<Page> &pages, const QList<Filter> &filters)
	: pages(pages)
	, filters(filters)
{}

void worker::process()
{
	for (const Filter &filter : filters) {
		// reset filter internal state
		filter->reset();
        pages = filter->processPages(pages);
	}

    emit result(pages);
	emit finished();
}
