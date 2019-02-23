#ifndef FEATURESEXTRACTOR_H
#define FEATURESEXTRACTOR_H

#endif // FEATURESEXTRACTOR_H
#include <fstream>
#include <QDialog>
#include <yin/Yin.h>
#include <vector>
#include <fftw/fftw3.h>

class FeaturesExtractor
{
    std::vector<double> data;
    qreal peak;
    int sample_per_frame;
    int sampleRate;
    std::vector<double>whole_signal;
    double *fft;
public:
    FeaturesExtractor(std::vector<double>whole_signal, std::vector<double>frame, qreal,int,int);
   ~ FeaturesExtractor();
    std::vector<double> calcF0(int frame_number);
    std::vector<double> FeaturesExtractor::calcFFT();

    double calcEnergy();
    double calcZCR();
    std::vector<double> getData();
};
