#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "qdialog.h"
#include "filter/interface.h"

class settingsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit settingsDialog(QWidget *parent);
	void addLineEdit(const QString &argName, const QVariant &argValue);
    void addCheckBox(const QString &argName, const QVariant &argValue, const QString &desc);
	void addComboBox(const QString &label, const QStringList &argNames, const QVariantList &argValues);

public slots:
	int exec();

public:
	FilterInterface::Args args;
};

#endif
