QT += quick concurrent

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
        header/Common \
        header/ImageCompressor \
        header/Controller \
        header/Model \
        usr/local/include

SOURCES += \
        source/Common/Utilities.cpp \
        source/Controller/ApplicationEngine.cpp \
        source/Controller/ProcessWorker.cpp \
        source/ImageCompressor/ImageCompressor.cpp \
        source/Model/ApplicationModel.cpp \
        source/Model/ApplicationSettings.cpp \
        source/Model/Events.cpp \
        source/Model/GUIModel.cpp \
        source/main.cpp

RESOURCES += \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

TARGET = AstcLz4Gui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    header/Common/Common.h \
    header/Common/Utilities.h \
    header/Controller/ApplicationEngine.h \
    header/Controller/ProcessWorker.h \
    header/ImageCompressor/ImageCompressor.h \
    header/Model/ApplicationModel.h \
    header/Model/ApplicationSettings.h \
    header/Model/Events.h \
    header/Model/GUIModel.h

LIBS += -L"/usr/local/lib" -llz4
