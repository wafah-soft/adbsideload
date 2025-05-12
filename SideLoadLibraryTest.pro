QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  test\tst_sideloadlibraryhandletest.cpp

win32:LIBS += -LD:/Wafah/Dependencies/all-library/adbfastboot/ -lSideLoadLibrary
LIBS += --static "$$PWD/lib/prebuilt/usb/AdbWinApi.a"
