QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
# my win32 specifics
    INCLUDEPATH += D:\Boost\Shared\include\boost-1_82
    LIBS += "-LD:\Boost\Shared\lib"
} else {
# All other platforms.
}

SOURCES += \
    main.cpp \
    Windows\adminwindow.cpp \
    Windows\loginrequest.cpp \
    Windows\messagewindow.cpp \
    Windows\operatorwindow.cpp \
    Windows\userwindow.cpp \
    Network\netcommunication.cpp \
    Network\requestmanager.cpp \


HEADERS += \
    Windows\adminwindow.h \
    Windows\loginrequest.h \
    Windows\messagewindow.h \
    Windows\operatorwindow.h \
    Windows\userwindow.h \
    Network\netcommunication.h \
    Network\requestmanager.h \
    data_types.h

FORMS += \
    Forms\adminwindow.ui \
    Forms\loginrequest.ui \
    Forms\messagewindow.ui \
    Forms\operatorwindow.ui \
    Forms\userwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
