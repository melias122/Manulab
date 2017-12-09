#ifndef COINCIDENCEINDEX_H
#define COINCIDENCEINDEX_H

#include "filter/interface.h"

class CoincidenceIndex : public FilterInterface
{
public:
    CoincidenceIndex();
    void reset() override { IC = 0; }
    QList<Page> processPages(QList<Page> &pages) override;
    QString process(QString &text) override;
    void resultUi(QWidget *parent) override;

    void setArgs(const Args &args) override;
    void settingsUi(QWidget *parent) override;


private:
    QMap<QString, float> IC_languages;
    QString delimitter;
    static QString arg1;
    float IC;
};
#endif // COINCIDENCEINDEX_H
