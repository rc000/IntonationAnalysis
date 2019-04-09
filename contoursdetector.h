#ifndef CONTOURSDETECTOR_H
#define CONTOURSDETECTOR_H
#include <vector>
#include <QScatterSeries>
#include <QtCharts/QLineSeries>
#include "extractionHelper.h"
#include "contour.h"
#include "classificator.h"
QT_CHARTS_USE_NAMESPACE

class ContoursDetector
{
 public:
    ContoursDetector(ExtractionHelper extractionHelper);

    ~ContoursDetector();

    void findContours();
    void foundNewContour();
    void calcRegressionLines();
    void classification();

    int findIndexOfLastF0Value();
    double getMaxValue() { return maxValue;}
    double getMinValue() { return minValue;}

    std::vector<QLineSeries*> getSeriesRegresionLines();
    QScatterSeries *seriesContours;
    QScatterSeries * getSeriesContours(){return seriesContours;}
    ExtractionHelper getFeatures(){ return extractionHelper;}
    int getNextValidateContour(int index);

    int getIndexOfFirstValue() { return firstValueIndex;}
    int getIndexOfLastValue() { return lastValueIndex;}
    int getLastIndexOfFirstPart() { return lastIndexOfFirstPart;}
    int getLastIndexOfCenterPart() { return lastIndexOfCenterPart;}
    void setContourLocation(int i);
    std::vector<QString> getResult(){return result;}
    std::vector<QString> getAnalysisResults(){return analysisResults;}
    std::vector<QString> getStateChanges(){return stateChanges;}
    void findLongest();


private:
    double maxValue = 0.0;
    double minValue = 400.0;
    double longestContourLength;
    double averageValue;
    int numberOfPositiveValues;
    int firstValueIndex,lastValueIndex;
    int lastIndexOfFirstPart = 0;
    int lastIndexOfCenterPart = 0;
     std::vector<QLineSeries*>seriesRegresionLines;
    Classificator *classificator;
    ExtractionHelper extractionHelper;
    std::vector<Contour> ContoursVector;

    Contour currentContour;
    std::vector<QString> analysisResults;
    std::vector<QString> stateChanges;
    double sumAllValues = 0.0;
    int numberAllValues = 0;


    std::vector<QString> result;

};

#endif // CONTOURSEXTRACTOR_H
