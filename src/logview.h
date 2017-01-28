#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QObject>
#include <QMainWindow>
#include <QTableView>
#include <QHeaderView>
#include <QAction>
#include <QToolBar>
#include <QClipboard>
#include <QApplication>
#include <QStatusBar>

#include <QDebug>

#include "model.h"

class LogView : public QMainWindow
{
    Q_OBJECT
public:
    explicit LogView(Model *model, QWidget *parent = 0);

signals:
    void closeLogViewer();

private slots:
    void copy();
    void clearLog();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void init();
    void createActions();
    void createToolBar();



    QTableView *view;
    Model *model;

    QToolBar *toolBar;
    QAction *copyAct;
    QAction *clearLogAct;
};

#endif // LOGVIEW_H
