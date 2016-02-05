#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T15:09:55
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LTSgui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp\
        gui.cpp \
    subgui/corvusverification.cpp \
    subgui/posdraft.cpp \
    loginterface/logtote.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS  += gui.h \
    subgui/corvusverification.h \
    subgui/posdraft.h \
    loginterface/logtote.h \
    qcustomplot/qcustomplot.h

FORMS    += gui.ui \
    subgui/corvusverification.ui \
    subgui/posdraft.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LTSCore/release/ -lLTSCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LTSCore/debug/ -lLTSCore
else:unix: LIBS += -L$$OUT_PWD/../LTSCore/ -lLTSCore

INCLUDEPATH += $$PWD/../LTSCore
DEPENDPATH += $$PWD/../LTSCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LTSCore/release/libLTSCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LTSCore/debug/libLTSCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LTSCore/release/LTSCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LTSCore/debug/LTSCore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../LTSCore/libLTSCore.a
