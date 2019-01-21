#ifndef SINGLESEGMENT_H
#define SINGLESEGMENT_H
#include <vector>

#define BEGINNING 1
#define CENTER 2
#define END 3
#define ENTIRE 4

#define FALL 0
#define RISE 1
#define FALLING 0
#define RISING 1
#define FLAT 2

#define WOMAN 0
#define MAN 1
class SingleSegment {
public:
    SingleSegment(){}
    size_t getStartIndex(){return start;}
    size_t getEndIndex(){return end;}
    size_t getCenter(){return center;}

    size_t getSize(){return values.size();}
    void setStart(size_t start){this->start = start;}
    void setEnd(size_t end){this->end = end;}
    void setCenter(){this->center = start + (end-start)/2;}
    void setLocation(int location){locationOnTheChart = location;}

    int getLocationOnTheChart(){ return locationOnTheChart;}
    double getValue(int i){return values.at(i);}
    void addValue(double value){values.emplace_back(value);}
    void clear(){values.clear();}
    void setIsSpaceBeforeContour(bool isSpace){this->spaceBeforeContour = isSpace;}
    bool IsSpaceBeforeContour(){return spaceBeforeContour;}
    void setPreviousContourOrSpaceLength(size_t length){this->previousContourOrSpaceLength = length;}
    size_t getPreviousContourOrSpaceLength(){return previousContourOrSpaceLength;}
    size_t getSegmentLength(){return end-start;}

    bool isContourValidate()
    {
        if (values.size()<3)
            return false;
        for (int i =0;i<values.size();i++)
        {
            if (values.at(i)==0)
                return false;
        }
        return true;
    }
    double getWspA(){ return wspA;}
    double getWspB(){ return wspB;}
    int getContourLength(){return values.size();}
    bool getStartState(){return startState;}
    bool getContourState(){return contourState;}
    double getCenterOfRegressionLine(){return centerRegressionLine;}

    void setWspA(double wspA){ this->wspA = wspA;}
    void setWspB(double wspB){ this->wspB = wspB;}
    void setContourLength(int length){this->contourLength = length;}
    void setStartState(bool state){this->startState = state;}
    void setContourState(bool state){this->contourState = contourState;}
    void setCenterRegressionLine(double center){this->centerRegressionLine = center;}


private:
    size_t start;
    size_t center;
    size_t end;
    bool spaceBeforeContour;
    size_t previousContourOrSpaceLength;
    std::vector<double> values;
    int locationOnTheChart;

    int index;
    double wspA,wspB;
    int contourLength;
    bool startState;
    bool contourState;
    double centerRegressionLine;
};

#endif // SINGLESEGMENT_H
