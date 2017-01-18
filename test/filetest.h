#ifndef FILETEST_H
#define FILETEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "src/file.h"


class FileTest : public QObject
{
    Q_OBJECT

private slots:

    void testReadFile();

    void testFormatSize();

    void testAttributes();

    void testCreatedAndModified();

private:
    const QString validPath = "/home/kors2m/Development/qHasher/qHasher/test/fixtures/349795.jpg";
    const QString invalidPath = "/home/kors2m/Development/qHasher/qHasher/test/fixtures/349795.jpg??";
    const QString invalidPath2 = "";
};



#endif // FILETEST_H
