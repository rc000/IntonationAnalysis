#ifndef SINGLESAMPLEFEATURES_H
#define SINGLESAMPLEFEATURES_H

#endif // SINGLESAMPLE_H
#include<vector>
class SingleSampleFeatures{
private:
    std::vector<double>buffer;
    std::vector<double>fftBuffer;
    std::vector<double>energy;
    std::vector<double>centroid;
    std::vector<double>zcr;
    std::vector<double>f0;
public:
    SingleSampleFeatures();
    void buffer_emplace_back(double);
    size_t buffer_size();
    double buffer_value(int index);

    void fftBuffer_emplace_back(std::vector<double>);
    size_t fftBuffer_size();
    double fftBuffer_value(int index);

    void energy_emplace_back(double);
    size_t energy_size();
    double energy_value(int index);

    void centroid_emplace_back(double);
    size_t centroid_size();
    double centroid_value(int index);

    void zcr_emplace_back(double);
    size_t zcr_size();
    double zcr_value(int index);

    void f0_emplace_back(std::vector<double>);
    size_t f0_size();
    double f0_value(int index);

};
