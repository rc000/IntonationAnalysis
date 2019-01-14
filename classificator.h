#ifndef CLASSIFICATOR_H
#define CLASSIFICATOR_H
#include<vector>
#include<contour.h>

using namespace std;

class Classificator
{
public:
    Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart,bool sex);
    void addContour(Contour contour) {contours.emplace_back(contour);}
    Contour getContour(int index) {return contours.at(index);}
    std::string classification();

private:
    std::vector<Contour>contours;
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
