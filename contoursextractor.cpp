#include "contoursextractor.h"
#include <QDebug>

ContoursExtractor::ContoursExtractor(std::vector<SingleFrameFeatures>framesFeatures)
{
    this->framesFeatures = framesFeatures;
    this->seriesContours =  new QScatterSeries();

}

ContoursExtractor::ContoursExtractor(std::vector<SingleFrameFeatures>framesFeatures,
                  QScatterSeries *seriesContours)
{
    this->framesFeatures = framesFeatures;
    this->seriesContours =  seriesContours;
}
int ContoursExtractor::findIndexOfLastF0Value()
{
    for(size_t i=framesFeatures[framesFeatures.size()-1].f0_size()-1;i>1;i--)
    {
         if (framesFeatures[framesFeatures.size()-1].f0_value(i) > 50)
         {
             return i;
         }
     }
}
void ContoursExtractor::findContours()
{
    SegmentsVector.clear();
    currentSegment.clear();


    double averageValue = 0.0;
    int numberOfPositiveValues = 0;

    currentSegment.setStart(1);
    lastValueIndex = findIndexOfLastF0Value();
    for(size_t i=1;i<framesFeatures[framesFeatures.size()-1].f0_size();i++)
    {
        double value =framesFeatures[framesFeatures.size()-1].f0_value(i);
         seriesContours->append(i,value);
        if (value > maxValue)
            maxValue = value;
        if (value < minValue && value > 40)
            minValue = value;
        if(std::abs(value - framesFeatures[framesFeatures.size()-1].f0_value(i-1)) > 15)
        {
            qDebug()<<"hello mam kontur";
            foundNewContour(i,averageValue,numberOfPositiveValues);
        }

    }
    lookForLastContour(averageValue,numberOfPositiveValues);

    averageValue/=numberOfPositiveValues;

     classificator = new Classificator(firstPart,centerPart);
    calcRegressionLines();

    result = QString::fromStdString(classificator->classification());
    qDebug()<<"result "<<result;


}
void ContoursExtractor::foundNewContour(int i,double &averageValue,int &numberOfPositiveValues)
{
    currentSegment.setEnd(i-1);
    currentSegment.setCenter();

    for (int j = currentSegment.getStartIndex(); j<currentSegment.getEndIndex();j++)
    {
        currentSegment.addValue(framesFeatures[framesFeatures.size()-1].f0_value(j));
    }
    if (currentSegment.isContourValidate())
    {

         for (int j = currentSegment.getStartIndex(); j<currentSegment.getEndIndex();j++)
         {
            averageValue += framesFeatures[framesFeatures.size()-1].f0_value(j);
            numberOfPositiveValues++;
          }
    }

    if (firstPart == 0 && currentSegment.isContourValidate() && currentSegment.getSegmentLength()>2)
    {
        firstValueIndex = currentSegment.getStartIndex();
        firstPart = currentSegment.getStartIndex()+ (lastValueIndex-currentSegment.getStartIndex())/4;
        centerPart = lastValueIndex - (lastValueIndex-firstValueIndex)/4;

    }
    if (currentSegment.isContourValidate())
    {
        if (currentSegment.getCenter() < firstPart)
            currentSegment.setLocation(BEGINNING);
        else if (currentSegment.getCenter() < centerPart)
            currentSegment.setLocation(CENTER);
        else
            currentSegment.setLocation(END);
    }
    SegmentsVector.push_back(currentSegment);
    currentSegment.setStart(i);
    currentSegment.setIsSpaceBeforeContour(!SegmentsVector.back().isContourValidate());
    currentSegment.setPreviousContourOrSpaceLength(SegmentsVector.back().getSize());

    currentSegment.clear();
}
void ContoursExtractor::lookForLastContour(double &averageValue, int &numberOfPositiveValues)
{
    SingleSegment lastSegment;
    lastSegment.setStart(SegmentsVector.back().getEndIndex()+1);
    lastSegment.setEnd(framesFeatures[framesFeatures.size()-1].f0_size());
    for (int j = lastSegment.getStartIndex(); j<lastSegment.getEndIndex();j++)
    {
         lastSegment.addValue(framesFeatures[framesFeatures.size()-1].f0_value(j));
    }

   if (lastSegment.isContourValidate())
   {
       lastSegment.setCenter();
       SegmentsVector.push_back(lastSegment);
       for (int j = lastSegment.getStartIndex(); j<lastSegment.getEndIndex();j++)
       {
           averageValue += framesFeatures[framesFeatures.size()-1].f0_value(j);
           numberOfPositiveValues++;
       }
   }
}

void ContoursExtractor::calcRegressionLines()
{
    double prevA = 0.0;
    double prevB = 0.0;
    for (int i=0;i<SegmentsVector.size();i++)
    {
        if (!(SegmentsVector.at(i).isContourValidate()) )
            continue;
        if (SegmentsVector.at(i).getCenter() < firstPart)
            SegmentsVector.at(i).setLocation(BEGINNING);
        else if (SegmentsVector.at(i).getCenter() < centerPart)
            SegmentsVector.at(i).setLocation(CENTER);
        else
            SegmentsVector.at(i).setLocation(END);
        double A = 0.0;
        double B = 0.0;
        double sigX = 0.0;
        double sigY = 0.0;
        double sigXY = 0.0;
        double sigSqrX = 0.0;
        double sigSqrY = 0.0;

        int j;
        for (j=0;j<SegmentsVector.at(i).getSize();j++)
        {
            sigX+=j;
            sigY+=SegmentsVector.at(i).getValue(j);
            sigXY+=j*SegmentsVector.at(i).getValue(j);

            sigSqrX+=j*j;
            sigSqrY+=SegmentsVector.at(i).getValue(j)*SegmentsVector.at(i).getValue(j);
        }
        A = (j * sigXY - sigX * sigY) / (j * sigSqrX - sigX * sigX);
        B =  (sigY - A * sigX) / j;
        double diffB = B - prevB;
        QString state;

        (diffB>0)? state = "wzrost, " : state = "spadek, ";
        (A>0)? state+="kontur rosnacy" : state+="kontur opadajacy";

        double r = sigXY / std::sqrt(sigSqrX * sigSqrY);
           if(j>1)
        {
            double centerRegresionLine;
            if (A > 0.1)
            {
                centerRegresionLine = A * (SegmentsVector.at(i).getEndIndex()-SegmentsVector.at(i).getStartIndex()) + B;
            }
            else

                centerRegresionLine = A * (SegmentsVector.at(i).getCenter()-SegmentsVector.at(i).getStartIndex()) + B;
            SegmentsVector.at(i).setWspA(A);
            SegmentsVector.at(i).setWspB(B);
            SegmentsVector.at(i).setStartState((diffB>0)?RISE:FALL);
            SegmentsVector.at(i).setContourState((A>0.2)?RISING:FALLING);
            SegmentsVector.at(i).setCenterRegressionLine(centerRegresionLine);

            classificator->addContour(SegmentsVector.at(i));
        }
          prevA=A;
          prevB=B;

            seriesRegresionLines.push_back(new QLineSeries());
            seriesRegresionLines.back()->append(SegmentsVector.at(i).getStartIndex(),A * (SegmentsVector.at(i).getStartIndex()-SegmentsVector.at(i).getStartIndex()) + B);
            seriesRegresionLines.back()->append(SegmentsVector.at(i).getEndIndex(),A * (SegmentsVector.at(i).getEndIndex()-SegmentsVector.at(i).getStartIndex()) + B);
      }
}
