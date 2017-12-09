#ifndef THUMBNAILLABEL_H
#define THUMBNAILLABEL_H

#include <QLabel>

class thumbnailLabel : public QLabel
{
    Q_OBJECT
public:
    explicit thumbnailLabel(QWidget *parent = 0);
    ~thumbnailLabel();

    void setImage(const QString &image);

private:
    int m_witdth{150}, m_heigth{150};
};

#endif // THUMBNAILLABEL_H
