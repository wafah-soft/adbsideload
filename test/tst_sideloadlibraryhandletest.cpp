#include <QtTest>
#include "src/sideloadlib.h"

// add necessary includes here

class SideLoadLibraryHandleTest : public QObject
{
    Q_OBJECT

public:
    SideLoadLibraryHandleTest();
    ~SideLoadLibraryHandleTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void sideload_command_test();

};

SideLoadLibraryHandleTest::SideLoadLibraryHandleTest()
{

}

SideLoadLibraryHandleTest::~SideLoadLibraryHandleTest()
{

}

void SideLoadLibraryHandleTest::initTestCase()
{

}

void SideLoadLibraryHandleTest::cleanupTestCase()
{

}

void SideLoadLibraryHandleTest::sideload_command_test()
{

    QByteArray output;
    int *stop_flag = 0;
    wait_for_sideload_device(stop_flag);
    sideload_send_command("getdevice", output);
    qInfo() << output;
    sideload_send_command("getcodebase", output);
    qInfo() << output;
    sideload_send_command("getversion", output);
    qInfo() << output;

    sideload_send_command("getmitoken", output);
    qInfo() << output;
}

QTEST_APPLESS_MAIN(SideLoadLibraryHandleTest)

#include "tst_sideloadlibraryhandletest.moc"
