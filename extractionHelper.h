#ifndef EXTRACTIONHELPER_H
#define EXTRACTIONHELPER_H

#include <fstream>


#include <QDialog>
#include <yin/Yin.h>
#include <vector>
#include <fftw/fftw3.h>
#define F0_MIN 60
#define F0_MAX 450

class ExtractionHelper
{
    std::vector<double> currentFrameData;
    qreal peak;
    int frameSize;
    int sampleRate;
    std::vector<double>whole_signal;
    std::vector<double>energyVec;
    std::vector<double>f0;
public:
    ExtractionHelper(std::vector<double>whole_signal, qreal,int,int);
    ExtractionHelper(){}
   ~ ExtractionHelper();
    void calcF0(int frame_number);
    void setCurrentFrameData(std::vector<double>frame)
    {
        this->currentFrameData=frame;
        for(int i = 0;i<currentFrameData.size();i++)
        {
            currentFrameData.at(i)/=peak;
        }
    }
    std::vector<double>getWholeSignal(){return this->whole_signal;}
    size_t f0_size();
    double f0_value(int index);
};
#endif // ExtractionHelper_H
