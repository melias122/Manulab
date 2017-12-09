#ifndef THUMBNAILSLIDER_H
#define THUMBNAILSLIDER_H

#include <QSlider>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class QMouseEvent;

class thumbnailSlider : public QSlider
{
    Q_OBJECT
public:
    explicit thumbnailSlider(QWidget *parent = 0);
    ~thumbnailSlider();

    void mouseReleaseEvent ( QMouseEvent * event );
    bool eventFilter(QObject * watched, QEvent * event);

signals:

    void jumpToPageOfSlider(int);
    void showThumbnailMINI(bool);

};

#endif // THUMBNAILSLIDER_H
