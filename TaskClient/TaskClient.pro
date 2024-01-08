QT       += core gui xml

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
    Data/collector.cpp \
    Data/data_keeper.cpp \
    Network/requestmanager_add.cpp \
    Network/requestmanager_del.cpp \
    Network/requestmanager_get.cpp \
    Network/requestmanager_set.cpp \
    Network/netcommunication.cpp \
    Network/requestmanager.cpp \
    Parser/replyparser.cpp \
    Windows/admin_functions.cpp \
    Windows/admin_slots.cpp \
    Windows/operator_slots.cpp \
    Windows/simple_delegate.cpp \
    Windows/user_slots.cpp \
    Windows/adminwindow.cpp \
    Windows/messagewindow.cpp \
    Windows/operatorwindow.cpp \
    Windows/userwindow.cpp \
    Windows/passwdwindow.cpp \
    main.cpp

HEADERS += \
    Data/collector.h \
    Data/data_collections.h \
    Data/data_keeper.h \
    Data/data_types.h \
    Parser/replyparser.h \
    Windows/simple_delegate.h \
    Windows/adminwindow.h \
    Windows/messagewindow.h \
    Windows/operatorwindow.h \
    Windows/userwindow.h \
    Windows/passwdwindow.h \
    Network/netcommunication.h \
    Network/requestmanager.h

FORMS += \
    Forms/passwdwindow.ui \
    Forms/adminwindow.ui \
    Forms/messagewindow.ui \
    Forms/operatorwindow.ui \
    Forms/userwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
