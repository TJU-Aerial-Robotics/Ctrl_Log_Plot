QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Python.h以及arrayobject.h
INCLUDEPATH += include/ \
               /usr/include/python3.8

# /lib/x86_64-linux-gnu/libpython3.8.so 的位置
LIBS += -L/lib/x86_64-linux-gnu -lpython3.8

VPATH += src/

QMAKE_LFLAGS += -no-pie

SOURCES += \
    main.cpp \
    src/logplot.cpp \
    src/qcustomplot.cpp \
    src/pid_analyzer_interface.cpp

HEADERS += \
    include/logplot.h \
    include/qcustomplot.h \
    include/pid_analyzer_interface.h

FORMS += \
    logplot.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
