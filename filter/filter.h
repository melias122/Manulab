#ifndef FILTER
#define FILTER

#include "interface.h"

#include <memory>

using Filter = std::shared_ptr<FilterInterface>;

class FilterFactory
{
	friend class FilterInterface;

public:
	static Filter create(const QString &name);
	static QStringList filters();

private:
	static FilterInterface *ptr_create(const QString &name);

	using NewFilter = FilterInterface* (*)();
	static QHash<QString, NewFilter> factory;
};

QByteArray filtersToJson(const QList<Filter> &filters);
QList<Filter> filtersFromJson(const QByteArray &json);

#endif  // FILTER
