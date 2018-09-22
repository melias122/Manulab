#ifndef CONFIG_H
#define CONFIG_H

#include <QFont>
#include <QObject>

#include "page.h"
#include "path.h"

class Config {

public:
	explicit Config() {}

	// CreateProject creates project.
	// If project with name exists, CreateProject
	// returns false. Otherwise it returns true
	// if successfull.
	bool CreateProject(const QString &projectName, const QList<QString> &images, const QList<QString> &texts);

	// Load restores Config from config.ml
	static Config *load(const QString &path);

	// Save saves current project config
	bool save();

	static QString defaultProjectsPath();
	static QString applicationDataPath();

	const Path *getPath(int i) const;
	QList<Page> getPages();

	QFont getFont();
	void setFont(const QFont &font);

	QString projectName() const { return mProjectDir.split("/").last(); }
	QString absoulteProjectPath() const { return mProjectDir; }

private:
	bool createProject(const QString &projectName, QString defaultPath = Config::defaultProjectsPath());
	bool import(const QList<QString> &images, const QList<QString> &_texts);

	// CreateProjectDirTree create dir and subdir
	// for given project. Returns true if directories
	// where succesfully created, othrewise false.
	// Example:
	// |--path/
	// | |---- images/
	// | |---- images/thumbnails/
	// | |---- text/
	// | |---- ...
	bool createProjectDirTree(QString path);
	//	bool createIcon();

private:
	Q_DISABLE_COPY(Config)

	// mProjectDir is set when Config is created or
	// restored from file.
	QString mProjectDir;
	QList<Path> mPaths;
	QFont m_font;
};

#endif // CONFIG_H
