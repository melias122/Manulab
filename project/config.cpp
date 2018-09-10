#include "config.h"

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDataStream>
#include <QScopedPointer>

#define j_config = "config"
#define j_paths "paths"
#define j_font "font"
#define j_fontFamily "family"
#define j_fontSize "size"

bool Config::CreateProject(const QString &projectName, const QList<QString> &images, const QList<QString> &texts)
{
	bool ok = createProject(projectName);
	ok = ok && import(images, texts);
	if (!ok) {
		QDir dir(defaultProjectsPath() + "/" + projectName);
		dir.removeRecursively();
	} else {
		ok = save();
	}
	return ok;
}

Config *Config::load(const QString &path)
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Config::Load: cant open file " << file.fileName();
		return 0;
	}

	Config *xconfig = new Config;

	QDir cwd(path);
	cwd.cdUp();
	xconfig->mProjectDir = cwd.absolutePath();

	QByteArray saveData = file.readAll();
	QJsonDocument document(QJsonDocument::fromJson(saveData));

	QJsonObject config = document.object();
	QJsonObject font = config[j_font].toObject();

	xconfig->m_font = QFont(font[j_fontFamily].toString());
	xconfig->m_font.setPixelSize(font[j_fontSize].toInt());

	for (auto a : config[j_paths].toArray()) {
		xconfig->mPaths.append(Path(xconfig->mProjectDir, a.toObject()));
	}

	// sort paths by their index
	qSort(xconfig->mPaths.begin(), xconfig->mPaths.end(), [](Path &p0, Path &p1) { return p0.index() < p1.index(); });

	return xconfig;
}

bool Config::save()
{
	if (mProjectDir.isEmpty()) {
		return false;
	}

	QFile file(mProjectDir + "/config.ml");

	if (!file.open(QIODevice::WriteOnly)) {
		qWarning() << "Config::Save: cant open file " << file.fileName();
		return false;
	}

	// set paths
	QJsonArray paths;
	for (Path &p : mPaths) {
		QJsonObject o;
		p.write(o);
		paths.append(o);
	}

	// set font
	QJsonObject font;
	font[j_fontFamily] = m_font.family();
	font[j_fontSize] = m_font.pixelSize();

	// set config
	QJsonObject config;
	config[j_paths] = paths;
	config[j_font] = font;

	QJsonDocument document(config);
	file.write(document.toJson());

	return true;
}

QString Config::defaultProjectsPath()
{
	return QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory) + "ManuLab Projects";
}

QString Config::applicationDataPath()
{
	QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	if (!QFileInfo(path).exists()) {
		QDir dir;
		dir.mkpath(path);
	}
	return path;
}

const Path *Config::getPath(int i) const
{
	Q_ASSERT(mPaths.size() != 0);

	if (i < 0) {
		i = 0;
	} else if (i >= mPaths.size()) {
		i = mPaths.size() - 1;
	}

	return &mPaths[i];
}

QList<Page> Config::getPages()
{
	QList<Page> pages;
	for (const auto &path : mPaths) {
		pages.append(Page(&path));
	}
	return pages;
}

QFont Config::getFont()
{
	return m_font;
}

void Config::setFont(const QFont &font)
{
	m_font = font;
}

bool Config::createProject(const QString &projectName, QString defaultPath)
{
	QString projectDir = defaultPath + "/" + projectName;
	if (QDir(projectDir).exists()) {
		qWarning() << "Config::CreateProject: dir " << projectDir << " exists!";
		return false;
	}
	if (!createProjectDirTree(projectDir)) {
		return false;
	}
	mProjectDir = projectDir;

	return true;
}

bool Config::import(const QList<QString> &images, const QList<QString> &_texts)
{
	if (images.isEmpty()) {
		qWarning() << "Config::ImportImages: no images?";
		return false;
	}

	QStringList texts = _texts;
	for (int i = 0; i < images.size(); i++) {
		texts.append("");
	}

	QDir projectDir(mProjectDir);
	for (int i = 0; i < images.size(); i++) {

		auto image = projectDir.absoluteFilePath("images/" + QString::number(i));
		if (!QFile::copy(images[i], image)) {
			qWarning() << "Config::import: cant copy image " << images[i] << image;
			return false;
		}

		auto text = projectDir.absoluteFilePath("texts/" + QString::number(i));
		if (!texts[i].isEmpty() && !QFile::copy(texts[i], text)) {
			qWarning() << "Config::import: cant copy text " << texts[i] << text;
			return false;
		}

		mPaths.append(Path(mProjectDir, i, projectDir.relativeFilePath(image), projectDir.relativeFilePath(text)));
	}

	// sort paths by their index
	qSort(mPaths.begin(), mPaths.end(), [](Path &p0, Path &p1) { return p0.index() < p1.index(); });

	return true;
}

bool Config::createProjectDirTree(QString path)
{
	QString subdirs[] = {
		"images", "images/thumbnails", "texts",
	};

	QDir dir(path);
	if (!dir.mkpath(".")) {
		qWarning() << "Config::createProjectDirTree: " << dir.absolutePath() << " already exist";
		return false;
	}

	for (auto &subdir : subdirs) {
		if (!dir.mkdir(subdir)) {
			qWarning() << "Config::createProjectDirTree: cant create subdir " << subdir;
			return false;
		}
	}

	return true;
}
