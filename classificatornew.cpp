#include "classificatornew.h"
#include <QDebug>

QString ClassificatorNew::classification()
{
    double firstValue = 0.0;
    double secondValue = 0.0;
    int loopEnd;
    segments.size() > 4 ? loopEnd = segments.size()-4 : loopEnd = 0;
    for( int i = segments.size()-1; i>loopEnd;i--)
    {
        if (firstValue == 0.0)
        {
            if (segments.at(i).getSegmentLength() > 2 )//&& segments.at(i).getCoefA() > 0.0)
                firstValue = segments.at(i).getCenterOfRegressionLine();
            else
                continue;
        }
        if (secondValue == 0.0)
        {
            if (segments.at(i-1).getSegmentLength() > 2 )//&& segments.at(i).getCoefA() > 0.0)
                secondValue = segments.at(i-1).getCenterOfRegressionLine();
            else {
                continue;
            }

        }
        double difference = firstValue - secondValue;
        qDebug()<<i << " diff "<<difference;

        if(difference > 20 && (segments.at(i).getCoefA() > -0.3 || segments.at(i-1).getCoefA() > -0.3))
        {
            bool biggerThanFewPrevious = true;
            for (int j = i-1; j>(i-4);j--)
            {
                if(j<1)break;
                if (segments.at(j).getCenterOfRegressionLine() > firstValue)
                {
                    biggerThanFewPrevious = false;
                    break;
                }
            }
            for( int j = i-1;j > segments.size()/2;j--)
            {
                if (firstValue < (segments.at(j).getCenterOfRegressionLine() + 10.0))
                    biggerThanFewPrevious = false;
            }
            if (segments.at(i).getSegmentLength() < 10 && firstValue > (secondValue + 100.0))
                biggerThanFewPrevious = false;
            if(biggerThanFewPrevious)
                return "pytanie rozstrzygniecia? ";
        }
        firstValue = 0.0;
        secondValue = 0.0;

    }



    bool endRising = true;
    int indexPreviousSegment = -1;
    int firstValidFromEnd = 0;
    for (int i = segments.size()-1; i >=0;i--)
    {
        if(segments.at(i).getSegmentLength() > 5)
        {
            firstValidFromEnd = i;
            break;
        }
    }
    if (segments.at(firstValidFromEnd).getSegmentLength() > 50)
        firstValidFromEnd = 0;

    for (int i = firstValidFromEnd;i > firstValidFromEnd -3;i--)
    {
        if(i==0) break;
        if (segments.at(i).getSegmentLength() < 5)
            continue;
        if (segments.at(i).getCenterOfRegressionLine() < 200.0)
        {
            endRising = false;
            break;
        }
        indexPreviousSegment = i-1;
        while(true)
        {
            if(segments.at(indexPreviousSegment).getSegmentLength() > 5)
                break;
            if (indexPreviousSegment == 0)
            {
                indexPreviousSegment = -1;
                break;
            }
            indexPreviousSegment--;
        }

        if (indexPreviousSegment == -1)
            break;
        if (segments.at(indexPreviousSegment).getCenterOfRegressionLine() < 200.0)
            endRising = false;
        if((segments.at(i).getCenterOfRegressionLine() < (segments.at(indexPreviousSegment).getCenterOfRegressionLine() -5.0))
                && segments.at(indexPreviousSegment).getCenterOfRegressionLine() > segments.at(firstValidFromEnd).getCenterOfRegressionLine())
        {
            qDebug()<<"MNIEJSZE "<<segments.at(i).getCenterOfRegressionLine()<<" " <<segments.at(indexPreviousSegment).getCenterOfRegressionLine();
            endRising = false;
            break;
        }
        if (i == 1) break;
    }
    if (endRising && indexPreviousSegment != -1)
        return "pytanie rozstrzygnięcia end";
    firstValue = 0.0;
    secondValue = 0.0;
    for(int i = 0;i<segments.size()/2;i++)
    {
        qDebug()<<"first Value "<<firstValue<<" "<<secondValue;
        qDebug()<<segments.at(i).getCenterOfRegressionLine()<<" "<<segments.at(i+1).getCenterOfRegressionLine();
        qDebug()<<segments.at(i).getSegmentLength()<<" "<<segments.at(i+1).getSegmentLength();

        if (firstValue == 0.0)
        {
            if (segments.at(i).getSegmentLength() > 5)
                firstValue = segments.at(i).getCenterOfRegressionLine();
            else
                continue;
        }
        if (secondValue == 0.0)
        {
            if (segments.at(i+1).getSegmentLength() > 5)
                secondValue = segments.at(i+1).getCenterOfRegressionLine();
            else {
                continue;
            }
        }

        double difference = firstValue - secondValue;
        qDebug()<<"difference dopelnienie "<<difference<<" "<<firstValue<<" "<<secondValue;


        if (difference > (secondValue/3) &&  segments.at(i+1).getCoefA() < 0.1 && firstValue > 200.0 )
        {
            qDebug()<<"pytanie dopelnienia";
            if (i+2 < segments.size())
            {
                if (segments.at(i+1).getCenterOfRegressionLine() > (segments.at(i+2).getCenterOfRegressionLine()-10.0))
                    return "pyt dopelnienia 1 "+QString::number(firstValue)+" ";
            }
            else {
                return "pyt dopelnienia 2 "+QString::number(firstValue)+" ";
            }
        }
        if(segments.at(i+1).getCoefA() < -2.0 && (segments.at(i).getCoefA() > -4.0 && segments.at(i+1).getCoefA()>-4.0))
        {
            secondValue = segments.at(i+1).getMin();
        difference = firstValue - secondValue;
        qDebug()<<"difference dopelnienie2 "<<difference;

        if (difference > (secondValue/3) &&  segments.at(i+1).getCoefA() < 0.1  && firstValue > 200.0 )
        {
            qDebug()<<"pytanie dopelnienia";
            if (i+2 < segments.size())
            {
                if (segments.at(i+1).getCenterOfRegressionLine() > (segments.at(i+2).getCenterOfRegressionLine()-10.0))
                    return "pyt dopelnienia 3 "+QString::number(firstValue)+" ";
            }
            else {
                return "pyt dopelnienia 4 "+QString::number(firstValue)+" ";
            }
        }

        }
        firstValue = 0.0;
        secondValue = 0.0;
    }

    Segment highestSegmentFirstHalf = segments.at(0);

    int indexHighestSegment = 0;
    for(int i = 1;i < segments.size()/2;i++)
    {
        if(segments.at(i).getCenterOfRegressionLine() > highestSegmentFirstHalf.getCenterOfRegressionLine()
                && segments.at(i).getCoefA() > -3.0
                && segments.at(i).getSegmentLength() > 5.0)
        {
            highestSegmentFirstHalf = segments.at(i);
            indexHighestSegment = i;
        }
    }
    if ((highestSegmentFirstHalf.getCoefA() > 0.6 && highestSegmentFirstHalf.getSegmentLength()>20)
            || (highestSegmentFirstHalf.getCoefA() > 0.6 && highestSegmentFirstHalf.getCoefA() < 3.0
            && highestSegmentFirstHalf.getSegmentLength() > 10))
        return "rozkaz1";
    else if (highestSegmentFirstHalf.getCoefA() > 0.3 && highestSegmentFirstHalf.getSegmentLength()>60)
        return "rozkaz2";
    if (indexHighestSegment > 0 && segments.at(indexHighestSegment-1).getCoefA() < 0.1
            && segments.at(indexHighestSegment-1).getSegmentLength()>5.0
            && segments.at(indexHighestSegment).getRange() > 5.0
            && std::abs(segments.at(indexHighestSegment).getCoefA())<3.0)
    {
        if (highestSegmentFirstHalf.getCenterOfRegressionLine() > (segments.at(indexHighestSegment-1).getEndRegressionLine()+20.0))
            return "rozkaz3";
    }

    if(highestSegmentFirstHalf.getSegmentLength() > 10.0 && highestSegmentFirstHalf.getCoefA() < -0.5
            && segments.at(indexHighestSegment+1).getCoefA()<-0.2
            && highestSegmentFirstHalf.getCenterOfRegressionLine() > (segments.at(indexHighestSegment+1).getCenterOfRegressionLine() + 10.0))
    {
        if (indexHighestSegment > 0 && highestSegmentFirstHalf.getCenterOfRegressionLine() > (segments.at(indexHighestSegment-1).getCenterOfRegressionLine() + 10.0))
           {
         qDebug()<<   highestSegmentFirstHalf.getCenterOfRegressionLine();
         qDebug() << segments.at(indexHighestSegment-1).getCenterOfRegressionLine();
         qDebug() <<"rozkaz4";
            return "rozkazz4";
        }
        else if (indexHighestSegment == 0)
        {
            qDebug()<<   highestSegmentFirstHalf.getCenterOfRegressionLine();
            qDebug() << segments.at(indexHighestSegment+1).getCenterOfRegressionLine();
            qDebug() <<"rozkaz4 "<<indexHighestSegment;
                return "rozkazz4";
        }

    }
    int indexNextValidateAfterHighest = 0;
    for (int i = indexHighestSegment+1; i < segments.size(); i++)
    {
        if (segments.at(i).getSegmentLength() > 5)
        {
            indexNextValidateAfterHighest = i;
            break;
        }
    }
    double spaceBetweenHighestAndNext = segments.at(indexNextValidateAfterHighest).getStartIndex() - highestSegmentFirstHalf.getEndIndex();
    qDebug()<<"startIndex "<<segments.at(indexNextValidateAfterHighest).getStartIndex()<<" end "<<highestSegmentFirstHalf.getEndIndex();
    if ((spaceBetweenHighestAndNext > highestSegmentFirstHalf.getSegmentLength() && highestSegmentFirstHalf.getCoefA() > -0.5
            && highestSegmentFirstHalf.getSegmentLength() > 10))
        return "rozkazz5";

    int indexSegmentBeforeHighest = -1;
    for (int i = indexHighestSegment-1; i >=0; i--)
    {
        if (segments.at(i).getSegmentLength() > 5)
        {
           indexSegmentBeforeHighest = i;
           break;
        }
    }
    if (indexSegmentBeforeHighest == -1)
        return "zdanie twierdzace";

    double spaceBetweenHighestAndPrevious = highestSegmentFirstHalf.getStartIndex() - segments.at(indexSegmentBeforeHighest).getEndIndex();
    qDebug()<<"startIndex "<<highestSegmentFirstHalf.getStartIndex()<<" end "<<segments.at(indexSegmentBeforeHighest).getEndIndex();
    /*if ((spaceBetweenHighestAndPrevious > highestSegmentFirstHalf.getSegmentLength() && segments.at(indexSegmentBeforeHighest).getCoefA() > -0.5
        && highestSegmentFirstHalf.getSegmentLength() > 10))
        return "rozkazz6";*/
    return "zdanie twierdzace";
  /*  double averageFirstHalf = 0.0;
    int counter = 0;
    for (int i = 0; i< segments.size()/2;i++)
    {
        averageFirstHalf += segments.at(i).getCenterOfRegressionLine();
        counter++;
    }
    averageFirstHalf /=counter;
    for (int i = 0; i< segments.size()/2;i++)
    {
        if (std::abs(segments.at(i).getCenterOfRegressionLine() - averageFirstHalf)>10)
            return " ";
    }
    return "zdanie twierdzace";
*/
}
