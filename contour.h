#ifndef CONTOUR_H
#define CONTOUR_H

#define BEGINNING 1
#define CENTER 2
#define END 3
#define ENTIRE 4

#define FALL -1
#define RISE 1
#define FALLING 0
#define RISING 1
#define FLAT 2

#define WOMAN 0
#define MAN 1

class Contour
{
public:
    Contour(int i,double wspA,double wspB,int cL,bool startState,bool contourState,int locationOnTheChart,
            double centerRegresionLine,int start,int end);

    bool getContourState() {return contourState;}
    int getContourLength() {return contourLength;}
    int getLocationOnTheChart(){return locationOnTheChart;}


    double getWspB() {return wspB;}
    double getWspA() {return wspA;}
     bool getStartState() {return startState;}
     double getCenterOfRegresionLine() {return centerRegresionLine;}
     int getStartIndex() {return start;}
     int getEndIndex() {return end;}


private:
    int index;
    double wspA,wspB;
    int contourLength;
    bool startState;
    bool contourState;
    int locationOnTheChart;
    double centerRegresionLine;
    int start,end;


};

#endif // CONTOUR_H
