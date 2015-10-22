#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T16:43:04
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = TuneEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tunepusher.cpp

HEADERS  += mainwindow.h \
    tunepusher.h

FORMS    += mainwindow.ui

RESOURCES += \
    javasc.qrc \
    images.qrc

DISTFILES += \
    kantele.png
