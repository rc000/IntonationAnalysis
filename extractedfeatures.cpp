#include<ExtractedFeatures.h>

ExtractedFeatures::ExtractedFeatures()
{
}
void ExtractedFeatures::buffer_emplace_back(double value)
{
    this->buffer.emplace_back(value);
}
size_t ExtractedFeatures::buffer_size()
{
    return this->buffer.size();
}
double ExtractedFeatures::buffer_value(int index)
{
    return this->buffer[index];
}

void ExtractedFeatures::clear()
{
    buffer.clear();
    energy.clear();
    f0.clear();
    centroid.clear();
    zcr.clear();
}

void ExtractedFeatures::energy_emplace_back(double energy)
{
    this->energy.emplace_back(energy);
}

size_t ExtractedFeatures::energy_size()
{
    return this->energy.size();
}
double ExtractedFeatures::energy_value(int index)
{
    return this->energy[index];
}
void ExtractedFeatures::f0_emplace_back(std::vector<double> f0)
{
    for(std::vector<double>::iterator it=f0.begin();it!=f0.end();++it)
    {
            this->f0.emplace_back(*it);
    }
}
size_t ExtractedFeatures::f0_size()
{
    return this->f0.size();
}
double ExtractedFeatures::f0_value(int index)
{
    return this->f0[index];
}

void ExtractedFeatures::centroid_emplace_back(double centroid)
{
    this->centroid.emplace_back(centroid);
}
size_t ExtractedFeatures::centroid_size()
{
    return this->centroid.size();
}
double ExtractedFeatures::centroid_value(int index)
{
    return this->centroid[index];
}
void ExtractedFeatures::zcr_emplace_back(double zcr)
{
    this->zcr.emplace_back(zcr);
}
size_t ExtractedFeatures::zcr_size()
{
    return this->zcr.size();
}
double ExtractedFeatures::zcr_value(int index)
{
    return this->zcr[index];
}
