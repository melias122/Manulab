#include "entropy.h"

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QDebug>
#include "filter/settingsDialog.h"
#include "math.h"

QString Entropy::arg1 = "N-gram";
QString Entropy::arg2 = "Delimitter";

Entropy::Entropy()
{
     uiFlag = UiFlag(result | settings);

     argv[arg1] = "1";
     ngram_len = 1;

     argv[arg2] = "";
     delimitter = "";
}

void Entropy::setArgs(const Args &args)
{
     ngram_len = args[arg1].toString().toInt();
     if (ngram_len < 0 || ngram_len > 5) {
         ngram_len = 1;
     }
     delimitter = args[arg2].toString();
     FilterInterface::setArgs(args);
}

void Entropy::settingsUi(QWidget *parent)
{
    settingsDialog dialog(parent);

    Args args = getArgs();
    dialog.addLineEdit(arg1, args[arg1]);
    dialog.addLineEdit(arg2, args[arg2]);

    if (dialog.exec() == QDialog::Accepted) {
        setArgs(dialog.args);
    }
}


QList<Page> Entropy::processPages(QList<Page> &pages){

    QString allInOne;
    for (Page &page : pages) {
        QString txt = page.plainText();
        txt = txt.replace("\n",delimitter);
        allInOne.append(txt).append(delimitter);
    }
    allInOne = allInOne.left(allInOne.size() - delimitter.length());
    process(allInOne);
    return pages;
}

QString Entropy::process(QString &text)
{

    if( (ngram_len > 0) && (ngram_len < 6) )
    {
        QStringList chunks = text.split(delimitter);
        entropy = 0;
        QString ngram;
        ngram.reserve(ngram_len);

        for(int i = 0; i < (chunks.size() - (ngram_len-1)); i++ )
        {
            ngram.clear();
            for(int j=0; j < ngram_len; j++){
                ngram.append(chunks.at(i+j)).append(",");
            }
            ngram = ngram.left(ngram.length() - 1); // remove last ,
            if(ngram.isEmpty()){
                continue;
            }
            if( data.find(ngram) != data.end() )
            {
                data[ngram]++;
            }
            else
            {
                data.insert(ngram, 1);
            }
            total++;
        }

        //
        QMap<QString, uint>::iterator map_it;

        entropy = 0;
        for( map_it = data.begin(); map_it != data.end(); map_it++ )
        {

            float pi = (float)(map_it.value()/total);
            entropy += pi * (log(pi)/log(2));
        }
        entropy *= -1;
//        qDebug() << "entropy: " << entropy;

    }
    else
    {
        qDebug() << "ngram lenght has to by in range <1, 5>";
    }

    return text;
}

void Entropy::resultUi(QWidget *parent)
{

    QDialog dialog(parent);
    QVBoxLayout layout(&dialog);


    qDebug() << "entropy: " << entropy;

    QLabel label("Entropy of the document is: " + QString::number(entropy), &dialog);
    label.setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font;
    font.setBold(true);
    label.setFont(font);

    layout.addWidget(&label);

    dialog.setWindowTitle("Entropy");

    dialog.exec();
}
