#ifndef SELECT_H
#define SELECT_H

#include "filter/interface.h"

class Select : public FilterInterface
{
public:
	Select();
	void setArgs(const Args &args) override;
    QList<Page> processPages(QList<Page> &pages) override;
	QString process(QString &text) override;
	void settingsUi(QWidget *parent) override;
	void reset() override;

private:
	int counter{0}, from, to;
	static QString arg1;
	static QString arg2;
};

#endif // SELECT_H
