#-------------------------------------------------
#
# Project created by QtCreator 2018-07-26T13:37:22
#
#-------------------------------------------------

QT       += core gui charts
QT += printsupport


QT       += multimedia

LIBS += -L"$$PWD/fftw"
LIBS+=libfftw3-3.lib
LIBS+=libfftw3f-3.lib
LIBS+=libfftw3l-3.lib
LIBS+= -L"$$PWD/lib"



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeakerRecognizer
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    featureExtractor.cpp \
    yin/yin.cpp \
    yin/yinutil.cpp \
    featureExtractor.cpp \
    yin/fft.cpp \
    main.cpp \
    mainwindow.cpp \
    yin/yin.cpp \
    yin/yinutil.cpp \
    yin/fft.cpp \
    yin/fftimpl.cpp \
    yin/Yin.cpp \
    yin/YinUtil.cpp \
    featureExtractor.cpp \
    main.cpp \
    mainwindow.cpp \
    yin/fft.cpp \
    yin/fftimpl.cpp \
    yin/Yin.cpp \
    yin/YinUtil.cpp \
    yin/ext/kiss_fft.c \
    yin/ext/kiss_fftr.c \
    borderlayout.cpp \
    singlesframefeatures.cpp \
    classificator.cpp \
    contoursextractor.cpp \
    singlesegment.cpp

HEADERS += \
        mainwindow.h \
    featuresExtractor.h \
    yin/fft.h \
    yin/yin.h \
    yin/meanfilter.h \
    yin/yinutil.h \
    fftw/fftw3.h \
    FeaturesExtractor.h \
    yin/fft.h \
    mainwindow.h \
    yin/meanfilter.h \
    yin/yin.h \
    yin/yinutil.h \
    fftw/fftw3.h \
    yin/FFT.h \
    yin/MeanFilter.h \
    yin/plugguard.h \
    yin/Yin.h \
    yin/YinUtil.h \
    FeaturesExtractor.h \
    mainwindow.h \
    yin/ext/_kiss_fft_guts.h \
    yin/ext/_kiss_fft_guts.h \
    yin/ext/kiss_fft.h \
    yin/ext/kiss_fftr.h \
    yin/FFT.h \
    yin/MeanFilter.h \
    yin/plugguard.h \
    yin/Yin.h \
    yin/YinUtil.h \
    borderlayout.h \
    singleframefeatures.h \
    classificator.h \
    contoursextractor.h \
    singlesegment.h

FORMS += \
    mainwindow.ui
target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
