#include "settingsDialog.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLayout>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>

settingsDialog::settingsDialog(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);
	setLayout(new QVBoxLayout(this));
}

void settingsDialog::addLineEdit(const QString &argName, const QVariant &argValue)
{
	// set previous value
	args[argName] = argValue;

	// create lineEdit
	QLineEdit *lineEdit = new QLineEdit(this);

	// use arg as placeholder, set value
	lineEdit->setPlaceholderText(argName);
	lineEdit->setText(argValue.toString());

	// add lineEdit to layout
	layout()->addWidget(lineEdit);

	// edit arg
	connect(lineEdit, &QLineEdit::textEdited, [=](const QString &text) {
		args[argName] = text;
	});
}

void settingsDialog::addCheckBox(const QString &argName, const QVariant &argValue, const QString &desc)
{
	// set previous value
	args[argName] = argValue;

	// create lineEdit
	QCheckBox *checkBox = new QCheckBox(desc, this);

	if (argValue.toBool()) {
		checkBox->setChecked(true);
	} else {
		checkBox->setChecked(false);
	}

	// add checkBox to layout
	layout()->addWidget(checkBox);

	// edit arg

	connect(checkBox, &QCheckBox::stateChanged, [=](const bool checked) {
		args[argName] = (checked) ? "true" : "false";
	});
}

//void settingsDialog::addComboBox(const QString &label, const QStringList &argNames, const QVariantList &argValues)
//{
//	args[argName] = argValue;

//	QComboBox *comboBox = new QComboBox(this);
//	comboBox->setLayout(new QHBoxLayout(comboBox));

//	comboBox->addItem(argName, argValue[0]);
//	comboBox->addItem(argName, argValue[1]);
//	comboBox->ad

//	layout()->addWidget(comboBox);
//}

int settingsDialog::exec()
{
	// create buttons for ok, cancel
	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

	QPushButton *cancel = buttonBox->addButton(QDialogButtonBox::Cancel);
	QPushButton *ok = buttonBox->addButton(QDialogButtonBox::Ok);

	connect(ok, &QPushButton::clicked, this, &settingsDialog::accept);
	connect(cancel, &QPushButton::clicked, this, &settingsDialog::reject);

	// add buttons to layout
	layout()->addWidget(buttonBox);

	// exec parent
	return QDialog::exec();
}
