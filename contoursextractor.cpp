#include "contoursextractor.h"

#include <QDebug>

ContoursExtractor::ContoursExtractor(ExtractionHelper extractionHelper)
{
    this->extractionHelper = extractionHelper;
    this->seriesContours =  new QScatterSeries();

}
ContoursExtractor::~ContoursExtractor()
{
}

int ContoursExtractor::findIndexOfLastF0Value()
{
    for(size_t i=extractionHelper.f0_size()-1;i>1;i--)
    {
         if (extractionHelper.f0_value(i) > 50)
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
    for(size_t i=1;i<extractionHelper.f0_size();i++)
    {
        double value =extractionHelper.f0_value(i);
         seriesContours->append(i,value);
        if (value > maxValue)
            maxValue = value;
        if (value < minValue && value > 60)
            minValue = value;
        if(std::abs(value - extractionHelper.f0_value(i-1)) > 15)
        {
            foundNewContour(i,averageValue,numberOfPositiveValues);
        }
     }
     lookForLastContour(averageValue,numberOfPositiveValues);

    int index =0;
    for(int i = 0;i<SegmentsVector.size();i++)
    {
        for(int j =0; j<SegmentsVector.at(i).getContourLength();j++)
        {
           //seriesContours->append(index,SegmentsVector.at(i).getValue(j));
            index++;

        }
    }

    averageValue/=numberOfPositiveValues;

     classificator = new Classificator(firstPart,centerPart);

     calcRegressionLines();

     for(int i = 0;i<SegmentsVector.size();i++)
     {
        if(!SegmentsVector.at(i).isContourValidate())
            continue;
         sumAllValues += SegmentsVector.at(i).getCenterOfRegressionLine();
         numberAllValues++;

     }
     qDebug()<<"allValues "<<sumAllValues;
     qDebug()<<"number of values "<<numberAllValues;
     qDebug()<<"average "<<sumAllValues/numberAllValues;
     for(int i = 0;i<SegmentsVector.size();i++)
     {
         double averageWithoutCurrentContour = sumAllValues - SegmentsVector.at(i).getCenterOfRegressionLine();
         averageWithoutCurrentContour /= (numberAllValues-1);
         if((SegmentsVector.at(i).getCenterOfRegressionLine() > (averageWithoutCurrentContour*1.6))
                 && (SegmentsVector.at(i).getContourLength()<10))
         {
             for(auto & value:SegmentsVector.at(i).getValuesVector())
                 value = 0.0;

            // std::fill(SegmentsVector.at(i).getValuesVector().begin(),SegmentsVector.at(i).getValuesVector().end(),0.0);


             SegmentsVector.at(i).setRegresionLine(nullptr);

             int nextValidate = getNextValidateContour(i);
             if(nextValidate!=-1)
                SegmentsVector.at(nextValidate).setStartState(0);


             //std::fill(SegmentsVector.at(i).getValuesVector().begin(),SegmentsVector.at(i).getValuesVector().end(),0);
         }
         else if((SegmentsVector.at(i).getStartState()== RISE )
                 && (SegmentsVector.at(i).getWspA()<-1.0))
                SegmentsVector.at(i).setStartState(0);

         if(SegmentsVector.at(i).getRegressionLine()!=nullptr)
             classificator->addContour(SegmentsVector.at(i));

     }


    result = classificator->classification();
    qDebug()<<"result "<<result;
    analysisResults = classificator->getAnalysisResult();
    stateChanges = classificator->getStateChanges();
    delete classificator;
    //extractionHelper.clear();


}
void ContoursExtractor::foundNewContour(int i,double &averageValue,int &numberOfPositiveValues)
{

    currentSegment.setEnd(i-1);
    currentSegment.setCenter();

    for (int j = currentSegment.getStartIndex(); j<currentSegment.getEndIndex();j++)
    {
        currentSegment.addValue(extractionHelper.f0_value(j));
    }
    if (currentSegment.isContourValidate())
    {

         for (int j = currentSegment.getStartIndex(); j<currentSegment.getEndIndex();j++)
         {
            averageValue += extractionHelper.f0_value(j);
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

    if (ValidateSegmentsVector.size()>0 && currentSegment.isContourValidate())
    {


   /* if ((currentSegment.getValue(0)-ValidateSegmentsVector.back().getValue(ValidateSegmentsVector.back().getSize()-1))
            >(ValidateSegmentsVector.back().getStartIndex()/5))*/
      if ((currentSegment.getValue(0)-ValidateSegmentsVector.back().getValue(ValidateSegmentsVector.back().getSize()-1))
             >(currentSegment.getValue(0)/5))

    {
        qDebug()<<"WZROST "<<currentSegment.getStartIndex()<<" "<<currentSegment.getWspA();

        currentSegment.setStartState(RISE);
    }
    //if ((currentSegment.getValue(0)-ValidateSegmentsVector.back().getValue(ValidateSegmentsVector.back().getSize()-1))<-50)
     else if ((ValidateSegmentsVector.back().getValue(ValidateSegmentsVector.back().getSize()-1) - currentSegment.getValue(0))
             >(ValidateSegmentsVector.back().getValue(ValidateSegmentsVector.back().getSize()-1)/4))
    {
        qDebug()<<"spadek "<<currentSegment.getStartIndex();

        currentSegment.setStartState(FALL);

    }
     }
    if (currentSegment.isContourValidate())
        ValidateSegmentsVector.push_back(currentSegment);
    SegmentsVector.push_back(currentSegment);

    currentSegment.setStart(i);
     currentSegment.setPreviousContourOrSpaceLength(SegmentsVector.back().getSize());

    currentSegment.clear();
}
void ContoursExtractor::lookForLastContour(double &averageValue, int &numberOfPositiveValues)
{
    SingleSegment lastSegment;
    lastSegment.setStart(SegmentsVector.back().getEndIndex()+1);
    lastSegment.setEnd(extractionHelper.f0_size());
    for (int j = lastSegment.getStartIndex(); j<lastSegment.getEndIndex();j++)
    {
         lastSegment.addValue(extractionHelper.f0_value(j));
    }

   if (lastSegment.isContourValidate())
   {
       lastSegment.setCenter();
       SegmentsVector.push_back(lastSegment);
       for (int j = lastSegment.getStartIndex(); j<lastSegment.getEndIndex();j++)
       {
           averageValue += extractionHelper.f0_value(j);
           numberOfPositiveValues++;
       }
   }
}
float sqrt3(const float x)
{
  union
  {
    int i;
    float x;
  } u;

  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1<<22);
  return u.x;
}
void ContoursExtractor::calcRegressionLines()
{
    seriesRegresionLines.clear();
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


        double r = sigXY / sqrt3(sigSqrX * sigSqrY);
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
            SegmentsVector.at(i).setContourState((A>0.2)?RISING:FALLING);
            SegmentsVector.at(i).setCenterRegressionLine(centerRegresionLine);

        }
          prevA=A;
          prevB=B;

          QLineSeries *lineSeries = new QLineSeries();
            lineSeries->append(SegmentsVector.at(i).getStartIndex(),A * (SegmentsVector.at(i).getStartIndex()-SegmentsVector.at(i).getStartIndex()) + B);
            lineSeries->append(SegmentsVector.at(i).getEndIndex(),A * (SegmentsVector.at(i).getEndIndex()-SegmentsVector.at(i).getStartIndex()) + B);
           SegmentsVector.at(i).setRegresionLine(lineSeries);
    }
}
std::vector<QLineSeries*> ContoursExtractor::getSeriesRegresionLines()
{
    std::vector<QLineSeries*>vector;
    for(int i =0;i<SegmentsVector.size();i++)
    {
        if(SegmentsVector.at(i).getRegressionLine()==nullptr)
            continue;
        else {
            vector.emplace_back(SegmentsVector.at(i).getRegressionLine());
        }
    }
    return vector;
}
void ContoursExtractor::extractFeatures()
{

}
int ContoursExtractor::getNextValidateContour(int index)
{
    for(int i = index+1;i<SegmentsVector.size();i++)
    {
        if(!SegmentsVector.at(i).isContourValidate())
            continue;
        return i;
    }
    return -1;
}
