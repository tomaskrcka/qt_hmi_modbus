QT += quick
QT += sql
QT += network
QT += serialbus serialport
CONFIG += c++11

#QMAKE_CFLAGS += -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 \
#    -lmongoc-1.0 -lbson-1.0

INCLUDEPATH += /usr/local/include/libbson-1.0
INCLUDEPATH += /usr/local/include/libmongoc-1.0
LIBS += -L/usr/local/lib/test -lmongoc-1.0 -lbson-1.0


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/control/internalregisters.cpp \
    src/modules/cameramodule.cpp \
    src/control/processcontrol.cpp \
    src/http/httprestclient.cpp \
    src/appcontext.cpp \
    src/qmlconnection.cpp \
    src/storage/mongodb.cpp \
    src/settings/settings.cpp \ 
    src/modules/modbus.cpp \
    src/modules/restmodule.cpp \
    src/modules/cameramoduleweb.cpp

RESOURCES += src/qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/modules/cameramodule.h \
    src/control/internalregisters.h \
    src/appcontext.h \
    src/config.h \
    src/qmlconnection.h \
    src/tankitem.h \
    src/control/processcontrol.h \
    src/http/httprestclient.h \
    src/settings/settings.h \
    src/storage/mongodb.h \
    src/storage/storageinterface.h \
    src/settings/tanksetting.h \ 
    src/modules/modbus.h \
    src/settings/constants.h \
    src/modules/restmodule.h \
    src/modules/cameramoduleweb.h \
    src/modules/camerainterface.h

