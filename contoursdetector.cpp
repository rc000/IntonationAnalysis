#include "contoursdetector.h"

#include <QDebug>
#define TRANSITION 20

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
void ContoursDetector::setContourLocation(int i)
{
    if (ContoursVector.at(i).getCenter() < lastIndexOfFirstPart)
    {
        ContoursVector.at(i).setLocation(BEGINNING);
        if (ContoursVector.at(i).getStartIndex() > (lastIndexOfFirstPart-(lastIndexOfFirstPart-firstValueIndex)/2))
        {
                qDebug()<<"Start Index "<<ContoursVector.at(i).getStartIndex();
                qDebug()<<(lastIndexOfFirstPart-(lastIndexOfFirstPart-firstValueIndex)/2);
                ContoursVector.at(i).setImp(true);
        }
    }
    else if (ContoursVector.at(i).getCenter() < lastIndexOfCenterPart)
    {
        ContoursVector.at(i).setLocation(CENTER);
        if (ContoursVector.at(i).getEndIndex() < (lastIndexOfFirstPart+(lastIndexOfFirstPart-firstValueIndex)/2))
                ContoursVector.at(i).setImp(true);
    }
    else
        ContoursVector.at(i).setLocation(END);
}
void ContoursDetector::classification()
{
    classificator = new Classificator(lastIndexOfFirstPart,lastIndexOfCenterPart, longestContourLength);

    for(Contour contour : ContoursVector)
        classificator->addContour(contour);

    result = classificator->classification();
    analysisResults = classificator->getAnalysisResult();
    stateChanges = classificator->getStateChanges();
    qDebug()<<"result "<<result;
    delete classificator;
}
void ContoursDetector::findLongest()
{
    int longest = 0;
    int longestLength = 0;
    for (int i = 0;i<ContoursVector.size();i++)
    {
        if (ContoursVector.at(i).getContourLength()>longestLength)
        {
            longestLength = ContoursVector.at(i).getContourLength();
            longest = i;
        }
    }
    longestContourLength = longestLength;
    ContoursVector.at(longest).setAsLongest();
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
            if(currentContour.getSize()>1)
            {
                if(value < currentContour.getLastValue())
                    currentContour.falling = true;
            }

            currentContour.addValue(value);
}
    }


    for(int i = 0;i<ContoursVector.size();)
    {
        double averageWithoutCurrentContour = sumAllValues - ContoursVector.at(i).getCenterOfRegressionLine();
        averageWithoutCurrentContour /= (ContoursVector.size()-1);
        if((ContoursVector.at(i).getCenterValue() > (averageWithoutCurrentContour*1.2))
                && (ContoursVector.at(i).getContourLength()<10)               )
        {
            ContoursVector.erase(ContoursVector.begin()+i);
        }
        else
        {
            setContourLocation(i);
            i++;
        }

    }

    for(int i = 0;i<ContoursVector.size();i++)
    {

        qDebug()<<i;
            if (ContoursVector.at(i).getMax() > maxValue) maxValue = ContoursVector.at(i).getMax();
            if (ContoursVector.at(i).getMin() < minValue && ContoursVector.at(i).getMin() > F0_MIN) minValue = ContoursVector.at(i).getMin();

    }


    calcRegressionLines();
    findLongest();


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
        if ((currentContour.getFirstValue()> (20.0+ContoursVector.back().getLastValue()))
                || ((currentContour.getLastValue() - ContoursVector.back().getFirstValue()) > 100) )
        {
            currentContour.setStartState(GROWTH);
        }
        else if (((ContoursVector.back().getLastValue() - currentContour.getFirstValue())
                 >(ContoursVector.back().getLastValue()/4))
                 || ((ContoursVector.back().getFirstValue() - currentContour.getLastValue()) > 100))
        {
            currentContour.setStartState(DROP);
        }
    }

    ContoursVector.push_back(currentContour);


    currentContour.clear();
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

        prevA=A;
        prevB=B;

        QLineSeries *lineSeries = new QLineSeries();
        lineSeries->append(ContoursVector.at(i).getStartIndex(), B);
        lineSeries->append(ContoursVector.at(i).getEndIndex(),A * (ContoursVector.at(i).getContourLength()) + B);
        ContoursVector.at(i).setRegresionLine(lineSeries);
        double centerRegresionLine;
        if (A > 0.1)
        {
            centerRegresionLine = A * (ContoursVector.at(i).getContourLength()) + B;
        }
        else
            centerRegresionLine =  B;

          //  centerRegresionLine = A * (ContoursVector.at(i).getCenter()-ContoursVector.at(i).getStartIndex()) + B;
        ContoursVector.at(i).setCoefA(A);
        ContoursVector.at(i).setCoefB(B);
        ContoursVector.at(i).setCenterRegressionLine(centerRegresionLine);
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
