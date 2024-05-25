QT += core gui charts

QT += sql
LIBS += -L"C:/Program Files/PostgreSQL/16/lib" -lpq

QT += widgets texttospeech
requires(qtHaveModule(gui))
qtHaveModule(gui): requires(qtConfig(combobox))

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    charts.cpp \
    databasehandler.cpp \
    login.cpp \
    main.cpp \
    index.cpp

HEADERS += \
    charts.h \
    databasehandler.h \
    index.h \
    login.h

FORMS += \
    index.ui \
    login.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Materials.qrc
