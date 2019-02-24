#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H
#include<vector>
#include"singlesegment.h"
#include <string>
#include<QString>
using namespace std;

class Classificator
{
public:
    Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart);
    void addContour(SingleSegment contour) {contours.emplace_back(contour);}
    SingleSegment getContour(int index) {return contours.at(index);}

    std::vector<QString> classification();
    std::vector<QString>getAnalysisResult();
    std::vector<QString>getStateChanges(){return stateChanges;}

private:
    std::vector<SingleSegment>contours;
    std::vector<QString>stateChanges;
    int lastIndexOfBeginningPart;
    int lastIndexOfCenterPart;
    double highestValueOfRegresionLinesAtTheBeginning;
    int features;

    double highestValueOfRegresionLinesAtTheCenter;
    double highestValueOfRegresionLinesAtTheEnd;
    int indexHighestValueOfRegresionLinesAtTheBeginning;
    int indexHighestValueOfRegresionLinesAtTheCenter;
    int indexHighestValueOfRegresionLinesAtTheEnd;
    bool analysis();


    bool areAllContoursFalling();
    int getLastIndexOfBeginningPart() {return lastIndexOfBeginningPart;}
    int getLastIndexOfCenterPart() {return lastIndexOfCenterPart;}

    bool compareTwoParts(int part1,int part2);

};

#endif // CLASSIFICATION_H
