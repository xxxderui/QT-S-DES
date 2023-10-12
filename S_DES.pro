QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    a_figure.cpp \
    attack.cpp \
    choice.cpp \
    figure.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    a_figure.h \
    attack.h \
    choice.h \
    figure.h \
    mainwindow.h

FORMS += \
    a_figure.ui \
    attack.ui \
    choice.ui \
    figure.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
