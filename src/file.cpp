#include "file.h"

File::File(const QString& filepath)
{
    err = QFileDevice::NoError;

    file     = new QFile(filepath);
    fileInfo = new QFileInfo(*file);

    getInfo();
}

File::~File()
{
    delete file;
}

void File::getInfo()
{
    if (!file->isOpen()) open();

    size       = fileInfo->size();
    attributes = getAttributes();
    created    = fileInfo->created();
    modified   = fileInfo->lastModified();
}

quint64 File::getSize()
{
    return size;
}

QString File::getAttributes()
{
    if (err == QFileDevice::NoError) {
        attributes = "";
        fileInfo->isReadable() ?   attributes += 'R' : attributes += '-';
        fileInfo->isWritable() ?   attributes += 'W' : attributes += '-';
        fileInfo->isExecutable() ? attributes += 'E' : attributes += '-';
        fileInfo->isHidden() ?     attributes += 'H' : attributes += '-';
    }
    return attributes;
}

QString File::getTimeCreated()
{
    if (err == QFileDevice::NoError) return created.toString("yyyy-MM-dd hh:mm:ss");

    return "";
}

QString File::getTimeModified()
{
    if (err == QFileDevice::NoError) return modified.toString("yyyy-MM-dd hh:mm:ss");

    return "";
}

QString File::getFilename() const
{
    return fileInfo->fileName();
}

QString File::getPath() const
{
    return fileInfo->filePath();
}

QString File::getErrorMsg() const
{
    return errMsg;
}

QFileDevice::FileError File::getError()
{
    return err;
}

bool File::open()
{
    if (!file->open(QIODevice::ReadOnly)) {
        errMsg = file->errorString();
        err    = file->error();
        return false;
    }
    return true;
}

qint64 File::read(char *data, qint64 maxSize)
{
    if (!file->isOpen()) open();

    return file->read(data, maxSize);
}

void File::close()
{
    if (!file->isOpen()) return;

    file->close();
}

QString File::getSizeFormat()
{
    if (err == QFileDevice::OpenError) return "-";

    if (size <= 0) return 0;

    QVector<QString> units;
    units << "B" << "kB" << "MB" << "GB" << "TB";
    int digitGroups = static_cast <int>(qLn(size) / qLn(1000)); // format in decimal
    return QString().setNum(size / qPow(1000, digitGroups), 'f',
                            1) + " " + units.at(digitGroups);
}
