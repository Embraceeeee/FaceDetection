QT += quick

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

SOURCES += \
        imageprovider.cpp \
        main.cpp \
        showphoto.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT+=quickcontrols2
QT+=widgets

HEADERS += \
    imageprovider.h \
    showphoto.h

INCLUDEPATH += D:\OpenCv\Buildopencv\install\include

LIBS +=  D:\OpenCv\Buildopencv\install\x64\mingw\bin\libopencv_*

DISTFILES += \
    opencvFile/deploy.prototxt \
    opencvFile/res10_300x300_ssd_iter_140000_fp16.caffemodel
