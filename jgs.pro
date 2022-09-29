QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3rdParts/dragger/draggerwidget.cpp \
    3rdParts/myjgs/src/block.cpp \
    3rdParts/myjgs/src/event.cpp \
    3rdParts/myjgs/src/game.cpp \
    3rdParts/myjgs/src/item.cpp \
    3rdParts/myjgs/src/player.cpp \
    3rdParts/myjgs/src/type.cpp \
    3rdParts/myjgs/utils/config.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    3rdParts/dragger/draggerwidget.h \
    3rdParts/myjgs/3rdParts/meojson/include/json.hpp \
    3rdParts/myjgs/src/block.h \
    3rdParts/myjgs/src/event.h \
    3rdParts/myjgs/src/game.h \
    3rdParts/myjgs/src/item.h \
    3rdParts/myjgs/src/myjgs.h \
    3rdParts/myjgs/src/player.h \
    3rdParts/myjgs/src/type.h \
    3rdParts/myjgs/utils/config.h \
    mainwindow.h

FORMS += \
    3rdParts/dragger/draggerwidget.ui \
    mainwindow.ui

INCLUDEPATH += \
    3rdParts/dragger/ \
    3rdParts/myjgs/src \
    3rdParts/myjgs/utils \
    3rdParts/myjgs/3rdParts/meojson/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    3rdParts/myjgs/src/CMakeLists.txt
