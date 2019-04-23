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
        qDebug()<<i << " "<<firstValue<<" "<<secondValue;

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
        qDebug()<<i << " "<<difference;

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
            if(biggerThanFewPrevious)
                return "pytanie rozstrzygniecia? ";
        }
        firstValue = 0.0;
        secondValue = 0.0;

    }
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
                    return "pyt dopelnienia "+QString::number(firstValue)+" ";
            }
            else {
                return "pyt dopelnienia "+QString::number(firstValue)+" ";
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
                    return "pyt dopelnienia "+QString::number(firstValue)+" ";
            }
            else {
                return "pyt dopelnienia "+QString::number(firstValue)+" ";
            }
        }

        }
        firstValue = 0.0;
        secondValue = 0.0;
    }
    return " ";
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
