#ifndef FEATURESEXTRACTOR_H
#define FEATURESEXTRACTOR_H

#endif // FEATURESEXTRACTOR_H
#include <fstream>
#include <QDialog>
#include <yin/Yin.h>
#include <vector>

class FeaturesExtractor
{
    std::vector<double> data;
    qreal peak;
    int sample_per_frame;
    int sampleRate;
public:
    std::vector<qint16>whole_signal;
    FeaturesExtractor(std::vector<qint16>whole_signal, std::vector<qint16>frame, qreal,int,int);
   ~ FeaturesExtractor();
    std::vector<double> calcF0(int frame_number);
    double calcEnergy();
    double calcZCR();
    std::vector<double> getData();
};
