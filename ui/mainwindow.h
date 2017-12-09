#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow.h"

#include "project/pages.h"
#include "project/config.h"
#include "ui/thumbnaillabel.h"
#include "ui/thumbnailslider.h"
#include <QVBoxLayout>
#include "helpers/imagepool.h"
#include <memory>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void findReplaceFrameVisibility(bool visible);

signals:
    void findNextClicked(const QString &text);
    void findPreviousClicked(const QString &text);
    void replaceNextClicked(const QString &search, const QString &replace);
    void replaceAllClicked(const QString &search, const QString &replace);
    void openProjectManager();
    void jumpToPage(int);
    void nextPage();
    void previousPage();
    void saveProject();
    void plainTextChanged(const QString &);
    void executeFilters(const QList<Filter> &);
    void reloadPages();

public slots:
    void setInfo(const QString &info);
    void on_configChanged(Config *newConfig);
	void on_pageChanged(const Page &page);

private slots:
    void lineEditPalette(bool found);
    void findNextText();
    void findPreviousText();
    void replaceNextText();
    void replaceAllText();
    void changeOrientation(bool orientation);
    void on_filtersExecute_clicked();
    void on_changeFont_triggered();
    void on_jumpToPage_triggered();
    void on_mode_toggled(bool ok);
    void on_filtersSaveAs_triggered();
    void on_filtersOpenFrom_triggered();
    void on_saveProject_triggered();
    void setThumbnails();
    void setCurrentThumbnail(int index);

private:
    void setCurrentFont(const QFont &font);

private:
    Ui::MainWindow *ui;
    Config *config{0};
    std::unique_ptr<ImagePool> imgPool;// thumbnail
    bool filterMode{true};

    thumbnailLabel *label = NULL;
    thumbnailSlider *thumbnailSlider1 = NULL;
    QVBoxLayout *layout = NULL;
};

#endif  // MAINWINDOW_H
