#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H
#include<vector>
#include"singlesegment.h"

using namespace std;

class Classificator
{
public:
    Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart,bool sex);
    void addContour(SingleSegment contour) {contours.emplace_back(contour);}
    SingleSegment getContour(int index) {return contours.at(index);}
    std::string classification();

private:
    std::vector<SingleSegment>contours;
    int lastIndexOfBeginningPart;
    int lastIndexOfCenterPart;
    bool sex;
    double highestValueOfRegresionLinesAtTheBeginning;
    int features;

    double highestValueOfRegresionLinesAtTheCenter;
    double highestValueOfRegresionLinesAtTheEnd;
    int indexHighestValueOfRegresionLinesAtTheBeginning;
    int indexHighestValueOfRegresionLinesAtTheCenter;
    int indexHighestValueOfRegresionLinesAtTheEnd;
    bool isQuestion();
    bool isAffirmative();


    bool areAllContoursFalling();
     std::string notAllContoursAreFalling();
    int getLastIndexOfBeginningPart() {return lastIndexOfBeginningPart;}
    int getLastIndexOfCenterPart() {return lastIndexOfCenterPart;}
    bool getSex(){return sex;}

    bool compareTwoParts(int part1,int part2);

};

#endif // CLASSIFICATION_H
