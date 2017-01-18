#include "test/hashertest.h"

void HasherTest::testMD4()
{
    File *file1 = new File(validPath);


    Hasher *hasher = new Hasher();

    QString res = hasher->calcHashes(file1);
    qDebug() << res;
}
