QT       += core gui opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 static release

QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++17

TEMPLATE = app

TARGET = 3D_Viewer

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/controller.cpp \
    main.cpp \
    mainwindow.cpp \
    ../model/facade/*.cpp \
    ../model/figure/*.cpp \
    ../model/file_reader/*.cpp \
    ../model/transform_matrix/*.cpp \
    view/view.cpp \
    view/widget_utils.cpp \
    view/settings_widget.cpp \
    view/qt_scene_drawer.cpp

HEADERS += \
    ../model/facade/*.h \
    ../model/figure/*.h \
    ../model/file_reader/*.h \
    ../model/transform_matrix/*.h \
    controller/controller.h \
    mainwindow.h \
    view/view.h \
    view/widget_utils.h \
    view/settings_widget.h \
    view/qt_scene_drawer.h

FORMS += \
    mainwindow.ui

include(QtGifImage/src/gifimage/qtgifimage.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += resources.qrc
