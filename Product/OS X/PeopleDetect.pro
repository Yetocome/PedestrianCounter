#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T12:44:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PeopleDetect
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qwidgetdynamic.cpp \
    basis.cpp \
    blobfilter.cpp \
    fgseg.cpp \
    painter.cpp \
    pdclassifier.cpp \
    pdcounter.cpp \
    pddetector.cpp \
    preserver.cpp \
    tracker.cpp \
    utilities.cpp \
    dialog.cpp \
    captureloop.cpp

HEADERS  += mainwindow.h \
    qwidgetdynamic.h \
    basis.h \
    blobfilter.h \
    fgseg.h \
    painter.h \
    pdclassifier.h \
    pdcounter.h \
    pddetector.h \
    preserver.h \
    tracker.h \
    utilities.h \
    dialog.h \
    captureloop.h

FORMS    += mainwindow.ui \
    qwidgetdynamic.ui \
    dialog.ui

RESOURCES += \
    images.qrc \
    xml.qrc

INCLUDEPATH += /usr/local/include

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_calib3d.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_contrib.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_core.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_features2d.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_flann.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_gpu.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_highgui.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_imgproc.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_legacy.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_ml.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_nonfree.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_objdetect.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_ocl.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_photo.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_stitching.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_superres.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_video.2.4.12.dylib

LIBS += /usr/local/Cellar/opencv/HEAD/lib/libopencv_videostab.2.4.12.dylib

LIBS += -stdlib=libc++

QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -mmacosx-version-min=10.9
QMAKE_LFLAGS += -mmacosx-version-min=10.9
