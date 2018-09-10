#include "orientation.h"

#include <algorithm>
#include <QDialog>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
#include <QObject>

QString Orientation::arg1 = "Horizontal";
QString Orientation::arg2 = "Vertical";

Orientation::Orientation()
{
	uiFlag = UiFlag::settings;

	argv[arg1] = Horizontal::LeftToRight;
	argv[arg2] = Vertical::UpToDown;
}

QList<Page> Orientation::processPages(QList<Page> &pages){

    QList<Page> retVal;
    for (Page &page : pages) {
        QString text;
        text.append(page.plainText());
        page.setPlainText(text);
        retVal.append(page);
    }
    Horizontal h = Horizontal(argv[arg1].toInt());
    if (h == Horizontal::RightToLeft) {
        std::reverse(retVal.begin(), retVal.end());
    }

    return retVal;
}

QString Orientation::process(QString &text)
{
	Horizontal h = Horizontal(argv[arg1].toInt());
	Vertical v = Vertical(argv[arg2].toInt());

	QList<QString> lines = text.split("\n");

	if (h == Horizontal::RightToLeft) {
		for (QString &line : lines) {
			std::reverse(line.begin(), line.end());
		}
	}

	if (v == Vertical::DownToUp) {
		std::reverse(lines.begin(), lines.end());
	}

	return lines.join('\n');
}

void Orientation::settingsUi(QWidget *parent)
{
	QDialog dialog(parent);
	QVBoxLayout layout(&dialog);

	QComboBox horizontal(&dialog);
	horizontal.addItem("Left -> Right", LeftToRight);
	horizontal.addItem("Right -> Left", RightToLeft);
	horizontal.setCurrentIndex(argv[arg1].toInt());

	QComboBox vertical(&dialog);
	vertical.addItem("Up -> Down", UpToDown);
	vertical.addItem("Down -> Up", DownToUp);
	vertical.setCurrentIndex(argv[arg2].toInt());

	QHBoxLayout buttons;

	QPushButton cancel(&dialog);
	cancel.setText("Cancel");
	QObject::connect(&cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
	buttons.addWidget(&cancel);

	QPushButton ok(&dialog);
	ok.setText("Ok");
	ok.setDefault(true);
	ok.setFocus();
	QObject::connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
	buttons.addWidget(&ok);

	layout.addWidget(&horizontal);
	layout.addWidget(&vertical);
	layout.addLayout(&buttons);

	if (dialog.exec() == QDialog::Accepted) {
		argv[arg1] = horizontal.currentData().toInt();
		argv[arg2] = vertical.currentData().toInt();
	}
}

