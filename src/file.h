#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QFile>
#include <QFileInfo>
#include <QtMath>

#include <QDebug>

class File : public QObject
{
public:
    explicit File(const QString &filename);
    ~File();

    qint64 read(char *data, qint64 maxSize);
    void close();

    quint64 getSize();
    QString getSizeFormat();
    QString getAttributes();
    QString getTimeCreated();
    QString getTimeModified();
    QString getFilename() const;
    QString getPath() const;
    QString getErrorMsg() const;
    QFileDevice::FileError getError();

private:
    void getInfo();
    bool open();

    QFile *file;
    QFileInfo *fileInfo;

    QString filename;
    QString path;
    quint64 size;
    QString attributes;
    QDateTime created;
    QDateTime modified;

    QFileDevice::FileError err;
    QString errMsg;
};

#endif // FILE_H
