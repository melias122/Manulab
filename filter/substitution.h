#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include "interface.h"

class Substitution : public FilterInterface
{
public:
	Substitution();
	QString process(QString &text) override;
	void settingsUi(QWidget *parent) override;

private:
	static QString arg1;
	static QString arg2;
};

#endif
