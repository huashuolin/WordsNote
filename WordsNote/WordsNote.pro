#-------------------------------------------------
#
# Project created by QtCreator 2020-06-27T23:21:51
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wordsnote2
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
        MainWidget.cpp \
    ManageWidget.cpp \
    TextOperateWidget.cpp \
    DiaryManagerWidget.cpp \
    DocManagerWidget.cpp \
    DataManager.cpp \
    LoginWnd.cpp \
    RegisterWnd.cpp \
    LoginAdditionWidget.cpp \
    GlobalData.cpp

HEADERS += \
        MainWidget.h \
    ManageWidget.h \
    TextOperateWidget.h \
    DiaryManagerWidget.h \
    DocManagerWidget.h \
    log.h \
    DataManager.h \
    LoginWnd.h \
    RegisterWnd.h \
    LoginAdditionWidget.h \
    GlobalData.h \
    CommonDefine.h

FORMS += \
        MainWidget.ui \
    ManageWidget.ui \
    TextOperateWidget.ui \
    DiaryManagerWidget.ui \
    DocManagerWidget.ui \
    LoginWnd.ui \
    RegisterWnd.ui \
    LoginAdditionWidget.ui

RESOURCES += \
    WordsNote.qrc

ICON = images/WordsNote.icns
