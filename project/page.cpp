#include "page.h"

#include "config.h"

#include <QFile>
#include <QTextStream>
#include <QPixmapCache>

Page::Page(const Path *path)
    : m_path(path)
{
	Q_ASSERT(path != nullptr);

	// load text
	QFile file(m_path->text());
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		m_text = stream.readAll();
	}
}

QString Page::plainText() const { return m_text; }

void Page::setPlainText(const QString &text) { m_text = text; }

int Page::index() const { return m_path->index(); }

QPixmap Page::image() const {
    QPixmap pix;
	QString img = m_path->image();
    if (!QPixmapCache::find(img, &pix)) {
        pix.load(img);
        QPixmapCache::insert(img, pix);
    }
    return pix;
}

void Page::save()
{
	QFile file(m_path->text());
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		return;
	}
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	stream << plainText();
}

void Page::deleteImage()
{
}
