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
    static qint16 *whole_signal;
    static int whole_signal_size;
    //FeaturesExtractor( const qint16 *, qreal,int,int);
    FeaturesExtractor( std::vector<qint16>, qreal,int,int);
   ~ FeaturesExtractor();
    std::vector<double> calcF0(int frame_number);
    double calcEnergy();
    double calcZCR();
    std::vector<double> getData();
};
