#include "filter/filter.h"

#include "filter/frequency.h"
//#include "filter/reverse.h"
#include "filter/search.h"
#include "filter/substitution.h"
#include "filter/orientation.h"
#include "filter/select.h"
#include "filter/coincidenceindex.h"
#include "filter/distance.h"
#include "filter/entropy.h"

#include "qjsondocument.h"
#include "qjsonarray.h"
#include "qjsonobject.h"

template <typename filter> FilterInterface *newFilter() { return new filter; }

QHash<QString, FilterFactory::NewFilter> FilterFactory::factory = {
    {"Sub-selection", &newFilter<Select>},
    {"Page orientation", &newFilter<Orientation>},
    {"Substitution", &newFilter<Substitution>},
    {"Frequency", &newFilter<Frequency>},
//	{"Reverse", &newFilter<Reverse>},
    {"IC", &newFilter<CoincidenceIndex>},
    {"Pattern search", &newFilter<Search>},
    {"Distances", &newFilter<Distance>},
    {"Entropy", &newFilter<Entropy>}

};

Filter FilterFactory::create(const QString &name)
{
	return Filter(ptr_create(name));
}

QStringList FilterFactory::filters()
{
	return factory.keys();
}

FilterInterface * FilterFactory::ptr_create(const QString &name)
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
