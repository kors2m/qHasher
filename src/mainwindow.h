#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDesktopWidget>
#include <QProgressBar>
#include <QLineEdit>
#include <QThread>
#include <QClipboard>
#include <QTimer>
#include <QModelIndex>

#include <QDebug>

#include "hasher.h"
#include "model.h"
#include "version.h"
#include "logview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void doneHash();

public slots:
    void done();
    void update(const QModelIndex &index);
    void rehash();
    void openLog();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openFile();
    void about();
    void interruptedHashing();


private:
    void createActions();
    void createMenus();
    void createToolBar();
    void startHash(QString &pathFile);
    void clear();
    void copyHash(QLineEdit *lineEdit, QString statusBarMsg);

    Ui::MainWindow *ui;

    Model *model;
    LogView *logView;


    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *openFileAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *cancelHashAct;
    QAction *logViewAct;
};

#endif // MAINWINDOW_H
