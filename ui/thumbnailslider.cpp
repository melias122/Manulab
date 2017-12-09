#include "ui/thumbnailslider.h"
#include <QDebug>

#include "qgraphicsitem.h"
#include "qevent.h"
#include "qmenu.h"

thumbnailSlider::thumbnailSlider(QWidget *parent):
    QSlider(parent)
{
    this->setOrientation(Qt::Horizontal);
    this->setMinimum(0);
    //this->setMouseTracking(true);
    this->show();
    this->setVisible(true);
    this->installEventFilter(this);
}

thumbnailSlider::~thumbnailSlider(){

}

void thumbnailSlider::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->type() == QEvent::MouseButtonRelease)
    emit jumpToPageOfSlider(this->value());
    QSlider::mouseReleaseEvent(event);

}

bool thumbnailSlider::eventFilter(QObject * watched, QEvent * event)
{
    if (event->type() == QEvent::Enter){
        emit showThumbnailMINI(true);
    }

    if (event->type()==QEvent::Leave){
        emit showThumbnailMINI(false);
    }

    return QSlider::eventFilter(watched, event);

}
