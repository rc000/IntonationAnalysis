#include <iostream>
#include <fstream>

#include "featuresExtractor.h"
#include <QDialog>
#include <QDebug>
#define REAL 0
#define IMAG 1
#define F0_MIN 40
#define F0_MAX 450

qint16 *FeaturesExtractor::whole_signal = nullptr;
int FeaturesExtractor::whole_signal_size = 0;

FeaturesExtractor:: FeaturesExtractor( const qint16 *data, qreal peak ,int sample_per_frame,int sampleRate)
{
    this->peak = peak;
    this->sample_per_frame = sample_per_frame;
    this->sampleRate = sampleRate;
    for(size_t i=0;i<sample_per_frame;i++)
    {
        this->data.emplace_back(data[i]/peak);
    }
}
 FeaturesExtractor::~ FeaturesExtractor()
{
    //delete data;
}
 std::vector<double> FeaturesExtractor::getData()
 {
     return data;
 }
double  FeaturesExtractor::calcEnergy()
{
    double energy=0.0;
    for(int i=0;i<sample_per_frame;i++)
    {
        energy+=data[i]*data[i];
    }
    if (energy > 200)
    {
       return 0.0;
    }
    return energy;
}

std::vector<double> FeaturesExtractor::calcF0(int frame_number)
{
   int numberOfShifts=10;
   std::vector<double> f0;
   Yin m_yin(sample_per_frame, sampleRate, 0.15);
   int frame_start=frame_number*sample_per_frame;
   for(int i=0;i<numberOfShifts;i++)
   {
       int shift=i*(sample_per_frame/numberOfShifts);
       double *shift_frame =new double [sample_per_frame];
       int index=0;
       for(int j=frame_start+shift;j<frame_start+shift+sample_per_frame;j++)
       {
           if(j>=whole_signal_size)
           {
               break;
           }
           shift_frame[index] = whole_signal[j]/peak;
           index++;
       }
       Yin::YinOutput f0_struct=m_yin.process(shift_frame);
       if (f0_struct.f0 <F0_MAX && f0_struct.f0 >F0_MIN)
           f0.emplace_back(f0_struct.f0);
       else
           f0.emplace_back(0);
   }
    return f0;
}
double  FeaturesExtractor::calcZCR()
{
    double zcr=0.0;

    for (int j = 1; j < sample_per_frame; j++)
    {
        if ((data[j] * data[j - 1]) < 0)
            zcr++;
    }
    return zcr;
}


