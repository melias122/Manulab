#ifndef DISTANCE_H
#define DISTANCE_H

#include "filter/interface.h"
#include "filter/worker.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QGridLayout>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>

class Distance : public FilterInterface, public QObject
{
public:
    Distance();
    void reset() override;
    void setArgs(const Args &args) override;
    QList<Page> processPages(QList<Page> &pages) override;
    QString process(QString &text) override;

    void settingsUi(QWidget *parent) override;
    void resultUi(QWidget *parent) override;

private slots:
    QString export_histogram(void);
    void open_file_browser(void);
    void sort_by_colum(int column);

private:

    void generateAllNGrams(qint32 n, QSet<QString>& res);
    void generateAllNGrams(qint32 lvl, qint32 n, QString subStr, QSet<QString>& res, qint32 abcLength);
    void findAllNGrams(QString &text,quint32 n, QSet<QString>& res);

    QTableWidget table;
    QHeaderView *header_view;
    QLineEdit le_file_path;
    QPalette pallete;
    QPushButton btn_open_file_browser;
    QPushButton btn_export_data;

    float total;
    QMap<QString, QList<quint32>> data;
    qint32 ngram_len;
    bool sort_asc;
    QString delimitter;

    static QString arg1;
    static QString arg2;
};

#endif // DISTANCE_H
