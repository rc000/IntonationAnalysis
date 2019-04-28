#include "segmentsdetector.h"

#include <QDebug>
#define TRANSITION 20

SegmentsDetector::SegmentsDetector(ExtractionHelper extractionHelper)
{
    this->extractionHelper = extractionHelper;
    this->seriesSegments =  new QScatterSeries();
    averageValue = 0.0;
    numberOfPositiveValues = 0;

}
SegmentsDetector::~SegmentsDetector()
{
}

int SegmentsDetector::findIndexOfLastF0Value()
{
    for(size_t i=extractionHelper.f0_size()-1;i>1;i--)
    {
        if (extractionHelper.f0_value(i) > 50)
        {
            return i;
        }
    }
}
void SegmentsDetector::setSegmentLocation(int i)
{
    if (SegmentsVector.at(i).getCenter() < lastIndexOfFirstPart)
    {
        SegmentsVector.at(i).setLocation(BEGINNING);
        if (SegmentsVector.at(i).getStartIndex() > (lastIndexOfFirstPart-(lastIndexOfFirstPart-firstValueIndex)/2))
        {
                qDebug()<<"Start Index "<<SegmentsVector.at(i).getStartIndex();
                qDebug()<<(lastIndexOfFirstPart-(lastIndexOfFirstPart-firstValueIndex)/2);
                SegmentsVector.at(i).setImp(true);
        }
    }
    else if (SegmentsVector.at(i).getCenter() < lastIndexOfCenterPart)
    {
        SegmentsVector.at(i).setLocation(CENTER);
        if (SegmentsVector.at(i).getEndIndex() < (lastIndexOfFirstPart+(lastIndexOfFirstPart-firstValueIndex)/2))
                SegmentsVector.at(i).setImp(true);
    }
    else
        SegmentsVector.at(i).setLocation(END);
}
void SegmentsDetector::classification()
{
    classificator = new Classificator(lastIndexOfFirstPart,lastIndexOfCenterPart, longestSegmentLength);
    classificatorNew = new ClassificatorNew();
    for(Segment segment : SegmentsVector)
    {
        classificator->addSegment(segment);
        classificatorNew->addSegment(segment);
    }
   classificator->classification();
    //result = classificator->classification();
    result.emplace_back(classificatorNew->classification());
    analysisResults = classificator->getAnalysisResult();
    stateChanges = classificator->getStateChanges();
    qDebug()<<"result "<<result;
    delete classificator;
}
void SegmentsDetector::findLongest()
{
    int longest = 0;
    int longestLength = 0;
    for (int i = 0;i<SegmentsVector.size();i++)
    {
        if (SegmentsVector.at(i).getSegmentLength()>longestLength)
        {
            longestLength = SegmentsVector.at(i).getSegmentLength();
            longest = i;
        }
    }
    longestSegmentLength = longestLength;
    SegmentsVector.at(longest).setAsLongest();
}
void SegmentsDetector::findSegments()
{
    currentSegment.setStart(1);
    lastValueIndex = findIndexOfLastF0Value();

    for(size_t i=1;i<extractionHelper.f0_size();i++)
    {
        double value =extractionHelper.f0_value(i);

        double previousValue = extractionHelper.f0_value(i-1);
        seriesSegments->append(i,value);

        if(std::abs(value - previousValue) > TRANSITION || i == extractionHelper.f0_size() - 1)
        {
            currentSegment.setEnd(i-1);
            currentSegment.setCenter();
            foundNewSegment();
            currentSegment.setStart(i);
            currentSegment.addValue(value);
        }
        else
        {
            if(currentSegment.getSize()>1)
            {
                if(value < currentSegment.getLastValue())
                    currentSegment.falling = true;
            }

            currentSegment.addValue(value);
}
    }

    calcRegressionLines();

    for(int i = 0;i<SegmentsVector.size()-1;)
    {

        double averageWithoutCurrentSegment = sumAllValues - SegmentsVector.at(i).getCenterOfRegressionLine();
        averageWithoutCurrentSegment /= (SegmentsVector.size()-1);
        qDebug()<<SegmentsVector.at(i).getSegmentLength()<<" blabla "<<SegmentsVector.at(i).getCoefA();
        if(((SegmentsVector.at(i).getCenterValue() > (averageWithoutCurrentSegment*1.2))
                && ((SegmentsVector.at(i).getSegmentLength()<5) || ((SegmentsVector.at(i).getSegmentLength()<10) && (SegmentsVector.at(i).getCoefA()< 2.0))) ))
            /*|| (ContoursVector.at(i).getCenterValue() > (averageWithoutCurrentContour*1.1))
                && (ContoursVector.at(i).getContourLength()<3))*/
        {

            SegmentsVector.erase(SegmentsVector.begin()+i);
        }
        else if(((SegmentsVector.at(i).getCenterValue() < (averageWithoutCurrentSegment*0.7))
                 && (SegmentsVector.at(i).getSegmentLength()<7)))
        {
            SegmentsVector.erase(SegmentsVector.begin()+i);

        }
        else
        {
            setSegmentLocation(i);
            i++;
        }

    }

    for(int i = 0;i<SegmentsVector.size();i++)
    {

            if (SegmentsVector.at(i).getMax() > maxValue) maxValue = SegmentsVector.at(i).getMax();
            if (SegmentsVector.at(i).getMin() < minValue && SegmentsVector.at(i).getMin() > F0_MIN) minValue = SegmentsVector.at(i).getMin();

    }


    findLongest();


}
void SegmentsDetector::foundNewSegment()
{
    if (!currentSegment.isSegmentValidate())
    {
        currentSegment.clear();
        return;
    }
    sumAllValues += currentSegment.getCenterValue();

    if (lastIndexOfFirstPart == 0)
    {
        firstValueIndex = currentSegment.getStartIndex();
        double range = lastValueIndex-firstValueIndex;
        lastIndexOfFirstPart = currentSegment.getStartIndex()+range/4;
        lastIndexOfCenterPart = lastValueIndex - range/4;
    }

    if (SegmentsVector.size()>0)
    {
        if ((currentSegment.getFirstValue()> (20.0+SegmentsVector.back().getLastValue()))
                || ((currentSegment.getLastValue() - SegmentsVector.back().getFirstValue()) > 100) )
        {
            currentSegment.setStartState(GROWTH);
        }
        else if (((SegmentsVector.back().getLastValue() - currentSegment.getFirstValue())
                 >(SegmentsVector.back().getLastValue()/4))
                 || ((SegmentsVector.back().getFirstValue() - currentSegment.getLastValue()) > 100))
        {
            currentSegment.setStartState(DROP);
        }
    }

    SegmentsVector.push_back(currentSegment);


    currentSegment.clear();
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
void SegmentsDetector::calcRegressionLines()
{
    seriesRegresionLines.clear();
    double prevA = 0.0;
    double prevB = 0.0;
    for (int i=0;i<SegmentsVector.size();i++)
    {
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

        prevA=A;
        prevB=B;

        QLineSeries *lineSeries = new QLineSeries();
        lineSeries->append(SegmentsVector.at(i).getStartIndex(), B);
        lineSeries->append(SegmentsVector.at(i).getEndIndex(),A * (SegmentsVector.at(i).getSegmentLength()) + B);
        SegmentsVector.at(i).setRegresionLine(lineSeries);
        double centerRegresionLine;
        if (A > 0.1)
        {
            centerRegresionLine = A * (SegmentsVector.at(i).getSegmentLength()) + B;
        }
        else
            centerRegresionLine =  B;

          //  centerRegresionLine = A * (ContoursVector.at(i).getCenter()-ContoursVector.at(i).getStartIndex()) + B;
        SegmentsVector.at(i).setCoefA(A);
        SegmentsVector.at(i).setCoefB(B);
        SegmentsVector.at(i).setCenterRegressionLine(centerRegresionLine);
        SegmentsVector.at(i).setEndRegressionLine(A * (SegmentsVector.at(i).getSegmentLength()) + B);
    }
}
std::vector<QLineSeries*> SegmentsDetector::getSeriesRegresionLines()
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

int SegmentsDetector::getNextValidateSegment(int index)
{
    for(int i = index+1;i<SegmentsVector.size();i++)
    {
        if(!SegmentsVector.at(i).isSegmentValidate())
            continue;
        return i;
    }
    return -1;
}
