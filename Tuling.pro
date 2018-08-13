#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T21:39:42
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tuling
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        tuwidget.cpp \
    JasonQt_Net.cpp \
    JasonQt_Vop.cpp \
    jasonqt_tuling.cpp \
    jsonqt_voc.cpp

HEADERS += \
        tuwidget.h \
    JasonQt_Net.h \
    JasonQt_Vop.h \
    jasonqt_tuling.h \
    jsonqt_voc.h

FORMS += \
        tuwidget.ui

RESOURCES += \
    img.qrc
