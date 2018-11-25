#include<SingleSampleFeatures.h>

SingleSampleFeatures::SingleSampleFeatures()
{
}
void SingleSampleFeatures::buffer_emplace_back(double value)
{
    this->buffer.emplace_back(value);
}
size_t SingleSampleFeatures::buffer_size()
{
    return this->buffer.size();
}
double SingleSampleFeatures::buffer_value(int index)
{
    return this->buffer[index];
}
void SingleSampleFeatures::fftBuffer_emplace_back(std::vector<double> fft)
{
    for(std::vector<double>::iterator it=fft.begin();it!=fft.end();++it)
    {
            this->fftBuffer.emplace_back(*it);
    }
}
size_t SingleSampleFeatures::fftBuffer_size()
{
    return this->fftBuffer.size();
}
double SingleSampleFeatures::fftBuffer_value(int index)
{
    return this->fftBuffer[index];
}

void SingleSampleFeatures::energy_emplace_back(double energy)
{
    this->energy.emplace_back(energy);
}

size_t SingleSampleFeatures::energy_size()
{
    return this->energy.size();
}
double SingleSampleFeatures::energy_value(int index)
{
    return this->energy[index];
}

void SingleSampleFeatures::centroid_emplace_back(double centroid)
{
    this->centroid.emplace_back(centroid);
}
size_t SingleSampleFeatures::centroid_size()
{
    return this->centroid.size();
}
double SingleSampleFeatures::centroid_value(int index)
{
    return this->centroid[index];
}
void SingleSampleFeatures::zcr_emplace_back(double zcr)
{
    this->zcr.emplace_back(zcr);
}
size_t SingleSampleFeatures::zcr_size()
{
    return this->zcr.size();
}
double SingleSampleFeatures::zcr_value(int index)
{
    return this->zcr[index];
}
void SingleSampleFeatures::f0_emplace_back(std::vector<double> f0)
{
    for(std::vector<double>::iterator it=f0.begin();it!=f0.end();++it)
    {
            this->f0.emplace_back(*it);
    }
}
size_t SingleSampleFeatures::f0_size()
{
    return this->f0.size();
}
double SingleSampleFeatures::f0_value(int index)
{
    return this->f0[index];
}
