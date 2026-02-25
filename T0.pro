QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pakemonbox.cpp \
    picturepokemon.cpp

HEADERS += \
    mainwindow.h \
    pakemonbox.h \
    picturepokemon.h

FORMS += \
    mainwindow.ui \
    pakemonbox.ui \
    picturepokemon.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
