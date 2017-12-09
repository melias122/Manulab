#include "worker.h"

#include "qdebug.h"

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

//        QList<Page> filtered;
//		for (Page &page : pages) {

//			QString text = page.plainText();
//			text = filter->process(text);

//			if (text != "") {
//				page.setPlainText(text);
//				filtered.append(page);
//			}
//		}

	}

    emit result(pages);
	emit finished();
}
