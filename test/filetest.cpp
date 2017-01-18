#include "test/filetest.h"

void FileTest::testReadFile()
{
    File file1(validPath);
    QVERIFY2(file1.readFile(), qPrintable(QString("open %1: %2")
                                          .arg(file1.getFilename())
                                          .arg(file1.getError())) );

    File file2(invalidPath);
    QVERIFY2(!file2.readFile(), qPrintable(QString("open %1: %2")
                                          .arg(file2.getFilename())
                                          .arg(file2.getError())) );

    File file3(invalidPath2);
    QVERIFY2(!file3.readFile(), qPrintable(QString("open %1: %2")
                                          .arg(file3.getFilename())
                                          .arg(file3.getError())) );

}

void FileTest::testFormatSize()
{
    File file1(validPath);
    file1.readFile();
    QCOMPARE(file1.getSize(), QString("1.6 MB"));

    File file2(invalidPath);
    file2.readFile();
    QCOMPARE(file2.getSize(), QString("-"));


}

void FileTest::testAttributes()
{
    File file1(validPath);
    file1.readFile();
    QCOMPARE(file1.getAttributes(), QString("RW--"));

    File file2(invalidPath);
    file2.readFile();
    QCOMPARE(file2.getAttributes(), QString("----"));
}

void FileTest::testCreatedAndModified()
{
    File file1(validPath);
    file1.readFile();
    QCOMPARE(file1.getTimeCreated(), QString("2017-01-17 06:26:38"));
    QCOMPARE(file1.getTimeModified(), QString("2014-11-25 17:31:21"));

    File file2(invalidPath);
    file2.readFile();
    QCOMPARE(file2.getTimeCreated(), QString(""));
    QCOMPARE(file2.getTimeModified(), QString(""));

}

//QTEST_APPLESS_MAIN(FileTest)
//#include "filetest.moc"
