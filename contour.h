﻿#ifndef CONTOUR_H
#define CONTOUR_H
#include <vector>
#include <QtDebug>
#include <QtCharts/QLineSeries>
//#include <contour.h>

#define BEGINNING 1
#define CENTER 2
#define END 3
#define ENTIRE 4

#define DROP -1
#define GROWTH 1
#define FALLING 0
#define RISING 1
#define FLAT 2

#define WOMAN 0
#define MAN 1
QT_CHARTS_USE_NAMESPACE

class Contour {
public:
    Contour(int state=0){startState = state; seriesRegresionLine = nullptr; centerRegressionLine = 0.0;}
    size_t getStartIndex(){return startIndex;}
    size_t getEndIndex(){return endIndex;}
    size_t getCenter(){return centerIndex;}

    size_t getSize(){return values.size();}
    void setStart(size_t startIndex){this->startIndex = startIndex;}
    void setEnd(size_t endIndex){this->endIndex = endIndex;}
    void setCenter(){this->centerIndex = startIndex + (endIndex-startIndex)/2;}
    void setLocation(int location){locationOnTheChart = location;}
    void setRegresionLine(QLineSeries *series){this->seriesRegresionLine = series;}

    QLineSeries* getRegressionLine(){return this->seriesRegresionLine;}
    int getLocationOnTheChart(){ return locationOnTheChart;}
    double getValue(size_t i){return values.at(i);}
    double getLastValue(){return values.back();}
    double getFirstValue(){return values.front();}
    double getCenterValue(){return values.at(values.size()/2);}

    void addValue(double value){values.emplace_back(value);}
    void clear(){values.clear();startState = 0;}
    size_t getContourLength(){return endIndex-startIndex;}

    bool isContourValidate()
    {
        if (values.size()<2)
            return false;
        if (values.at(0) == 0)
             return false;
        return true;
    }
    double getCoefA(){ return coefA;}
    double getCoefB(){ return coefB;}
    int getStartState(){return startState;}
    double getCenterOfRegressionLine(){return centerRegressionLine;}
    std::vector<double>getValuesVector(){return values;}

    void setCoefA(double coefA){ this->coefA = coefA;}
    void setCoefB(double coefB){ this->coefB = coefB;}
    void setContourLength(int length){this->contourLength = length;}
    void setStartState(int state){startState = state;}
    void setCenterRegressionLine(double center){this->centerRegressionLine = center;}
    void setValue(int index, double value){this->values.at(index) = value;}

    void setImp(bool imp){this->imperative = imp;}
    bool getImp(){return imperative;}

private:
    size_t startIndex;
    size_t centerIndex;
    size_t endIndex;
    std::vector<double> values;
    int locationOnTheChart;

    double coefA,coefB;
    int contourLength;
    int startState;
    bool imperative = false;
    double centerRegressionLine;
    QLineSeries *seriesRegresionLine;

};

#endif // Contour_H
