#include "file.h"

File::File(const QString &filepath)
{
    path = filepath;
    err = QFileDevice::NoError;
    size = 0;
    attributes = "----";

    readFile();
}

void File::setMD4(const QString &hash)
{
    hashes.md4 = hash;
}


const QString File::getMD4()
{
    return hashes.md4;
}


QString File::getSize()
{
    if (err == QFileDevice::OpenError)
        return "-";
    return sizeFormat(size);
}

QString File::getAttributes()
{
    if (err == QFileDevice::NoError)
    {
        attributes = "";
        fileInfo->isReadable() ? attributes += 'R' : attributes += '-';
        fileInfo->isWritable() ? attributes += 'W' : attributes += '-';
        fileInfo->isExecutable() ? attributes += 'E' : attributes += '-';
        fileInfo->isHidden() ? attributes += 'H' : attributes += '-';
    }
    return attributes;
}

QString File::getTimeCreated()
{
    if (err == QFileDevice::NoError)
    {
        return created.toString("yyyy-MM-dd hh:mm:ss");
    }
    return "";
}

QString File::getTimeModified()
{
    if (err == QFileDevice::NoError)
    {
        return modified.toString("yyyy-MM-dd hh:mm:ss");
    }
    return "";
}

QString File::getFilename() const
{
    return path;
}

QString File::getError() const
{
    return errMsg;
}

bool File::readFile()
{
    file = new QFile(path);

    if(!file->open(QIODevice::ReadOnly))
    {
        errMsg = file->errorString();
        err = file->error();
        return false;
    }

    fileInfo = new QFileInfo(*file);

    size = fileInfo->size();
    attributes = getAttributes();
    created = fileInfo->created();
    modified = fileInfo->lastModified();

    file->close();
    return true;
}

QString File::sizeFormat(quint64 size)
{
    if (size <= 0) return 0;
    QVector<QString> units;
    units << "B" << "kB" << "MB" << "GB" << "TB";
    int digitGroups = (int) (qLn(size)/qLn(1000)); // format in decimal
    return QString().setNum(size/qPow(1000, digitGroups), 'f', 1) + " " + units.at(digitGroups);
}
