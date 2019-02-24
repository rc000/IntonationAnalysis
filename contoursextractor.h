#ifndef CONTOURSEXTRACTOR_H
#define CONTOURSEXTRACTOR_H
#include <vector>
#include <QScatterSeries>
#include <QtCharts/QLineSeries>
#include "extractionHelper.h"
#include "singlesegment.h"
#include "classificator.h"
QT_CHARTS_USE_NAMESPACE

class ContoursExtractor
{
 public:
    ContoursExtractor(ExtractionHelper extractionHelper);

    ~ContoursExtractor();

    void findContours();
    void foundNewContour(int i,double &averageValue,int &numberOfPositiveValues);
    void lookForLastContour(double &averageValue, int &numberOfPositiveValues);
    void calcRegressionLines();
    void extractFeatures();
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
    std::vector<QString> getResult(){return result;}
    std::vector<QString> getAnalysisResults(){return analysisResults;}
    std::vector<QString> getStateChanges(){return stateChanges;}
   // ExtractedFeatures getFrameFeatures(){ return extractedFeatures;}
    ExtractionHelper getFeatures(){ return extractionHelper;}
    int getNextValidateContour(int index);


private:
    double maxValue = 0.0;
    double minValue = 400.0;
    int firstValueIndex,lastValueIndex;
    int firstPart = 0;
    int centerPart = 0;
    //ExtractedFeatures extractedFeatures;
    std::vector<QLineSeries*>seriesRegresionLines;
    Classificator *classificator;
    ExtractionHelper extractionHelper;
    std::vector<SingleSegment> SegmentsVector;
    std::vector<SingleSegment> ValidateSegmentsVector;

    SingleSegment currentSegment;
    std::vector<QString> analysisResults;
    std::vector<QString> stateChanges;
    double sumAllValues = 0.0;
    int numberAllValues = 0;


    std::vector<QString> result;

};

#endif // CONTOURSEXTRACTOR_H
