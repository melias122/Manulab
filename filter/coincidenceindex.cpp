#include "coincidenceindex.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QDebug>
#include "filter/settingsDialog.h"
#include "math.h"

QString CoincidenceIndex::arg1 = "Delimitter";

CoincidenceIndex::CoincidenceIndex()
{
     uiFlag = UiFlag(result | settings);
    // https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
	// known frequency distributions
	// http://www.dcode.fr/index-coincidence
    IC_languages["Italian"] = 0.0738;
    IC_languages["French"] = 0.0778;
    IC_languages["Russian"] = 0.0529;
    IC_languages["English"] = 0.0667;
    IC_languages["German"] = 0.0762;
    IC_languages["Spanish"] = 0.0770;
    IC_languages["Random text"] = 0.0385;

    argv[arg1] = "";
    delimitter = "";
}

void CoincidenceIndex::setArgs(const Args &args)
{
     delimitter = args[arg1].toString();
     FilterInterface::setArgs(args);
}

void CoincidenceIndex::settingsUi(QWidget *parent)
{
    settingsDialog dialog(parent);

    Args args = getArgs();
    dialog.addLineEdit(arg1, args[arg1]);

    if (dialog.exec() == QDialog::Accepted) {
        setArgs(dialog.args);
    }
}


QList<Page> CoincidenceIndex::processPages(QList<Page> &pages){

    QString allInOne;
    for (Page &page : pages) {
        QString txt = page.plainText();
        txt = txt.replace("\n",delimitter);
        allInOne.append(txt).append(delimitter);
    }
    //allInOne = allInOne.left(allInOne.length() - 1);// last
    allInOne = allInOne.left(allInOne.size() - delimitter.length());
    process(allInOne);
    return pages;
}

QString CoincidenceIndex::process(QString &text)
{
    QMap<QString, quint32> data;

    unsigned long long letterCountSum = 0L;
    unsigned long long totalLetters = 0L;


    // calculate IC for given text
    if( !text.isEmpty() )
    {
        QStringList chunks = text.split(delimitter);



            for(QString s :chunks)
            {
                if(s.isEmpty()){
                    continue;
                }
                if( data.find(s) != data.end() )
                {
                    data[s]++;
                }
                else
                {
                    data.insert(s, 1);
                }
                totalLetters++;
            }


        for( auto data_it = data.begin(); data_it != data.end(); data_it++ )
        {
            letterCountSum += data_it.value() * (data_it.value() - 1);
        }

        // formula for calculating the index of coincidence
        IC = ((double)letterCountSum / (double)(totalLetters * (totalLetters - 1)));
        // qDebug() << "Index of coincidence for given text is:";
        // qDebug() << IC;
    }

    return text;
}

void CoincidenceIndex::resultUi(QWidget *parent)
{
    qint16 columns_count = 3;

    // http://practicalcryptography.com/cryptanalysis/text-characterisation/index-coincidence/
    QDialog dialog(parent);
	QVBoxLayout layout(&dialog);
    QTableWidget table;//table(&dialog);

	// setting rows, columns and labels for columns
	table.setRowCount(IC_languages.count());
	table.setColumnCount(columns_count);
	table.setHorizontalHeaderLabels(QStringList() << "Language"
							   << "IC"
                               << "Absoult value");
	// stretch the columns width to their content
	table.resizeColumnsToContents();

	// align the labels to left
    for (int i = 0; i < columns_count; i++)
		table.horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);

    qint32 i = 0;
	// inserting data to table
	for (auto IC_lang_it = IC_languages.begin(); IC_lang_it != IC_languages.end(); IC_lang_it++) {
		table.setItem(i, 0, new QTableWidgetItem(IC_lang_it.key()));
		table.setItem(i, 1, new QTableWidgetItem(QString::number(IC_lang_it.value(), 'g', 5)));

		float match = fabs(IC - IC_lang_it.value());  // abs for float
		table.setItem(i, 2, new QTableWidgetItem(QString::number(match, 'g', 5)));

		// coloring cells
        if (match <= 0.002)
			table.item(i, 2)->setBackgroundColor(Qt::green);
        else if (match > 0.002 && match <= 0.005)
			table.item(i, 2)->setBackgroundColor(Qt::yellow);
		else
			table.item(i, 2)->setBackgroundColor(Qt::red);
		i++;
	}
	// sort table rows by best match with documents actual coincidence index
	table.sortByColumn(2, Qt::AscendingOrder);

	QLabel label("Index of coincidence for the document is: " + QString::number(IC), &dialog);
    label.setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font;
    font.setBold(true);
    label.setFont(font);

	layout.addWidget(&label);
    //layout.addWidget(&table);

	dialog.setWindowTitle("Index of Coincidence");

	dialog.exec();
}
