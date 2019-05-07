#-------------------------------------------------
#
# Project created by QtCreator 2019-01-10T13:13:43
#
#-------------------------------------------------

QT       += sql charts core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECGDatabase
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dao.cpp \
    open.cpp \
    currentsignal.cpp \
    ecgchartview.cpp \
    ecggraphicdrawer.cpp \
    callout.cpp \
    addfile.cpp \
    showresult.cpp \
    filemanager.cpp \
    referencemanager.cpp \
    copyprogress.cpp \
    messageboxcreator.cpp \
    receivegraphictask.cpp \
    settings.cpp

HEADERS += \
    globalvariable.h \
        mainwindow.h \
    dao.h \
    open.h \
    currentsignal.h \
    ecgchartview.h \
    ecggraphicdrawer.h \
    callout.h \
    addfile.h \
    showresult.h \
    filemanager.h \
    referencemanager.h \
    copyprogress.h \
    messageboxcreator.h \
    receivegraphictask.h \
    settings.h

FORMS += \
        mainwindow.ui \
    open.ui \
    allresults.ui \
    addfile.ui \
    showresult.ui \
    copyprogress.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
