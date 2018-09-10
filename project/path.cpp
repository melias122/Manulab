#include "path.h"

#include <QJsonObject>

#define j_index "index"
#define j_image "image"
#define j_text "text"

int Path::index() const
{
	return mIndex;
}

QString Path::image() const
{
	return mProjectDir + "/" + mImage;
}

QString Path::text() const
{
	return mProjectDir + "/" + mText;
}

Path::Path(const QString &projectDir, int index, const QString &image, const QString &text)
	: mProjectDir(projectDir)
	, mIndex(index)
	, mImage(image)
	, mText(text)
{
}

Path::Path(const QString &projectDir, const QJsonObject &o)
	: Path(projectDir, o[j_index].toInt(), o[j_image].toString(), o[j_text].toString())
{
}

void Path::write(QJsonObject &o)
{
	o[j_index] = mIndex;
	o[j_image] = mImage;
	o[j_text] = mText;
}
