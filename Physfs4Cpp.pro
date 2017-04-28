#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T19:04:54
#
#-------------------------------------------------

QT       -= core gui

TARGET = Physfs4Cpp
TEMPLATE = lib

DEFINES += PHYSFS4CPP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lphysfs
INCLUDEPATH += $$PWD/include

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Physfs4Cpp.cpp

HEADERS += \
    include/Physfs4Cpp.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    FindPhysFS.cmake \
    CMakeLists.txt
