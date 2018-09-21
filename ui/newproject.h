#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QFileDialog>
#include <QIcon>

#include "project/pages.h"

namespace Ui {
class NewProject;
}

class NewProject : public QDialog {
	Q_OBJECT

public:
	explicit NewProject(QWidget *parent = 0);
	~NewProject();

signals:
	void configChanged(Config *config);

private slots:
	void loadConfig(const QString &path);
	void on_importImages_clicked();
	void on_importTexts_clicked();
	void on_createProject_clicked();
	bool projectExist(const QString &projectName);

	void on_NewDocument_clicked();
	void on_Back_clicked();
	void on_OpenProject_clicked();

private:
	void createRecentPanel();
	void updateRecent(const QString &projectName);
	void createBlocks();
	QIcon getIcon(const QString &path, const QSize iconSize);

	QString recentProjectsPath() const { return Config::applicationDataPath() + "/recentProjects"; }

private:
	Ui::NewProject *ui;
};

#endif // NEWPROJECT_H
