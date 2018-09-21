#include "filter.h"

#include "distance.h"
#include "entropy.h"
#include "frequency.h"
#include "indexofcoincidence.h"
#include "orientation.h"
#include "search.h"
#include "select.h"
#include "substitution.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

template <typename filter>
FilterInterface *newFilter() { return new filter; }

// TODO(m): Add registry
QHash<QString, FilterFactory::NewFilter> FilterFactory::factory = {
	{ "Sub-selection", &newFilter<Select> },
	{ "Page orientation", &newFilter<Orientation> },
	{ "Substitution", &newFilter<Substitution> },
	{ "Frequency", &newFilter<Frequency> },
	{ "IC", &newFilter<IndexOfCoincidence> },
	{ "Pattern search", &newFilter<Search> },
	{ "Distances", &newFilter<Distance> },
	{ "Entropy", &newFilter<Entropy> }
};

Filter FilterFactory::create(const QString &name)
{
	return Filter(ptr_create(name));
}

QStringList FilterFactory::filters()
{
	return factory.keys();
}

FilterInterface *FilterFactory::ptr_create(const QString &name)
{
	FilterInterface *filter = factory[name]();
	filter->Name = name;
	return filter;
}

QByteArray filtersToJson(const QList<Filter> &filters)
{
	QJsonDocument document;
	QJsonArray array;

	for (const Filter &filter : filters) {
		array.append(filter->toJson());
	}

	document.setArray(array);

	return document.toJson();
}

QList<Filter> filtersFromJson(const QByteArray &json)
{
	QJsonDocument document = QJsonDocument::fromJson(json);

	QList<Filter> filters;

	for (const QJsonValueRef &ref : document.array()) {
		filters.append(Filter(FilterInterface::fromJson(ref.toObject())));
	}

	return filters;
}
