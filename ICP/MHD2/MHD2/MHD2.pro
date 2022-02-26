#-------------------------------------------------
#
# Project created by QtCreator 2020-04-28T19:30:27
#
#-------------------------------------------------

QT       += core gui



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



QMAKE_CXXFLAGS += -std=c++17

TARGET = MHD2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicscene.cpp \
    Coordinate.cpp \
    Stop.cpp \
    Street.cpp \
    line.cpp \
    bus.cpp \
    traffic_t.cpp

HEADERS  += mainwindow.h \
    graphicscene.h \
    Coordinate.h \
    Stop.h \
    Street.h \
    line.h \
    bus.h \
    traffic_t.h

FORMS    += mainwindow.ui
