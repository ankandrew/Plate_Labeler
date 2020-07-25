#-------------------------------------------------
#
# Project created by QtCreator 2020-07-18T23:48:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plate_Labeling
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
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/build/x64/vc15/lib/ -lopencv_world440
#else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/build/x64/vc15/lib/ -lopencv_world440d
#else:unix: LIBS += -LD:/opencv/build/x64/vc15/lib/ -lopencv_world440


#Automat. added

# Debug


#win32: LIBS += -L$$PWD/../vcpkg/tesseract_x64-windows-static/lib/packages/x64-windows-static/debug/lib/ -ltesseract41d

#INCLUDEPATH += $$PWD/../vcpkg/installed/x64-windows-static/include
#DEPENDPATH += $$PWD/../vcpkg/installed/x64-windows-static/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../vcpkg/installed/x64-windows-static/debug/lib/tesseract41d.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../vcpkg/installed/x64-windows-static/debug/lib/libtesseract41d.a
