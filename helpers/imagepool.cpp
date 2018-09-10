#include "imagepool.h"

QPixmap ImagePool::getImage(int index)
{
    QString k = QString::number(index);

    if( data.find(k) == data.end() )
    {
        QPixmap newThumbnail(config->getPath(index)->image());
        data.insert(k, newThumbnail);
    }

    return data[k];
}

void ImagePool::reset()
{
    data.clear();
}
