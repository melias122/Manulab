#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "filter/interface.h"

class Substitution : public FilterInterface
{
public:
	Substitution();
//    QList<Page> process(QList<Page> &pages) override;
	QString process(QString &text) override;
	void settingsUi(QWidget *parent) override;

private:
	static QString arg1;
	static QString arg2;
};

#endif
