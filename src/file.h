#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QtMath>

#include <QDebug>


class File : public QObject
{
    struct Hashes {
        QString md4;
        QString md5;
    };

public:
    File(const QString &filename);
    bool readFile();

    const QString getMD4();
    QString getSize();
    QString getAttributes();
    QString getTimeCreated();
    QString getTimeModified();
    QString getFilename() const;
    QString getError() const;

    void setMD4(const QString &hash);

private:

    QString sizeFormat(quint64 size);

    QFile *file;
    QFileInfo *fileInfo;
    QString path;
    quint64 size;
    QString attributes;
    QDateTime created;
    QDateTime modified;
    QFileDevice::FileError err;
    QString errMsg;

    Hashes hashes;

};

#endif // FILE_H
