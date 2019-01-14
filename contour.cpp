#include "contour.h"

Contour::Contour(int index, double wspA,double wspB,int cL,bool startState,bool contourState,
                 int location,double centerRegresionLine,int start, int end)
    :index(index),wspA(wspA),wspB(wspB),contourLength(cL),startState(startState),contourState(contourState),locationOnTheChart(location),
      centerRegresionLine(centerRegresionLine),start(start),end(end)
{}
