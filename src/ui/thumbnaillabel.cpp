#include "ui/thumbnaillabel.h"

#include <QPixmapCache>

thumbnailLabel::thumbnailLabel(QWidget *parent)
	: QLabel(parent)
{
	setVisible(false);
}

thumbnailLabel::~thumbnailLabel()
{
}

void thumbnailLabel::setImage(const QString &image)
{
	QPixmap pix;
	QString key = image + QString::number(m_witdth) + "-" + QString::number(m_heigth);

	if (!QPixmapCache::find(key, &pix)) {
		pix.load(image);
		pix = pix.scaled(m_witdth, m_heigth, Qt::KeepAspectRatio, Qt::FastTransformation);
		QPixmapCache::insert(key, pix);
	}
	setPixmap(pix);
}
