#-------------------------------------------------
#
# Project created by QtCreator 2019-05-31T08:45:52
#
#-------------------------------------------------

QT       += \
         core gui \
         opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shortest_path_search
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

CONFIG += c++14

SOURCES += \
        camera3d.cpp \
        generatespacedialog.cpp \
        input.cpp \
        loggerwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        oglwidget.cpp \
        result.cpp \
        resultwindow.cpp \
        scene.cpp \
        settings.cpp \
        transform3d.cpp \
        voxel.cpp

HEADERS += \
        astar.h \
        camera3d.h \
        djikstra.h \
        dstarlite.h \
        generatespacedialog.h \
        input.h \
        isearchalgorithm.h \
        ispacegenerator.h \
        loggerwidget.h \
        mainwindow.h \
        objectbuffers.h \
        oglwidget.h \
        result.h \
        resultwindow.h \
        scene.h \
        settings.h \
        transform3d.h \
        vector3d.h \
        voxel.h

FORMS += \
        generatespacedialog.ui \
        mainwindow.ui \
        resultwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    resources/shaders/main.frag \
    resources/shaders/main.vert
