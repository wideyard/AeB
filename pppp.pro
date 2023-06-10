#-------------------------------------------------
#
# Project created by QtCreator 2022-04-26T18:42:03
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
CONFIG += resources_big

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


SOURCES += \
    main.cpp \
    bomb.cpp \
    player.cpp \
    item.cpp \
    envr.cpp \
    enemy.cpp\
    widget.cpp \
    login.cpp \
    figure.cpp \
    mianwork.cpp \
    singlework.cpp \
    end.cpp

HEADERS += \
    player.h \
    bomb.h \
    def.h \
    item.h \
    envr.h \
    enemy.h \
    widget.h \
    login.h \
    figure.h \
    mianwork.h \
    singlework.h \
    end.h

RESOURCES += \
    images.qrc \
    sound.qrc

DISTFILES +=

FORMS += \
    login.ui \
    figure.ui \
    mianwork.ui \
    singlework.ui \
    end.ui

STATECHARTS +=
