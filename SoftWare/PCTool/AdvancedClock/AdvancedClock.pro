QT       += core gui network concurrent
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ControlDevice.cpp \
    OTA.cpp \
    PictureShow.cpp \
    TimeShow.cpp \
    easylogging++.cpp \
    main.cpp \
    AdvancedClock.cpp

HEADERS += \
    AdvancedClock.h \
    Cmd.h \
    ControlDevice.h \
    OTA.h \
    PictureShow.h \
    TimeShow.h \
    easylogging++.h

FORMS += \
    AdvancedClock.ui \
    ControlDevice.ui \
    OTA.ui \
    PictureShow.ui \
    TimeShow.ui

RC_ICONS = AdvancedClock.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resource.qrc
