#ifndef EXTRACTEDFEATURES_H
#define EXTRACTEDFEATURES_H

#include<vector>

class ExtractedFeatures{
private:

    std::vector<double>buffer;
     std::vector<double>energy;
    std::vector<double>centroid;
    std::vector<double>zcr;
    std::vector<double>f0;
    std::vector<double>fft;

public:
    ExtractedFeatures();
    void clear();
    void buffer_emplace_back(double);
    size_t buffer_size();
    double buffer_value(int index);



    void energy_emplace_back(double);
    size_t energy_size();
    double energy_value(int index);

    void f0_emplace_back(std::vector<double>);
    size_t f0_size();
    double f0_value(int index);

    void centroid_emplace_back(double);
    size_t centroid_size();
    double centroid_value(int index);

    void zcr_emplace_back(double);
    size_t zcr_size();
    double zcr_value(int index);


    void  setFFT(std::vector<double>fft) {  this->fft = fft;}

    std::vector<double>getFFT() { return fft;}

};
#endif // SINGLESAMPLE_H
