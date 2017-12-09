#include "select.h"
#include "settingsDialog.h"

QString Select::arg1 = "From";
QString Select::arg2 = "To";

Select::Select()
{
	uiFlag = UiFlag::settings;

    argv[arg1] = "";
    argv[arg2] = "";
}

void Select::setArgs(const Args &args)
{
	FilterInterface::setArgs(args);

	from = argv[arg1].toString().toInt();
	to = argv[arg2].toString().toInt();
}


QList<Page> Select::processPages(QList<Page> &pages){
    if(argv[arg1] == "" || argv[arg2]==""){
        return pages;
    }
    QList<Page> retVal;
    for (Page &page : pages) {
        counter++;
        if (counter >= from && counter <= to) {
            retVal.append(page);
        }
    }
    return retVal;
}

QString Select::process(QString &text)
{
//	counter++;
//	if (counter >= from && counter <= to) {
//		return text;
//	}

    return text;
}

void Select::settingsUi(QWidget *parent)
{
	settingsDialog dialog(parent);

	Args args = getArgs();

	dialog.addLineEdit(arg1, args[arg1]);
	dialog.addLineEdit(arg2, args[arg2]);

	if (dialog.exec() == QDialog::Accepted) {
		setArgs(dialog.args);
	}
}

void Select::reset()
{
	counter = 0;
}
