#include "interface.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

#include "filter.h"

FilterInterface* FilterInterface::fromJson(const QJsonObject &o)
{
	qDebug() << "FilterInterface::fromJson: reading" << o;

	FilterInterface *filter = FilterFactory::ptr_create(o["name"].toString());

	if (!filter) {
		return 0;
	}

	filter->uiFlag = UiFlag(o["uiFlag"].toInt());
	filter->setArgs(o["argv"].toObject().toVariantHash());

	return filter;
}

QJsonObject FilterInterface::toJson()
{
	QJsonObject o;

	o["name"] = Name;
	o["uiFlag"] = uiFlag;
	o["argv"] = QJsonObject::fromVariantHash(argv);

	return o;
}

QList<Page> FilterInterface::processPages(QList<Page> &pages){
    for (Page &page : pages) {

        QString text = page.plainText();
        text = process(text);

        if (text != "") {
            page.setPlainText(text);
        }
    }
    return pages;
}
