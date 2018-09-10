#include "newproject.h"
#include "ui_newproject.h"

#include <QFileDialog>
#include <QSignalMapper>
#include <QToolButton>
#include <QLayoutItem>
#include <QDebug>

#include "project/config.h"

NewProject::NewProject(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewProject)
{
	ui->setupUi(this);
	ui->tabWidget->tabBar()->hide();

	connect(ui->ProjectName, &QLineEdit::textEdited, this, &NewProject::projectExist);

	createRecentPanel();
	createBlocks();
}

NewProject::~NewProject()
{
	delete ui;
}

void NewProject::loadConfig(const QString &path)
{
	Config *config = Config::load(path);
	if (config) {
		updateRecent(config->projectName());
		emit configChanged(config);
		emit accept();
	}
}

void NewProject::on_importImages_clicked()
{
	QFileDialog dialog(this);
	dialog.setDirectory(QDir::homePath());
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(trUtf8("Images (*.jpg *.jpeg *.png *.JPG *JPEG *.PNG)"));

	if (dialog.exec()) {
		ui->imagesPaths->setStringList(dialog.selectedFiles());
	}
}

void NewProject::on_importTexts_clicked()
{
	QFileDialog dialog(this);
	dialog.setDirectory(QDir::homePath());
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(trUtf8("Text files (*.txt)"));

	if (dialog.exec()) {
		ui->textPaths->setStringList(dialog.selectedFiles());
	}
}

void NewProject::on_createProject_clicked()
{
	QString projectName = ui->ProjectName->text();
	if (projectExist(projectName)) {
		return;
	}

	QScopedPointer<Config> config(new Config);
	bool created = config->CreateProject(projectName, ui->imagesPaths->stringList(), ui->textPaths->stringList());

	if (!created) {
		// TODO: set info
		return;
	}

	updateRecent(config->projectName());

	emit configChanged(config.take());
	emit accept();
}

bool NewProject::projectExist(const QString &projectName)
{
	QDir path(Config::defaultProjectsPath() + "/" + projectName);
	bool ok = path.exists();
	if (ok && !projectName.isEmpty()) {
		ui->ProjectName->setStyleSheet("QLineEdit { color: red; }");
	} else {
		ui->ProjectName->setStyleSheet("QLineEdit { color: black; }");
	}
	return ok;
}

void NewProject::on_NewDocument_clicked()
{
	ui->tabWidget->tabBar()->setCurrentIndex(1);
}

void NewProject::on_Back_clicked()
{
	ui->tabWidget->tabBar()->setCurrentIndex(0);
}

void NewProject::on_OpenProject_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "Open Project", Config::defaultProjectsPath(), "*.ml (*.ml)");
	if (path == "") {
		return;
	}

	loadConfig(path);
}

void NewProject::createRecentPanel()
{
	QFile recentProjects(recentProjectsPath());

	QVBoxLayout *layout = static_cast<QVBoxLayout*>(ui->recentFrame->layout());

	if (recentProjects.open(QIODevice::ReadOnly)) {

		QTextStream stream(&recentProjects);
		while (!stream.atEnd()) {

			QString projectName = stream.readLine();
			QString configPath = Config::defaultProjectsPath() + "/" + projectName + "/config.ml";

			if (!QFileInfo(configPath).exists()) {
				continue;
			}

			QToolButton *recentProject = new QToolButton(this);
			recentProject->setText(projectName);
			recentProject->setFixedSize(200, 40);
			recentProject->setIcon(getIcon(projectName, QSize(38, 38)));
			recentProject->setIconSize(QSize(38, 38));
			recentProject->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
			recentProject->setStyleSheet(
				"QToolButton{ color:white; background-color: rgb(91, 195, 184);border:none;}"
				"QToolButton:hover{background-color: rgb(72, 154, 145);}"
			);
			QFont font = recentProject->font();
			font.setPointSize(10);
			recentProject->setFont(font);

			connect(recentProject, &QToolButton::clicked, [this, configPath] (){
				emit loadConfig(configPath);
			});
			layout->addWidget(recentProject);
		}
	}
	layout->addStretch();
}

void NewProject::updateRecent(const QString &projectName)
{
	QFile recentProjects(recentProjectsPath());

	if (!recentProjects.open(QFile::ReadWrite)) {
		qWarning() << "NewProject::updateRecent: cant open file " << recentProjects.fileName();
		return;
	}

	QStringList projects, toRemove;

	toRemove.append(projectName);

	{
		QTextStream stream(&recentProjects);
		while (!stream.atEnd()) {
			QString project = stream.readLine();

			if (QFileInfo(Config::defaultProjectsPath() + "/" + project + "/config.ml").exists()) {
				projects.append(project);
			} else {
				toRemove.append(project);
			}
		}
	}

	for (const QString &project : toRemove) {
		projects.removeAll(project);
	}

	projects.prepend(projectName);

	// trucate file
	recentProjects.resize(0);

	{
		QTextStream stream(&recentProjects);
		for (const QString &project : projects) {
			stream << project << "\n";
		}
	}
}

void NewProject::createBlocks()
{
	// helper struct
	struct ProjectInfo {
		QString configPath;
		QString projectName;
	//	QString projectDir;
	};

	QDir projectsDir(Config::defaultProjectsPath());
	projectsDir.setFilter(QDir::AllDirs);

	QList<ProjectInfo> projects;
	for (const QString &dir : projectsDir.entryList()) {

		QFileInfo path(projectsDir.absolutePath() + "/" + dir + "/config.ml");
		if (path.exists()) {

			ProjectInfo info;
			info.configPath = path.absoluteFilePath();
			info.projectName = dir;
//			info.projectDir = projectsDir.absolutePath() + "/" + dir;

			projects.append(info);
		}
	}

	// by default create 3xN grid
	int row = ui->projectsGrid->rowCount() - 1;
	int col = ui->projectsGrid->columnCount();

	for (const ProjectInfo &info : projects) {

		QToolButton *project = new QToolButton(this);

		project->setIcon(getIcon(info.projectName, ui->NewDocument->iconSize()));
		project->setIconSize(ui->NewDocument->iconSize());
		project->setToolButtonStyle(ui->NewDocument->toolButtonStyle());
		project->setText(info.projectName);
		project->setStyleSheet(ui->NewDocument->styleSheet());

		connect(project, &QToolButton::clicked, [=]() {
			emit loadConfig(info.configPath);
		});

		ui->projectsGrid->addWidget(project, row, col);

		col = ++col % 3;
		if (col == 0) {
			row++;
		}
	}

	ui->projectsGrid->setRowStretch(ui->projectsGrid->rowCount(), 1);
}

QIcon NewProject::getIcon(const QString &projectName, const QSize iconSize)
{
	QString iconPath(Config::applicationDataPath() + "/" + projectName + "/icons/");
	iconPath += QString::number(iconSize.width()) + "x" + QString::number(iconSize.height()) + ".png";

	if (!QFileInfo(iconPath).exists()) {
		// make dir if does not exist
		QDir dir;
		if (!dir.mkpath(Config::applicationDataPath() + "/" + projectName + "/icons")) {
			qDebug() << "can't mkdir";
		}
	}

	if (!QFileInfo(iconPath).exists()) {

		QDir images(Config::defaultProjectsPath() + "/" + projectName + "/images");
		QFileInfoList list = images.entryInfoList(QDir::Readable | QDir::Files, QDir::SortFlag::Size);

		if (list.empty()) {
			return QIcon("://icons/new.jpg");
		}

		QPixmap icon;
		if (!icon.load(list.first().absoluteFilePath())) {
			qDebug() << "can't load icon " << list.first().absoluteFilePath();
		}

		icon = icon.scaled(iconSize);
		icon = icon.copy(QRect(QPoint(0, 0), iconSize));

		if (!icon.save(iconPath)) {
			qDebug() << "can't save icon " << iconPath;
		}
	}

	return QIcon(QPixmap(iconPath));
}
