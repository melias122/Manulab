#include "filter/distance.h"

#include "filter/settingsDialog.h"
#include "project/config.h"

QString Distance::arg1 = "N-gram";
QString Distance::arg2 = "Delimitter";


Distance::Distance()
{
    uiFlag = UiFlag(result | settings);
    argv[arg1] = "1";
    ngram_len = 1;
    argv[arg2] = "";
    delimitter = "";

    header_view = new QHeaderView(Qt::Horizontal);
    le_file_path.setText(QDir::homePath() + QDir::separator());
    le_file_path.setPlaceholderText("Path to the folder");
    btn_open_file_browser.setText("Open file browser");
    btn_export_data.setText("Export to csv");

    sort_asc = true;

    connect(&btn_export_data, &QPushButton::clicked, this, &Distance::export_histogram);
    connect(&btn_open_file_browser, &QPushButton::clicked, this, &Distance::open_file_browser);
    connect(header_view, &QHeaderView::sectionDoubleClicked, this, &Distance::sort_by_colum);
}

void Distance::sort_by_colum(int column)
{
    if( sort_asc == true )
    {
        table.sortByColumn(column, Qt::AscendingOrder);
        sort_asc = false;
    }
    else
    {
        table.sortByColumn(column, Qt::DescendingOrder);
        sort_asc = true;
    }
}

void Distance::setArgs(const Args &args)
{
     ngram_len = args[arg1].toString().toInt();
     if (ngram_len < 0 || ngram_len > 5) {
         ngram_len = 1;
     }
     delimitter = args[arg2].toString();
     FilterInterface::setArgs(args);
}

void Distance::settingsUi(QWidget *parent)
{
    settingsDialog dialog(parent);

    Args args = getArgs();
    dialog.addLineEdit(arg1, args[arg1]);
    dialog.addLineEdit(arg2, args[arg2]);

    if (dialog.exec() == QDialog::Accepted) {
        setArgs(dialog.args);
    }
}

QList<Page> Distance::processPages(QList<Page> &pages){

    QString allInOne;
    for (Page &page : pages) {
        QString txt = page.plainText();
        txt = txt.replace("\n",delimitter);
        allInOne.append(txt).append(delimitter);
    }
    //qDebug() << "before " << allInOne;
    allInOne = allInOne.left(allInOne.size() - delimitter.length());
    //qDebug() << "after " << allInOne;
    //left(allInOne.length() - 1);// last
    process(allInOne);
    return pages;
}

void Distance::reset()
{
    data.clear();
    total = 0;
}




void Distance::findAllNGrams(QString &text, quint32 n, QSet<QString>& res){
    QStringList chunks = text.split(delimitter);

    QString ngram;
    ngram.reserve(ngram_len);

        for(int i = 0; i < (chunks.size() - (n-1)); i++ )
        {
            ngram.clear();
            for(int j=0; j < n; j++){
                ngram.append(chunks.at(i+j));
            }
            if(ngram.isEmpty()){
                continue;
            }
            res.insert(ngram);
        }
}

QString Distance::process(QString &text)
{

    // check lenght of ngram inserted by user
    if( (ngram_len > 0) && (ngram_len < 5) )
    {

        QSet<QString> keys;
        findAllNGrams(text, ngram_len, keys);
        qDebug() << "Unique ngrams: " << keys.size();


        QString txt = text.replace( delimitter, "" );
        foreach(QString key, keys){
            QList<quint32> distances;

            int first = txt.indexOf(key, 0);

            do {
                int next = txt.indexOf(key, first + 1);
                if (next == -1) {
                   break;
                }

                distances.push_back(next - first);
                first = next;
            } while (true);

            data[key] = distances;

        }
    }
    else
    {
        qDebug() << "ngram lenght have to by in range <1, 15>";
    }

    return text;
}

void Distance::resultUi(QWidget *parent)
{
    if( data.isEmpty() )
    {
        return;
    }

    QDialog *dialog = new QDialog(parent);
    QGridLayout main_layout;

    const qint32 columns_count = 4;


    // adding widgetss to layout
    main_layout.addWidget(&table, 0, 0, 1, 2);
    main_layout.addWidget(&le_file_path, 1, 0, 1, 1);
    main_layout.addWidget(&btn_open_file_browser, 1, 1, 1, 1);
    main_layout.addWidget(&btn_export_data, 2, 0, 1, 2);

    dialog->setLayout(&main_layout);
    dialog->setWindowTitle("Distance results");

        // setting rows, columns and labels for columns
        table.setColumnCount(columns_count);
        table.setHorizontalHeaderLabels(QStringList() << "ngram"
                                   << "avg. dist." << "max. dist." << "count");
        // stretch the columns width to their content
        table.setHorizontalHeader(header_view);
        table.resizeColumnsToContents();

        // align the labels to left
        for (int i = 0; i < columns_count; i++){
            table.horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);
        }

        // inserting data to table

        QList<QString> keys = data.keys();

        int i=0;
        foreach( QString k, keys )
        {
                QList<quint32> vals = data[k];
                if(vals.empty()){
                    continue;
                }
                i++;
        }
        table.setRowCount(i);

        i=0;
        foreach( QString k, keys )
        {
                QList<quint32> vals = data[k];
                if(vals.empty()){
                    continue;
                }

                quint32 max = *std::max_element(vals.begin(), vals.end());
                float avg = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();


                QTableWidgetItem *itemAvg = new QTableWidgetItem;
                itemAvg->setData(Qt::DisplayRole, avg);
                QTableWidgetItem *itemMax = new QTableWidgetItem;
                itemMax->setData(Qt::DisplayRole, max);

                QTableWidgetItem *itemCount = new QTableWidgetItem;
                itemCount->setData(Qt::DisplayRole, vals.size()+1);

                table.setItem(i, 0, new QTableWidgetItem(k));
                table.setItem(i, 1, itemAvg);
                table.setItem(i, 2, itemMax);
                table.setItem(i, 3, itemCount);


                i++;

        }


        dialog->exec();
}

QString Distance::export_histogram(void)
{

    QString filePath = le_file_path.text();

    if( filePath.isEmpty() )
    {
        filePath = QDir::currentPath();
    }

    pallete.setColor(QPalette::Text,Qt::black);
    le_file_path.setPalette(pallete);

    if( !QDir(filePath).exists() )
    {
        pallete.setColor(QPalette::Text,Qt::red);
        le_file_path.setPalette(pallete);
        le_file_path.setText(filePath + " Folder not found");
        return "Folder not found";
    }

    QString time = QDateTime::currentDateTime().toString("yyyy_M_d_h_m");
    QString filename = Name + "_distance_" + time + QString(".csv");

    QFile file(filePath + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open " << file.fileName();
        return "Could not open file for writing, check your permissions";
    }

    qDebug() << "writing histogram to " << file.fileName();

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    stream << "ngram, average distance, maximal distance, distances ..." << "\n";

    QList<QString> keys = data.keys();

    foreach( QString k, keys )
    {
            QList<quint32> vals = data[k];
            quint32 max = *std::max_element(vals.begin(), vals.end());
            float avg = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();


            stream << k << "," <<  avg << "," <<  max << ",";

            for(int i=0; i < vals.size(); i++)
            {
                 stream << vals.at(i) << "," ;
            }

            stream<< "\n";

    }



    file.flush();
    file.close();
    return "Successfully exported to file.";
}

void Distance::open_file_browser()
{
    QString dir = QFileDialog::getExistingDirectory();

    if( !dir.isEmpty() )
    {
        dir.append(QDir::separator());
        pallete.setColor(QPalette::Text, Qt::black);
        le_file_path.setPalette(pallete);
        le_file_path.setText(dir);
    }
}
