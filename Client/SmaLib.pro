#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T13:32:33
#
#-------------------------------------------------

QT       += multimedia
QT       += multimediawidgets
QT       += core gui
QT       += sql
QT       += network
#QT       += webkit
QT       +=webkitwidgets
#QT       += webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmaLib
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    addbookdialog.cpp \
    addreaderdialog.cpp \
    newrecorddialog.cpp \
    informationdialog.cpp \
    modifyreaderdialog.cpp \
    modifybookdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    addbookdialog.h \
    addreaderdialog.h \
    newrecorddialog.h \
    informationdialog.h \
    modifyreaderdialog.h \
    modifybookdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    addbookdialog.ui \
    addreaderdialog.ui \
    newrecorddialog.ui \
    informationdialog.ui \
    modifyreaderdialog.ui \
    modifybookdialog.ui

RESOURCES += \
    images.qrc




