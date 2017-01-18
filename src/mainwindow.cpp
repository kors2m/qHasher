#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

//    QString msg = tr("A qHashe is loaded");
//    statusBar()
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createActions()
{
    openFileAct = new QAction(tr("&Open"),this);
    openFileAct->setShortcut(QKeySequence::Open);
    openFileAct->setStatusTip(tr("Open a file"));
    connect(openFileAct,&QAction::triggered, this, &MainWindow::openFile);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct,&QAction::triggered,this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}


void MainWindow::openFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QDir::homePath());
    dialog.setWindowTitle(tr("Choose a file..."));

    // position a center
    dialog.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            dialog.size(),
            qApp->desktop()->availableGeometry()
        )
    );

    QString filename;
    if (dialog.exec())
        filename = dialog.selectedFiles().first();

    if (!filename.isEmpty())
    {
        ui->pathToFileLE->setText(filename);
//        fileopen()
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About qHasher"),
                       tr("<h2>qHasher </h2>"));
}

