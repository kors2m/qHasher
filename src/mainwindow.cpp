#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowCloseButtonHint); // is not working on Linux
    setWindowTitle(tr("qHasher %1").arg(QHASHER_VERSION));

    createActions();
    createMenus();
    createToolBar();

    connect(ui->rehashBtn, &QPushButton::clicked, this, &MainWindow::rehash);

    // set copy event
    ui->crc32LE->installEventFilter(this);
    ui->md4LE->installEventFilter(this);
    ui->md5LE->installEventFilter(this);

    ui->progressBar->reset();

    statusBar()->showMessage(tr("qHasher is ready"), 2000);

    model = new Model();
    connect(model, &Model::dataChanged, this, &MainWindow::update);
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
    openFileAct = new QAction(tr("&Open"), this);
    openFileAct->setShortcut(QKeySequence::Open);
    openFileAct->setStatusTip(tr("Open a file"));
    openFileAct->setIcon(QIcon(":/icons/addFile.png"));
    connect(openFileAct, &QAction::triggered, this, &MainWindow::openFile);

    cancelHashAct = new QAction(tr("Cancel hashing file"));
    cancelHashAct->setIcon(QIcon(":/icons/stop.png"));
    cancelHashAct->setEnabled(false);

    logViewAct = new QAction(tr("Log hashing files"));
    logViewAct->setIcon(QIcon(":/icons/log.png"));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createToolBar()
{
    ui->mainToolBar->setIconSize(QSize(36, 36));
    ui->mainToolBar->addAction(openFileAct);
    ui->mainToolBar->addAction(logViewAct);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(cancelHashAct);
}

void MainWindow::openFile()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QDir::homePath());
    dialog.setWindowTitle(tr("Choose a file..."));

    // position a center
    dialog.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                           dialog.size(),
                                           qApp->desktop()->availableGeometry()));

    QString filePath;

    if (dialog.exec())
        filePath = dialog.selectedFiles().first();

    if (!filePath.isEmpty()) {
        clear();
        ui->pathToFileLE->setText(filePath);
        startHash(filePath);
    }
}

void MainWindow::rehash()
{
    QString path = ui->pathToFileLE->text();

    clear();
    ui->pathToFileLE->setText(path);
    ui->pathToFileLE->setFocus();
    startHash(path);
}

void MainWindow::done()
{
    ui->statusBar->showMessage(tr("File hashing is finished"), 2000);
    openFileAct->setEnabled(true);
    cancelHashAct->setEnabled(false);
    ui->rehashBtn->setEnabled(true);
    ui->pathToFileLE->setFocus();
}

void MainWindow::update(const QModelIndex &index)
{
    switch (index.column()) {
    case Model::CRC32:
        ui->crc32LE->setText(model->data(index).toString());
        break;

    case Model::MD4:
        ui->md4LE->setText(model->data(index).toString());
        break;

    case Model::MD5:
        ui->md5LE->setText(model->data(index).toString());
        break;
    }
}

void MainWindow::about()
{
    QString ret = QString(
                    "<b>qHasher</b> &copy; 2017 Manko Mikhail <br /> &lt;mnk-dev@yandex.ru&gt;<br><br>"
                    "<b>" + tr("Version: %1").arg(QHASHER_VERSION) + "</b>"
                    "<br />" + tr("Using Qt %1 (compiled with Qt %2)").arg(qVersion()).arg(QT_VERSION_STR));
    QMessageBox::about(this, tr("About qHasher"), ret );
}

void MainWindow::startHash(QString &path)
{
    QList<EAlgHash::Algs> selectedHashes;

    int cnt = 0;

    if (ui->crc32CB->isChecked()) {
        selectedHashes.append(EAlgHash::CRC32);
        ++cnt;
    }
    if (ui->md4CB->isChecked()) {
        selectedHashes.append(EAlgHash::MD4);
        ++cnt;
    }
    if (ui->md5CB->isChecked()) {
        selectedHashes.append(EAlgHash::MD5);
        ++cnt;
    }

    if (cnt == 0) return;

    if (path.isEmpty()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           tr("Specify path to the file."));
        msgBox.exec();
        return;
    }

    File *file = new File(path);

    if (file->getError() == QFileDevice::OpenError) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), file->getErrorMsg());
        msgBox.exec();
        return;
    }

    QThread *hThread = new QThread;
    Hasher *hasher = new Hasher(model);

    hasher->addFile(file, selectedHashes);
    hasher->moveToThread(hThread);

    ui->fileSizeLE->setText(file->getSizeFormat());
    ui->attributesLE->setText(file->getAttributes());
    ui->createdLE->setText(file->getTimeCreated());
    ui->modifiedLE->setText(file->getTimeModified());
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(cnt);
    ui->statusBar->showMessage(tr("File hashing..."));
    ui->rehashBtn->setEnabled(false);
    cancelHashAct->setEnabled(true);
    openFileAct->setEnabled(false);

    connect(cancelHashAct, &QAction::triggered, hThread, &QThread::requestInterruption);
    connect(hasher, &Hasher::interrupted, this, &MainWindow::interruptedHashing);
    connect(hasher, &Hasher::valChangedProgressBar, ui->progressBar, &QProgressBar::setValue);
    connect(hThread, &QThread::started, hasher, &Hasher::calcHashes);
    connect(hasher, &Hasher::finished, this, &MainWindow::done);
    connect(hasher, &Hasher::finished, hThread, &QThread::quit);
    connect(hasher, &Hasher::finished, hasher, &Hasher::deleteLater);
    connect(hThread, &QThread::finished, hThread, &QThread::deleteLater);

    hThread->start();
}

void MainWindow::interruptedHashing()
{
    ui->statusBar->showMessage(tr("File hashing is interruted"), 2000);
    ui->progressBar->reset();
    ui->rehashBtn->setEnabled(true);
    cancelHashAct->setEnabled(false);
    openFileAct->setEnabled(true);
    ui->pathToFileLE->setFocus();
}

void MainWindow::clear()
{
    ui->pathToFileLE->clear();
    ui->fileSizeLE->clear();
    ui->attributesLE->clear();
    ui->createdLE->clear();
    ui->modifiedLE->clear();
    ui->crc32LE->clear();
    ui->md4LE->clear();
    ui->md5LE->clear();
    ui->progressBar->reset();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn) {
        if (obj == ui->crc32LE) {
            copyHash(ui->crc32LE, tr("CRC32 copied to the clipboard"));
            return true;
        }
        if (obj == ui->md4LE) {
            copyHash(ui->md4LE, tr("MD4 copied to the clipboard"));
            return true;
        }
        if (obj == ui->md5LE) {
            copyHash(ui->md5LE, tr("MD5 copied to the clipboard"));
            return true;
        }
    }
    return false;
}

void MainWindow::copyHash(QLineEdit *lineEdit, QString statusBarMsg)
{
    QString hash = lineEdit->text();

    if (hash.isEmpty())
        return;

    QClipboard *clip = QApplication::clipboard();
    clip->setText(hash);

    // select text and copy Clipboard::Selection (on linux)
    // because mousePressEvent trigers right after focusInEvent
    // o text selected in focusInEvent unselects by mousePressEvent.
    QTimer::singleShot(0, lineEdit, SLOT(selectAll()));
    ui->statusBar->showMessage(statusBarMsg, 2000);
}
