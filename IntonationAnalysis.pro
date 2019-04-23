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
QT_DEBUG_PLUGINS=1
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
    yin/yin.cpp \
    yin/yinutil.cpp \
    yin/fft.cpp \
    yin/fftimpl.cpp \
    classificator.cpp \
    borderlayout.cpp \
    fftt.cpp \
    yin/ext/kiss_fft.c \
    yin/ext/kiss_fftr.c \
    extractionHelper.cpp \
    segment.cpp \
    segmentsdetector.cpp \
    classificatornew.cpp

HEADERS += \
        mainwindow.h \
    yin/fft.h \
    yin/yin.h \
    yin/meanfilter.h \
    yin/yinutil.h \
    yin/meanfilter.h \
    yin/FFT.h \
    yin/MeanFilter.h \
    yin/plugguard.h \
    classificator.h \
    borderlayout.h \
    yin/ext/_kiss_fft_guts.h \
    yin/ext/kiss_fft.h \
    yin/ext/kiss_fftr.h \
    borderlayout.h \
    fftt.h \
    fftw/fftw3.h \
    extractionHelper.h \
    segment.h \
    segmentsdetector.h \
    classificatornew.h

FORMS += \
    mainwindow.ui
target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
