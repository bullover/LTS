#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T14:36:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = LTSCore
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++1y

SOURCES += ltscore.cpp \
    bin/frombin.cpp \
    bin/tobin.cpp \
    config/configreader.cpp \
    container/updateinfo.cpp \
    Corvus/covuscontrol.cpp \
    Error/LTSError.cpp \
    log/Logger.cpp \
    pugi/pugixml.cpp \
    Socket/TCPAcceptor.cpp \
    Socket/TCPConnector.cpp \
    Socket/TCPStream.cpp \
    Socket/TuxServer.cpp \
    Tuxdaqctrl/LTS_DAQCtrl.cpp \
    dummytux.cpp

HEADERS += ltscore.h \
    bin/frombin.h \
    bin/tobin.h \
    config/Config_t.h \
    config/configreader.h \
    container/updatecollection.h \
    container/updateinfo.h \
    Corvus/covuscontrol.h \
    Error/LTSError.h \
    log/Logger.h \
    pugi/pugiconfig.hpp \
    pugi/pugixml.hpp \
    Socket/TCPAcceptor.h \
    Socket/TCPConnector.h \
    Socket/TCPStream.h \
    Socket/TuxServer.h \
    Tuxdaqctrl/LTS_DAQCtrl.h \
    Tuxdaqctrl/TuxDAQCMD.h \
    dummytux.h \
    LTSctrl.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
