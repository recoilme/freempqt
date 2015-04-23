#-------------------------------------------------
#
# Project created by QtCreator 2015-04-13T18:50:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = freempqt
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    audiothread.cpp \
    track.cpp \
    trackwidget.cpp \
    trackdelegate.cpp \
    trackmodel.cpp

LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lbass

HEADERS  += mainwindow.h \
    bass.h \
    audiothread.h \
    scanner.h \
    track.h \
    trackwidget.h \
    trackdelegate.h \
    trackmodel.h

FORMS    += mainwindow.ui \
    item.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
