#include "classificatornew.h"
#include <QDebug>

QString ClassificatorNew::classification()
{
    double firstValue = 0.0;
    double secondValue = 0.0;
    for( int i = segments.size()-1; i>segments.size()/2-1;i--)
    {
        if (firstValue == 0.0)
        {
            if (segments.at(i).getSegmentLength() > 2 && segments.at(i).getCoefA() > 0.0)
                firstValue = segments.at(i).getCenterOfRegressionLine();
            else
                continue;
        }
        if (secondValue == 0.0)
        {
            if (segments.at(i-1).getSegmentLength() > 2 && segments.at(i).getCoefA() > 0.0)
                secondValue = segments.at(i-1).getCenterOfRegressionLine();
            else {
                continue;
            }

        }
        double difference = firstValue - secondValue;
        qDebug()<<i << " "<<difference;

        if(difference > 20)
        {
            if(i>1)
            {
                if(firstValue > segments.at(i-2).getCenterOfRegressionLine())
                {
                    qDebug()<<"pytanie rozstrzygniecia? ";
                    return "pytanie rozstrzygniecia? ";
                }
            }
            else {
                qDebug()<<"pytanie rozstrzygniecia? ";
                return "pytanie rozstrzygniecia? ";
            }

        }
        firstValue = 0.0;
        secondValue = 0.0;

    }
    for(int i = 0;i<segments.size()/2;i++)
    {
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
        qDebug()<<"difference dopelnienie "<<difference;
        if (difference > (secondValue/3) &&  segments.at(i+1).getCoefA() < 0.1 )
        {
            qDebug()<<"pytanie dopelnienia";
            return "pytanie dopelnienia";
        }
        secondValue = segments.at(i+1).getMin();
        difference = firstValue - secondValue;
        qDebug()<<"difference dopelnienie2 "<<difference;

        if (difference > (secondValue/3) &&  segments.at(i+1).getCoefA() < 0.1  )
        {
            qDebug()<<"pytanie dopelnienia";
            //return "pytanie dopelnienia2";
        }
        firstValue = 0.0;
        secondValue = 0.0;
    }
    return " ";
   /* for (int i = 1; i<segments.size();i++)
    {
        double difference = segments.at(i).getCenterOfRegressionLine() - segments.at(i-1).getCenterOfRegressionLine();
        if(difference > 30.0)
        {
            qDebug()<<" wiekszy o ponad 30 "<<segments.at(i).getCenterOfRegressionLine() <<" diff "<<difference;
        }
        else if(difference <  -30.0)
        {
            qDebug()<<" mniejszy o ponad 30 "<<segments.at(i).getCenterOfRegressionLine()<<" diff "<<difference;
        }

    }*/
}
