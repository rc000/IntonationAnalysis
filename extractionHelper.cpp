#include <iostream>
#include <fstream>

#include "extractionHelper.h"
#include <QDialog>
#include <QDebug>
#include "yin/YinUtil.h"
#define REAL 0
#define IMAG 1
#define F0_MIN 60
#define F0_MAX 450



ExtractionHelper:: ExtractionHelper( std::vector<double> whole_signal,  qreal peak ,int frameSize,int sampleRate)
{
    this->peak = peak;
    this->frameSize = frameSize;
    this->sampleRate = sampleRate;
     for(size_t i=0;i<whole_signal.size();i++)
    {
        this->whole_signal.emplace_back(whole_signal[i]/peak );
    }
}
 ExtractionHelper::~ ExtractionHelper()
{
    //delete data;
}


void ExtractionHelper::calcF0(int numberOfFrames)
{
   int numberOfShifts=10;
   Yin m_yin(frameSize, sampleRate);

   int frameStartIndexAfterShifting = 0;
   int shift= frameSize/numberOfShifts;

   while(frameStartIndexAfterShifting < (whole_signal.size()))
   {
       m_yin.setFrameSize(frameSize);
       double *shift_frame =new double [frameSize];
       int index=0;
       frameStartIndexAfterShifting +=shift;
       for(int k=frameStartIndexAfterShifting;k<frameStartIndexAfterShifting+frameSize;k++)
       {
           if(k>=whole_signal.size())
              shift_frame[index] = 0;
           else
              shift_frame[index] = whole_signal.at(k);
           index++;
       }
       Yin::YinOutput f0_struct=m_yin.process(shift_frame);
       if (f0_struct.f0 <F0_MAX && f0_struct.f0 >F0_MIN)
           f0.emplace_back(f0_struct.f0);
       else
           f0.emplace_back(0);
       delete shift_frame;
   }

}


size_t ExtractionHelper::f0_size()
{
    return this->f0.size();
}
double ExtractionHelper::f0_value(int index)
{
    return this->f0[index];
}

