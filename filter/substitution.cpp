#include "substitution.h"

#include <QDebug>
#include <QRegularExpression>

#include "settingsDialog.h"

QString Substitution::arg1 = "Pattern";
QString Substitution::arg2 = "Replace";

Substitution::Substitution()
{
	uiFlag = UiFlag::settings;

	argv[arg1] = QString();
	argv[arg2] = QString();
}


QString Substitution::process(QString &text)
{
	auto pattern = QRegularExpression(argv[arg1].toString());
	auto replace = QString(argv[arg2].toString());

	text.replace(pattern, replace);

	return text;
}

void Substitution::settingsUi(QWidget *parent)
{
	auto args = getArgs();
	settingsDialog dialog(parent);

	dialog.addLineEdit(arg1, args[arg1]);
	dialog.addLineEdit(arg2, args[arg2]);

	if (dialog.exec() == QDialog::Accepted) {
		setArgs(dialog.args);
	}
}
