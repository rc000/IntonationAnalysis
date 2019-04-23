#ifndef CONTOURSDETECTOR_H
#define CONTOURSDETECTOR_H
#include <vector>
#include <QScatterSeries>
#include <QtCharts/QLineSeries>
#include "extractionHelper.h"
#include "segment.h"
#include "classificator.h"
#include "classificatornew.h"
QT_CHARTS_USE_NAMESPACE

class SegmentsDetector
{
 public:
    SegmentsDetector(ExtractionHelper extractionHelper);

    ~SegmentsDetector();

    void findSegments();
    void foundNewSegment();
    void calcRegressionLines();
    void classification();

    int findIndexOfLastF0Value();
    double getMaxValue() { return maxValue;}
    double getMinValue() { return minValue;}

    std::vector<QLineSeries*> getSeriesRegresionLines();
    QScatterSeries *seriesSegments;
    QScatterSeries * getSeriesSegments(){return seriesSegments;}
    ExtractionHelper getFeatures(){ return extractionHelper;}
    int getNextValidateSegment(int index);

    int getIndexOfFirstValue() { return firstValueIndex;}
    int getIndexOfLastValue() { return lastValueIndex;}
    int getLastIndexOfFirstPart() { return lastIndexOfFirstPart;}
    int getLastIndexOfCenterPart() { return lastIndexOfCenterPart;}
    void setSegmentLocation(int i);
    std::vector<QString> getResult(){return result;}
    std::vector<QString> getAnalysisResults(){return analysisResults;}
    std::vector<QString> getStateChanges(){return stateChanges;}
    void findLongest();


private:
    double maxValue = 0.0;
    double minValue = 400.0;
    double longestSegmentLength;
    double averageValue;
    int numberOfPositiveValues;
    int firstValueIndex,lastValueIndex;
    int lastIndexOfFirstPart = 0;
    int lastIndexOfCenterPart = 0;
     std::vector<QLineSeries*>seriesRegresionLines;
    Classificator *classificator;
    ClassificatorNew *classificatorNew;

    ExtractionHelper extractionHelper;
    std::vector<Segment> SegmentsVector;

    Segment currentSegment;
    std::vector<QString> analysisResults;
    std::vector<QString> stateChanges;
    double sumAllValues = 0.0;
    int numberAllValues = 0;


    std::vector<QString> result;

};

#endif // CONTOURSEXTRACTOR_H
