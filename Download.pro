#-------------------------------------------------
#
# Project created by QtCreator 2018-02-12T15:28:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Download
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    urlframe.cpp \
    net_work_manager.cpp

HEADERS += \
        widget.h \
    urlframe.h \
    net_work_manager.h

FORMS += \
        widget.ui \
    urlframe.ui

RESOURCES += \
    res.qrc

LIBS += -L$$PWD -llibeay32 \
    -L$$PWD -lssleay32

DISTFILES += \
    libeay32.dll \
    ssleay32.dll
