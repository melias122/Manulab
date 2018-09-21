#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QMap>
#include <QPixmap>

#include "project/config.h"

class ImagePool {
public:
	QPixmap getImage(int index);
	void reset();
	ImagePool(Config *con)
	{
		config = con;
	}
	~ImagePool()
	{
		reset();
	}

private:
	Config *config{ 0 };
	QMap<QString, QPixmap> data;
};

#endif // OBJECTPOOL_H
