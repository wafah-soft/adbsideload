TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    SideLoadLibrary.pro \
    SideLoadLibraryTest.pro

SideLoadLibraryTest.depends = SideLoadLibrary.pro
