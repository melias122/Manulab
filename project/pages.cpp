#include "pages.h"

#include "filter/worker.h"

#include <QFileDialog>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

struct Preload {
	Preload(int index, int max = 5) : index(index), max(max) {}

	void operator()(Page &page)
	{
		if (abs(index - page.index()) < max) {
			page.image();
		} else {
			page.deleteImage();
		}
	}

	int index;
	int max;
};

Pages::Pages()
{
	connect(this, &Pages::preload, this, &Pages::on_preload, Qt::QueuedConnection);
}

void Pages::next() { setIndex(m_index + 1); }

void Pages::prev() { setIndex(m_index - 1); }

void Pages::setIndex(int index)
{
	if (!m_config || m_pages.size() == 0) {
		return;
	}

	if (index < 0 || index >= m_pages.size()) {
		return;
	}

	m_index = index;

	emit pageChanged(m_pages[m_index]);
	emit info("Page: " + QString::number(m_index + 1) + "/" + QString::number(m_pages.size()));
	emit preload(m_index);
}

void Pages::save()
{
	if (!m_config) {
		return;
	}

	for (Page &page : m_pages) {
		page.save();
	}

	QString success = "successfuly";
	if (!m_config->save()) {
		success = "not " + success;
	}
	info("Project " + success + " saved");
}

void Pages::on_executeFilters(const QList<Filter> &filters)
{
	if (!m_config) {
		info("No project");
		return;
	}

	// prepare pages for for filtering
	// start with original(non-modified) content
	// every run. This mean that we load Pages(without images)
	// each time.
	QList<Page> pages = m_config->getPages();

	// create worker
	QThread *t = new QThread;
	worker *w = new worker(pages, filters);
	w->moveToThread(t);

	connect(t, &QThread::started, w, &worker::process);
	connect(t, &QThread::finished, t, &QThread::deleteLater);

	connect(w, &worker::finished, t, &QThread::quit);
	connect(w, &worker::finished, w, &worker::deleteLater);
	connect(w, &worker::result, this, &Pages::on_workerFinished);

	// start worker
	t->start();
}

void Pages::reload()
{
	if (!m_config) {
		return;
	}
	if (m_pagePreloadWatcher->isRunning()) {
		m_pagePreloadWatcher->cancel();
	}
	m_pagePreloadWatcher->waitForFinished();
	setPages(m_config->getPages());
	setIndex(0);
}

void Pages::on_configChanged(Config *config)
{
	m_config = config;
	reload();
}

void Pages::on_plainText_changed(const QString &text)
{
	if (!m_pages.empty()) {
		m_pages[m_index].setPlainText(text);
	}
}

void Pages::on_workerFinished(const QList<Page> &pages)
{
    //setPages(pages);
    //setIndex(0);
	info("Filtering done");
}

void Pages::on_preload(int index)
{
//    m_pagePreloadWatcher->waitForFinished();
//    m_pagePreloadWatcher->setFuture(QtConcurrent::map(m_pages, Preload(index, m_maxPagePreload)));
}

void Pages::setPages(const QList<Page> &pages) { m_pages = pages; }
