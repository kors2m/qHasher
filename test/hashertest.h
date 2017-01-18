#ifndef HASHERTEST_H
#define HASHERTEST_H

#include <QtTest/QtTest>
#include <QObject>

#include "src/hasher.h"
//#include

class HasherTest : public QObject
{
    Q_OBJECT

private slots:
    void testMD4();

private:
    const QString validPath = "/home/kors2m/Development/qHasher/qHasher/test/fixtures/349795.jpg";
    const QString invalidPath = "/home/kors2m/Development/qHasher/qHasher/test/fixtures/349795.jpg??";
};


#endif // HASHERTEST_H
