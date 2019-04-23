#ifndef CLASSIFICATORNEW_H
#define CLASSIFICATORNEW_H
#include "segment.h"

class ClassificatorNew
{
public:
    ClassificatorNew(){}
    void addSegment(Segment segment) {segments.emplace_back(segment);}
    QString classification();
    Segment getSegment(int index) {return segments.at(index);}
private:
    std::vector<Segment>segments;

};

#endif // CLASSIFICATORNEW_H
