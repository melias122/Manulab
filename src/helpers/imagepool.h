#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QMap>
#include <QPixmap>

#include "project/config.h"

class ImagePool {
public:
	ImagePool(Config *con)
	{
		config = con;
	}
	~ImagePool()
	{
		reset();
	}

	QPixmap getImage(int index);
	void reset();

private:
	Config *config{ 0 };
	QMap<QString, QPixmap> data;
};

#endif // OBJECTPOOL_H
