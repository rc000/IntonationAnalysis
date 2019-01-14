#ifndef SINGLESEGMENT_H
#define SINGLESEGMENT_H
#include <vector>

class SingleSegment {
public:
    SingleSegment(){}
    size_t getStart(){return start;}
    size_t getEnd(){return end;}
    size_t getCenter(){return center;}

    size_t getSize(){return values.size();}
    void setStart(size_t start){this->start = start;}
    void setEnd(size_t end){this->end = end;}
    void setCenter(){this->center = start + (end-start)/2;}
    void setLocation(int location){locationOnTheChart = location;}

    int getLocation(){ return locationOnTheChart;}
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
        for (int i =0;i<values.size();i++)
        {
            if (values.at(i)==0)
                return false;
        }
        return true;
    }

private:
    size_t start;
    size_t center;
    size_t end;
    bool spaceBeforeContour;
    size_t previousContourOrSpaceLength;
    std::vector<double> values;
    int locationOnTheChart;
};

#endif // SINGLESEGMENT_H
