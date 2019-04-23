#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H
#include<vector>
#include"segment.h"
#include <string>
#include<QString>
using namespace std;

class Classificator
{
public:
    Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart, double length);
    void addSegment(Segment segment) {segments.emplace_back(segment);}
    Segment getSegment(int index) {return segments.at(index);}

    std::vector<QString> classification();
    std::vector<QString>getAnalysisResult();
    std::vector<QString>getStateChanges(){return stateChanges;}

private:
    std::vector<Segment>segments;
    std::vector<QString>stateChanges;
    int lastIndexOfBeginningPart;
    int lastIndexOfCenterPart;
    double longestSegmentLength;
    double highestValueOfRegresionLinesAtTheBeginning;
    int features;
    Segment startHighestSegment, centerHighestSegment, endHighestSegment;

    double highestValueOfRegresionLinesAtTheCenter;
    double highestValueOfRegresionLinesAtTheEnd;
    int indexHighestValueOfRegresionLinesAtTheBeginning;
    int indexHighestValueOfRegresionLinesAtTheCenter;
    int indexHighestValueOfRegresionLinesAtTheEnd;
    void analysis();


    bool areAllSegmentsFalling();
    int getLastIndexOfBeginningPart() {return lastIndexOfBeginningPart;}
    int getLastIndexOfCenterPart() {return lastIndexOfCenterPart;}


};

#endif // CLASSIFICATION_H
