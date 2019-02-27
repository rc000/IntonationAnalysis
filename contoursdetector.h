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
    void lookForLastContour();
    void calcRegressionLines();
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
    int getLastIndexOfBeginningPart() { return lastIndexOfBeginningPart;}
    int getLastIndexOfCenterPart() { return lastIndexOfCenterPart;}
    std::vector<QString> getResult(){return result;}
    std::vector<QString> getAnalysisResults(){return analysisResults;}
    std::vector<QString> getStateChanges(){return stateChanges;}

private:
    double maxValue = 0.0;
    double minValue = 400.0;
    double averageValue;
    int numberOfPositiveValues;
    int firstValueIndex,lastValueIndex;
    int lastIndexOfBeginningPart = 0;
    int lastIndexOfCenterPart = 0;
     std::vector<QLineSeries*>seriesRegresionLines;
    Classificator *classificator;
    ExtractionHelper extractionHelper;
    std::vector<Contour> ContoursVector;
    std::vector<Contour> ValidateContoursVector;

    Contour currentContour;
    std::vector<QString> analysisResults;
    std::vector<QString> stateChanges;
    double sumAllValues = 0.0;
    int numberAllValues = 0;


    std::vector<QString> result;

};

#endif // CONTOURSEXTRACTOR_H
