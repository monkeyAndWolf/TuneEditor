#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T16:43:04
#
#-------------------------------------------------

QT       += core gui svg printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuneEditor
TEMPLATE = app

mac {
    ICON = kantele.icns
}


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

INCLUDEPATH += ../ABCShot
LIBS += -L../ShotGlass -lABCShots
