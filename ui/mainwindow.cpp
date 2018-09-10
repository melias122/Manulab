#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>
#include <QLabel>

#include "newproject.h"
#include "project/page.h"

#define SECOND (1000)

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> sizes = {260};
    ui->splitter->setSizes(sizes);
    ui->searchReplaceFrame->hide();

//	connect(ui->saveProject, &QAction::triggered, this, &MainWindow::saveProject);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->openProjectManager, &QAction::triggered, this, &MainWindow::openProjectManager);
    connect(ui->nextPage, &QAction::triggered, this, &MainWindow::nextPage);
    connect(ui->previousPage, &QAction::triggered, this, &MainWindow::previousPage);
    connect(ui->textEdit, &TextEditor::plainTextChanged, this, &MainWindow::plainTextChanged);
    connect(ui->filtersList, &FilterList::addFilterToManager, ui->filterManager, &FilterManager::on_addFilter);
    connect(ui->removeAllFilters, &QAction::triggered, ui->filterManager, &FilterManager::on_removeAllFilters);

    // TextEditor
    connect(ui->actionCopyText, &QAction::triggered, ui->textEdit, &TextEditor::copy);
    connect(ui->actionCutText, &QAction::triggered, ui->textEdit, &TextEditor::cut);
    connect(ui->actionPasteText, &QAction::triggered, ui->textEdit, &TextEditor::paste);
    connect(ui->actionUndoText, &QAction::triggered, ui->textEdit, &TextEditor::undo);
    connect(ui->actionRedoText, &QAction::triggered, ui->textEdit, &TextEditor::redo);
    connect(ui->actionWrapLine, &QAction::toggled, ui->textEdit, &TextEditor::on_actionWrapLine_toggled);

    // Find/Replace
    connect(ui->actionFindReplace, &QAction::toggled, this, &MainWindow::findReplaceFrameVisibility);
    connect(ui->findTextLineEdit, &QLineEdit::textChanged, ui->textEdit, &TextEditor::searchedTextChanged);
    connect(ui->textEdit, &TextEditor::textFound, this, &MainWindow::lineEditPalette);

    connect(this, &MainWindow::findNextClicked, ui->textEdit, &TextEditor::findNextText);
    connect(this, &MainWindow::findPreviousClicked, ui->textEdit, &TextEditor::findPreviousText);
    connect(ui->findNext, &QPushButton::clicked, this, &MainWindow::findNextText);
    connect(ui->findPrevious, &QPushButton::clicked, this, &MainWindow::findPreviousText);

    connect(this, &MainWindow::replaceNextClicked, ui->textEdit, &TextEditor::replaceNextText);
    connect(this, &MainWindow::replaceAllClicked, ui->textEdit, &TextEditor::replaceAllText);
    connect(ui->replaceNext, &QPushButton::clicked, this, &MainWindow::replaceNextText);
    connect(ui->replaceAll, &QPushButton::clicked, this, &MainWindow::replaceAllText);

    connect(ui->actionChange_Orientation, &QAction::toggled, this, &MainWindow::changeOrientation);

    // Set initial font size
    setCurrentFont(QFont("Sans", 14));

    // by default we are in filter mode
    ui->saveProject->setEnabled(false);

    ui->nextPage->setAutoRepeat(false);
    ui->previousPage->setAutoRepeat(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findReplaceFrameVisibility(bool visible)
{
    if (visible == true) {
        ui->searchReplaceFrame->show();
        ui->findTextLineEdit->setFocus();
    } else {
        ui->searchReplaceFrame->hide();
        ui->findTextLineEdit->setText("");
        ui->replaceTextLineEdit->setText("");
        ui->textEdit->setFocus();
    }
}

void MainWindow::findNextText() { emit findNextClicked(ui->findTextLineEdit->text()); }

void MainWindow::findPreviousText() { emit findPreviousClicked(ui->findTextLineEdit->text()); }

void MainWindow::replaceNextText()
{
    emit replaceNextClicked(ui->findTextLineEdit->text(), ui->replaceTextLineEdit->text());
}

void MainWindow::replaceAllText()
{
    emit replaceAllClicked(ui->findTextLineEdit->text(), ui->replaceTextLineEdit->text());
}

void MainWindow::setInfo(const QString &info) { ui->statusBar->showMessage(info, 5 * SECOND); }

void MainWindow::on_configChanged(Config *newConfig)
{
    config = newConfig;
    imgPool.reset(new ImagePool(config));
    setCurrentFont(config->getFont());
    setThumbnails();
}

void MainWindow::on_pageChanged(const Page &page)
{
	ui->textEdit->setPlainText(page.plainText());
	ui->imageView->setImage(page.image());
	thumbnailSlider1->setValue(page.index());
}

void MainWindow::lineEditPalette(bool found)
{
    QPalette palette;

    if (found) {
        palette.setColor(QPalette::Base, Qt::white);
    } else {
        palette.setColor(QPalette::Base, Qt::red);
    }

    ui->findTextLineEdit->setPalette(palette);
}

void MainWindow::changeOrientation(bool orientation)
{
    ui->splitter->setOrientation(orientation ? Qt::Vertical : Qt::Horizontal);
}

void MainWindow::on_filtersExecute_clicked()
{
    emit executeFilters(ui->filterManager->filters());
}

void MainWindow::on_changeFont_triggered()
{
    // get current font
    QFont oldFont = ui->textEdit->font();

    bool ok;

    // get selected font
    QFont newFont = QFontDialog::getFont(&ok, oldFont, this);

    if (ok) {
        setCurrentFont(newFont);
    }

}

void MainWindow::setCurrentFont(const QFont &font)
{
    ui->textEdit->setFont(font);
    if (config) {
        config->setFont(font);
    }
}

void MainWindow::on_jumpToPage_triggered()
{
    QDialog dialog(this);

    QVBoxLayout layout(&dialog);
    QLineEdit line(&dialog);

    QPushButton ok(&dialog), cancel(&dialog);
    ok.setText("Ok");
    cancel.setText("Cancel");

    connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    QHBoxLayout buttons(&dialog);
    buttons.addWidget(&cancel);
    buttons.addWidget(&ok);

    layout.addWidget(&line);
    layout.addLayout(&buttons);

    if (dialog.exec()) {
        bool ok;
        int page = line.text().toInt(&ok);
        if (ok) {
            emit jumpToPage(page - 1);
        }
    }
}

void MainWindow::on_mode_toggled(bool ok)
{
    // on edit mode
    ui->saveProject->setEnabled(ok);
    if (ok) {
        emit reloadPages();
    }

    // on filter mode
    ui->filtersExecute->setEnabled(!ok);
    filterMode = !ok;

}

void MainWindow::on_filtersSaveAs_triggered()
{
    QList<Filter> filters = ui->filterManager->filters();
    if (filters.size() == 0) {
        setInfo("Nothing to save");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Save As...", Config::defaultProjectsPath());

    if (path == "") {
        return;
    }

    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        setInfo("Can't open file " + path);
        return;
    }

    file.write(filtersToJson(filters));

    setInfo("Filters saved to " + path);
}

void MainWindow::on_filtersOpenFrom_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open from...", Config::defaultProjectsPath());

    if (path == "") {
        return;
    }

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        setInfo("Can't open file " + path);
        return;
    }

    QList<Filter> filters = filtersFromJson(file.readAll());

    ui->filterManager->setFilters(filters);
}

void MainWindow::on_saveProject_triggered()
{
    QMessageBox dialog;
    dialog.setText("Current project will be overriden");
    dialog.setInformativeText("Are you sure?");
    dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);

    if (dialog.exec() == QMessageBox::Save) {
        emit saveProject();
    }
}

//set Thumbnail Bar
void MainWindow::setThumbnails()
{
    if(layout != NULL){
        delete label;
        disconnect(thumbnailSlider1,&QSlider::valueChanged,this, &MainWindow::setCurrentThumbnail);
        delete thumbnailSlider1;
        delete layout;

    }
    label = new thumbnailLabel(ui->imageView);
    this->setCurrentThumbnail(0);

    thumbnailSlider1 = new thumbnailSlider(ui->imageView);
    thumbnailSlider1->setMaximum(config->getPages().size() -1);
    qDebug() << "maximum " << thumbnailSlider1->maximum();
    thumbnailSlider1->setValue(0);

    layout = new QVBoxLayout(ui->imageView);

    layout->addWidget(label,1,Qt::AlignBottom);
    layout->addWidget(thumbnailSlider1,0,Qt::AlignBottom);

    connect(thumbnailSlider1, &QSlider::valueChanged, this, &MainWindow::setCurrentThumbnail);
    connect(thumbnailSlider1, &thumbnailSlider::jumpToPageOfSlider, this, &MainWindow::jumpToPage);
    connect(thumbnailSlider1, &thumbnailSlider::showThumbnailMINI,label, &thumbnailLabel::setVisible);
}

void MainWindow::setCurrentThumbnail(int index)
{
	label->setImage(config->getPath(index)->image());
}
