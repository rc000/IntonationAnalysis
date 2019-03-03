#include "contoursdetector.h"

#include <QDebug>
#define TRANSITION 15

ContoursDetector::ContoursDetector(ExtractionHelper extractionHelper)
{
    this->extractionHelper = extractionHelper;
    this->seriesContours =  new QScatterSeries();
    averageValue = 0.0;
    numberOfPositiveValues = 0;

}
ContoursDetector::~ContoursDetector()
{
}

int ContoursDetector::findIndexOfLastF0Value()
{
    for(size_t i=extractionHelper.f0_size()-1;i>1;i--)
    {
        if (extractionHelper.f0_value(i) > 50)
        {
            return i;
        }
    }
}

void ContoursDetector::findContours()
{
    currentContour.setStart(1);
    lastValueIndex = findIndexOfLastF0Value();

    for(size_t i=1;i<extractionHelper.f0_size();i++)
    {
        double value =extractionHelper.f0_value(i);

        double previousValue = extractionHelper.f0_value(i-1);
        seriesContours->append(i,value);
        if (value > maxValue) maxValue = value;
        if (value < minValue && value > F0_MIN) minValue = value;
        if(std::abs(value - previousValue) > TRANSITION)
        {
            currentContour.setEnd(i-1);
            currentContour.setCenter();
            foundNewContour();
            currentContour.setStart(i);
            currentContour.addValue(value);
        }
        else
        {
            currentContour.addValue(value);
        }
    }
    calcRegressionLines();

    classificator = new Classificator(lastIndexOfFirstPart,lastIndexOfCenterPart);

    for(int i = 0;i<ContoursVector.size();)
    {
        double averageWithoutCurrentContour = sumAllValues - ContoursVector.at(i).getCenterOfRegressionLine();
        averageWithoutCurrentContour /= (ContoursVector.size()-1);
        if((ContoursVector.at(i).getCenterValue() > (averageWithoutCurrentContour*1.6))
                && (ContoursVector.at(i).getContourLength()<10))
        {
            /*int nextValidate = getNextValidateContour(i);
            if(nextValidate!=-1)
                ContoursVector.at(nextValidate).setStartState(0);*/
            ContoursVector.erase(ContoursVector.begin()+i);
        }
        else
        {
            if (ContoursVector.at(i).getCenter() < lastIndexOfFirstPart)
            {
                qDebug()<<"            if (currentContour.getCenter() < lastIndexOfFirstPart)";
                ContoursVector.at(i).setLocation(BEGINNING);
            }
            else if (ContoursVector.at(i).getCenter() < lastIndexOfCenterPart)
                ContoursVector.at(i).setLocation(CENTER);
            else
                ContoursVector.at(i).setLocation(END);
            classificator->addContour(ContoursVector.at(i));
            i++;
        }

    }

    result = classificator->classification();
    qDebug()<<"result "<<result;
    analysisResults = classificator->getAnalysisResult();
    stateChanges = classificator->getStateChanges();
    delete classificator;


}
void ContoursDetector::foundNewContour()
{
    if (!currentContour.isContourValidate())
    {
        currentContour.clear();
        return;
    }
    sumAllValues += currentContour.getCenterValue();

    if (lastIndexOfFirstPart == 0)
    {
        firstValueIndex = currentContour.getStartIndex();
        double range = lastValueIndex-firstValueIndex;
        lastIndexOfFirstPart = currentContour.getStartIndex()+range/4;
        lastIndexOfCenterPart = lastValueIndex - range/4;
    }





    if (ContoursVector.size()>0)
    {
        if ((currentContour.getFirstValue()-ContoursVector.back().getLastValue())
                >(currentContour.getFirstValue()/6))
        {
            currentContour.setStartState(GROWTH);
        }
        else if ((ContoursVector.back().getLastValue() - currentContour.getFirstValue())
                 >(ContoursVector.back().getLastValue()/4))
        {
            currentContour.setStartState(DROP);
        }
    }

    ValidateContoursVector.push_back(currentContour);

    ContoursVector.push_back(currentContour);


    currentContour.clear();
}
void ContoursDetector::lookForLastContour()
{
    Contour lastContour;
    lastContour.setStart(ContoursVector.back().getEndIndex()+1);
    lastContour.setEnd(extractionHelper.f0_size());
    for (int j = lastContour.getStartIndex(); j<lastContour.getEndIndex();j++)
    {
        lastContour.addValue(extractionHelper.f0_value(j));
    }

    if (lastContour.isContourValidate())
    {
        lastContour.setCenter();
        ContoursVector.push_back(lastContour);
        for (int j = lastContour.getStartIndex(); j<lastContour.getEndIndex();j++)
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
void ContoursDetector::calcRegressionLines()
{
    seriesRegresionLines.clear();
    double prevA = 0.0;
    double prevB = 0.0;
    for (int i=0;i<ContoursVector.size();i++)
    {
        if (!(ContoursVector.at(i).isContourValidate()) )
            continue;
        /*if (ContoursVector.at(i).getCenter() < lastIndexOfFirstPart)
            ContoursVector.at(i).setLocation(BEGINNING);
        else if (ContoursVector.at(i).getCenter() < lastIndexOfCenterPart)
            ContoursVector.at(i).setLocation(CENTER);
        else
            ContoursVector.at(i).setLocation(END);*/
        double A = 0.0;
        double B = 0.0;
        double sigX = 0.0;
        double sigY = 0.0;
        double sigXY = 0.0;
        double sigSqrX = 0.0;
        double sigSqrY = 0.0;

        int j;
        for (j=0;j<ContoursVector.at(i).getSize();j++)
        {
            sigX+=j;
            sigY+=ContoursVector.at(i).getValue(j);
            sigXY+=j*ContoursVector.at(i).getValue(j);

            sigSqrX+=j*j;
            sigSqrY+=ContoursVector.at(i).getValue(j)*ContoursVector.at(i).getValue(j);
        }
        A = (j * sigXY - sigX * sigY) / (j * sigSqrX - sigX * sigX);
        B =  (sigY - A * sigX) / j;
        double diffB = B - prevB;
        QString state;

        (diffB>0)? state = "wzrost, " : state = "spadek, ";
        (A>0)? state+="kontur rosnacy" : state+="kontur opadajacy";


        if(j>1)
        {
            double centerRegresionLine;
            if (A > 0.1)
            {
                centerRegresionLine = A * (ContoursVector.at(i).getEndIndex()-ContoursVector.at(i).getStartIndex()) + B;
            }
            else

                centerRegresionLine = A * (ContoursVector.at(i).getCenter()-ContoursVector.at(i).getStartIndex()) + B;
            ContoursVector.at(i).setCoefA(A);
            ContoursVector.at(i).setCoefB(B);
            ContoursVector.at(i).setCenterRegressionLine(centerRegresionLine);

        }
        prevA=A;
        prevB=B;

        QLineSeries *lineSeries = new QLineSeries();
        lineSeries->append(ContoursVector.at(i).getStartIndex(),A * (ContoursVector.at(i).getStartIndex()-ContoursVector.at(i).getStartIndex()) + B);
        lineSeries->append(ContoursVector.at(i).getEndIndex(),A * (ContoursVector.at(i).getEndIndex()-ContoursVector.at(i).getStartIndex()) + B);
        ContoursVector.at(i).setRegresionLine(lineSeries);
    }
}
std::vector<QLineSeries*> ContoursDetector::getSeriesRegresionLines()
{
    std::vector<QLineSeries*>vector;
    for(int i =0;i<ContoursVector.size();i++)
    {
        if(ContoursVector.at(i).getRegressionLine()==nullptr)
            continue;
        else {
            vector.emplace_back(ContoursVector.at(i).getRegressionLine());
        }
    }
    return vector;
}

int ContoursDetector::getNextValidateContour(int index)
{
    for(int i = index+1;i<ContoursVector.size();i++)
    {
        if(!ContoursVector.at(i).isContourValidate())
            continue;
        return i;
    }
    return -1;
}