#include<SingleFrameFeatures.h>

SingleFrameFeatures::SingleFrameFeatures()
{
}
void SingleFrameFeatures::buffer_emplace_back(double value)
{
    this->buffer.emplace_back(value);
}
size_t SingleFrameFeatures::buffer_size()
{
    return this->buffer.size();
}
double SingleFrameFeatures::buffer_value(int index)
{
    return this->buffer[index];
}

void SingleFrameFeatures::clear()
{
    buffer.clear();
    energy.clear();
    f0.clear();
    centroid.clear();
    zcr.clear();
}

void SingleFrameFeatures::energy_emplace_back(double energy)
{
    this->energy.emplace_back(energy);
}

size_t SingleFrameFeatures::energy_size()
{
    return this->energy.size();
}
double SingleFrameFeatures::energy_value(int index)
{
    return this->energy[index];
}

void SingleFrameFeatures::centroid_emplace_back(double centroid)
{
    this->centroid.emplace_back(centroid);
}
size_t SingleFrameFeatures::centroid_size()
{
    return this->centroid.size();
}
double SingleFrameFeatures::centroid_value(int index)
{
    return this->centroid[index];
}
void SingleFrameFeatures::zcr_emplace_back(double zcr)
{
    this->zcr.emplace_back(zcr);
}
size_t SingleFrameFeatures::zcr_size()
{
    return this->zcr.size();
}
double SingleFrameFeatures::zcr_value(int index)
{
    return this->zcr[index];
}
void SingleFrameFeatures::f0_emplace_back(std::vector<double> f0)
{
    for(std::vector<double>::iterator it=f0.begin();it!=f0.end();++it)
    {
            this->f0.emplace_back(*it);
    }
}
size_t SingleFrameFeatures::f0_size()
{
    return this->f0.size();
}
double SingleFrameFeatures::f0_value(int index)
{
    return this->f0[index];
}
