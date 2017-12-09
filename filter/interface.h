#ifndef FILTER_H
#define FILTER_H

#include "qstring.h"
#include "qvariant.h"
#include "project/page.h"

// TODO: make separate class for ui fuctions with "builer/declarative" style

class FilterInterface
{
public:

	// Used for serialization
	QString Name;

	// Args are arguments of filter.
	// Args are represented as <key, value> where key
	// argument(used as argx where x is number) must be set in constructor.
	// value is value for argument, set with setArgs manualy or
	// with settingsUi.
	using Args = QHash<QString, QVariant>;

	virtual ~FilterInterface() {}

	// setArgs handles argument setting for given filter.
	// In case filter need custom argument handling this
	// function may be overriden.
	virtual void setArgs(const Args &args)
	{
		for (const auto &k : argv.keys()) {
			argv[k] = args[k];
		}
	}

	// getArgs return list of filter arguments in sorted order.
	// Args may be empty if filter does not contain any arguments.
	Args getArgs() const { return argv; }

	static FilterInterface* fromJson(const QJsonObject &o);
	QJsonObject toJson();

	// process does filter processing. Every filter must
	// override it. Process is used in chain of filters and
	// it should process any text. Text may be one line, whole page
	// or chapter
    virtual QString process(QString &text) = 0;
    virtual QList<Page> processPages(QList<Page> &pages);

	// settingsUi fuction serves as entry point for user settings.
	// Every filter that have arguments should override this fuction
	virtual void settingsUi(QWidget*) {}

	// resultUi serves as result viewer after processing
	// is done
	virtual void resultUi(QWidget*) {}

	// reset must be runned before filter are applyied on text
	// so filters can init/reset they internal states.
	// override only if you need this
	virtual void reset() {}

	// UiFlag indicates Filter Ui capabilities
	enum UiFlag
	{
		none = 0x00,
		settings = 0x01,
		result = 0x02
	};

	// hasUi is tests if filter support view
	bool hasUi(UiFlag flag) { return (flag & uiFlag) == flag; }

protected:
	UiFlag uiFlag{none};
	Args argv;
};

#endif
