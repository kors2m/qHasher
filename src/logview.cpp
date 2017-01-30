#include "logview.h"

LogView::LogView(Model *model, QWidget *parent) : QMainWindow(parent)
{
    this->model = model;

    init();

    createActions();
    createToolBar();
}

void LogView::init()
{
    view = new QTableView(this);
    view->setModel(model);
    view->setColumnHidden(Model::PATH, true);
    view->setColumnWidth(Model::FILENAME, 160);
    view->setColumnWidth(Model::CRC32, 65);
    view->setColumnWidth(Model::MD4, 230);
    view->setColumnWidth(Model::MD5, 230);
    view->setColumnWidth(Model::SHA1, 230);
    view->setColumnWidth(Model::SHA256, 230);
    view->setColumnWidth(Model::SHA512, 230);

    view->setShowGrid(false);
    view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);

    QHeaderView *horizontalHeader = view->horizontalHeader();
    horizontalHeader->setStretchLastSection(true);

    QHeaderView *verticalHeader = view->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(28);

    setWindowTitle(tr("Log Viewer - qHasher"));
    setMinimumSize(700, 300);
    setCentralWidget(view);
}

void LogView::createActions()
{
    copyAct = new QAction(tr("&Copy to clipboard"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setIcon(QIcon(":/icons/copy.png"));
    connect(copyAct, &QAction::triggered, this, &LogView::copy);

    clearLogAct = new QAction(tr("&Clear log"), this);
    clearLogAct->setShortcut(QKeySequence::Delete);
    clearLogAct->setIcon(QIcon(":/icons/delete.png"));
    connect(clearLogAct, &QAction::triggered, this, &LogView::clearLog);
}

void LogView::createToolBar()
{
    toolBar = new QToolBar();
    toolBar->setIconSize(QSize(16, 16));
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->addAction(copyAct);
    toolBar->addAction(clearLogAct);
}

void LogView::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit closeLogViewer();
}

void LogView::copy()
{
    QModelIndex index = view->selectionModel()->currentIndex();

    if (!index.isValid()) return;

    QString itm = model->data(index, Qt::DisplayRole).toString();

    if (itm.isEmpty()) return;

    QClipboard *clip = QApplication::clipboard();
    clip->setText(itm, QClipboard::Clipboard);
    clip->setText(itm, QClipboard::Selection);

    statusBar()->showMessage(tr("\"%1\" copied to the clipboard").arg(itm), 2000);
}

void LogView::clearLog()
{
    if (model->rowCount() == 0) return;
    model->removeAllRows();
    statusBar()->showMessage(tr("Clear logs is done"),  2000);
}
