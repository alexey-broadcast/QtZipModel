#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T23:56:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -L$$PWD/QuaZip/ -lquazip

DESTDIR = ..\bin

QMAKE_PRE_LINK = copy QuaZip\QuaZip.dll ..\bin\
QMAKE_PRE_LINK = copy ZipModel.zip ..\bin\

TARGET = ZipModel
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
    ZipFileInfo.cpp \
    ZipModel.cpp

HEADERS  += widget.h \
    ZipFileInfo.h \
    ZipModel.h

RESOURCES += res.qrc
