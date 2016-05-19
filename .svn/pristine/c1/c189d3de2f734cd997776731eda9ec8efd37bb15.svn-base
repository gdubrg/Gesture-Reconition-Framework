#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T13:31:15
#
#-------------------------------------------------

QT       += core gui network
CONFIG  += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GRFramework
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    acquisition.cpp \
    acquisitionkinect1.cpp \
    OpenCVFrameHelper.cpp \
    OpenCVHelper.cpp \
    acquisitionkinect2.cpp \
    gesturerecognition.cpp \
    gesturerecognitionhmm.cpp \
    gmmstd_backward_GMM.cpp \
    gmmstd_baum_GMM.cpp \
    gmmstd_forward_GMM.cpp \
    gmmstd_gmm_tiny.cpp \
    gmmstd_hmm_gmm.cpp \
    gesturerecognitiondtw.cpp \
    guigesturerecognitionhmm.cpp \
    guigesturerecognitiondtw.cpp \
    qcustomplot.cpp \
    guigesturerecognitiontest.cpp \
    gesturerecognitiontest.cpp \
    gesturerecognitionsave.cpp \
    guigesturerecognitionsave.cpp \
    acquisitionfile.cpp \
    framesetsequence.cpp \
    trainingset.cpp \
    msrimporter.cpp \
    batchprocess.cpp \
    testingset.cpp \
    gesturerecognitionhmmoffline.cpp \
    getfeatures.cpp \
    msdhmm_dprob.cpp \
    msdhmm_hmm.cpp \
    gesturerecognitiondhmm.cpp

HEADERS  += mainwindow.h \
    frameset.h \
    acquisition.h \
    acquisitionkinect1.h \
    OpenCVFrameHelper.h \
    OpenCVHelper.h \
    resource.h \
    KinectHelper.h \
    acquisitionkinect2.h \
    stdafx.h \
    gesturerecognition.h \
    gesturerecognitionhmm.h \
    dirent.h \
    gmmstd_gmm_tiny.h \
    gmmstd_hmm_GMM.h \
    conversionutility.h \
    gesturerecognitiondtw.h \
    guigesturerecognitionhmm.h \
    guigesturerecognitiondtw.h \
    qcustomplot.h \
    guigesturerecognitiontest.h \
    gesturerecognitiontest.h \
    gesturerecognitionsave.h \
    guigesturerecognitionsave.h \
    acquisitionfile.h \
    framesetsequence.h \
    trainingset.h \
    msrimporter.h \
    testingset.h \
    coordinatesutility.h \
    gesturerecognitionhmmoffline.h \
    getfeatures.h \
    msdhmm_dprob.h \
    msdhmm_hmm.h \
    gesturerecognitiondhmm.h

FORMS    += mainwindow.ui \
    GUIgesturerecognitionhmm.ui \
    GUIgesturerecognitiondtw.ui \
    GUIgesturerecognitiontest.ui \
    GUIgesturerecognitionsave.ui

win32 {
    INCLUDEPATH += "C:\opencv\opencv\build\include" \

    CONFIG(debug,debug|release) {
        LIBS += -L"C:\opencv\opencv\build\x86\vc12\lib" \
            -lopencv_core2411d \
            -lopencv_highgui2411d \
            -lopencv_imgproc2411d \
            -lopencv_features2d2411d \
            -lopencv_calib3d2411d \
            -lopencv_ml2411d \
            -lopencv_video2411d \
            -lopencv_objdetect2411d \
            -lopencv_contrib2411d \
            -lopencv_legacy2411d \
            -lopencv_flann2411d
    }

    CONFIG(release,debug|release) {
        DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT
        LIBS += -L"C:\opencv\opencv\build\x86\vc12\lib" \
            -lopencv_core2411 \
            -lopencv_highgui2411 \
            -lopencv_imgproc2411 \
            -lopencv_features2d2411 \
            -lopencv_calib3d2411 \
            -lopencv_ml2411 \
            -lopencv_video2411 \
            -lopencv_objdetect2411 \
            -lopencv_contrib2411 \
            -lopencv_legacy2411 \
            -lopencv_flann2411
    }
}

win32 {
    INCLUDEPATH += "C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc" \

    CONFIG(debug,debug|release) {
        LIBS += -L"C:\Program Files\Microsoft SDKs\Kinect\v1.8\lib\x86" \
            -lKinect10 \

    }

    CONFIG(release,debug|release) {
        DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT
        LIBS += -L"C:\Program Files\Microsoft SDKs\Kinect\v1.8\lib\x86" \
            -lKinect10 \
    }
}


win32 {
    INCLUDEPATH += "C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\inc" \

    CONFIG(debug,debug|release) {
        LIBS += -L"C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x86" \
            -lKinect20 \
            -lKinect20.Face \

    }

    CONFIG(release,debug|release) {
        DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT
        LIBS += -L"C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Lib\x86" \
            -lKinect20 \
            -lKinect20.Face \
    }
}

DISTFILES +=
