#ifndef CONTOURSEXTRACTOR_H
#define CONTOURSEXTRACTOR_H
#include <vector>
#include <QScatterSeries>
#include <QtCharts/QLineSeries>
#include "singleframefeatures.h"
#include "singlesegment.h"
#include "classificator.h"
QT_CHARTS_USE_NAMESPACE

class ContoursExtractor
{
 public:
    ContoursExtractor(SingleFrameFeatures framesFeatures);
    ContoursExtractor(SingleFrameFeatures framesFeatures,
                      QScatterSeries *seriesContours);

    void findContours();
    void foundNewContour(int i,double &averageValue,int &numberOfPositiveValues);
    void lookForLastContour(double &averageValue, int &numberOfPositiveValues);
    void calcRegressionLines();
    int findIndexOfLastF0Value();
    double getMaxValue() { return maxValue;}
    double getMinValue() { return minValue;}
    int getIndexOfFirstValue() { return firstValueIndex;}
    int getIndexOfLastValue() { return lastValueIndex;}
    int getLastIndexOfBeginningPart() { return firstPart;}
    int getLastIndexOfCenterPart() { return centerPart;}
    std::vector<QLineSeries*> getSeriesRegresionLines();
    QScatterSeries *seriesContours;
    QScatterSeries * getSeriesContours(){return seriesContours;}
    QString getResult(){return result;}
    std::vector<QString> getAnalysisResults(){return analysisResults;}
    std::vector<QString> getStateChanges(){return stateChanges;}
    SingleFrameFeatures getFrameFeatures(){ return framesFeatures;}
    int getNextValidateContour(int index);


private:
    double maxValue = 0.0;
    double minValue = 400.0;
    int firstValueIndex,lastValueIndex;
    int firstPart = 0;
    int centerPart = 0;
    SingleFrameFeatures framesFeatures;
    std::vector<QLineSeries*>seriesRegresionLines;
    Classificator *classificator;
    std::vector<SingleSegment> SegmentsVector;
    std::vector<SingleSegment> ValidateSegmentsVector;

    SingleSegment currentSegment;
    std::vector<QString> analysisResults;
    std::vector<QString> stateChanges;
    double sumAllValues = 0.0;
    int numberAllValues = 0;


    QString result;

};

#endif // CONTOURSEXTRACTOR_H
