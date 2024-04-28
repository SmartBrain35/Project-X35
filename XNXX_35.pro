INCLUDEPATH += D:/MySQL/MySQL Connector C++ 8.3/include/jdbc
LIBS += -L/D:/MySQL/MySQL Connector C++ 8.3/lib64

QT += core gui sql charts

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
    database_manager.cpp \
    login.cpp \
    main.cpp \
    index.cpp

HEADERS += \
    charts.h \
    database_manager.h \
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
