#ifndef PATH_H
#define PATH_H

#include <QString>

class QJsonObject;

class Path
{
	friend class Config;

public:
	int index() const;
	QString image() const;
	QString text() const;

private:
	Path(const QString &projectDir, const QJsonObject &o);
	Path(const QString &projectDir, int index, const QString &image, const QString &text);
	void write(QJsonObject &o);

	QString mProjectDir;
	int mIndex;
	QString mImage;
	QString mText;
};

#endif // PATH_H
