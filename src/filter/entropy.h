#ifndef ENTROPY_H
#define ENTROPY_H

#include "interface.h"

class Entropy : public FilterInterface {
public:
	Entropy();
	void reset() override
	{
		total = 0;
		data.clear();
	}
	QList<Page> processPages(QList<Page> &pages) override;
	QString process(QString &text) override;
	void resultUi(QWidget *parent) override;

	void setArgs(const Args &args) override;
	void settingsUi(QWidget *parent) override;

private:
	QString delimitter;
	qint32 ngram_len;
	QMap<QString, quint32> data;
	float total;
	static QString arg1;
	static QString arg2;
	float entropy;
};

#endif // ENTROPY_H
