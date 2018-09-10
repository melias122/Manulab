#ifndef COINCIDENCEINDEX_H
#define COINCIDENCEINDEX_H

#include "interface.h"

class IndexOfCoincidence : public FilterInterface
{
public:
    IndexOfCoincidence();
    void reset() override { IC = 0; }
    QList<Page> processPages(QList<Page> &pages) override;
    QString process(QString &text) override;
    void resultUi(QWidget *parent) override;

    void setArgs(const Args &args) override;
    void settingsUi(QWidget *parent) override;


private:
    QMap<QString, float> IC_languages;
    float delta = 0.002;
    QString delimitter;
    static QString arg1, arg2;
    float IC;
};
#endif // COINCIDENCEINDEX_H
