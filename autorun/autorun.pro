QT += core gui

TARGET = autorun
TEMPLATE = app
DEFINES += WINVER=0x0501
LIBS += -lsetupapi -ladvapi32 -luser32

SOURCES += main.cpp\
    MainWindow.cpp \
    SerialPort.cpp \
    SerialPortWin.cpp \
    SerialEnumeratorWin.cpp

HEADERS  += MainWindow.h \
    SerialPort.h \
    SerialEnumerator.h
