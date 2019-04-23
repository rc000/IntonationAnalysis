#include "classificator.h"
#include <string>
#include<QDebug>
#include <sstream>





#define centerHasSegmentWithBiggerF0ValueThanEnd (1<<1)

#define highestSegmentLocatedBetweenStartEndCenter (1<<2)

#define centerHighestSegmentSteeplyFalling (1<<3)


#define allSegmentsAreFalling (1<<4)
#define endHasSegmentWithBiggerF0ValueThanStart (1<<5)

#define endHasSegmentWithMuchBiggerF0ValueThanCenter (1<<6)

#define startHasSegmentWithSlightlyBiggerF0ValueThanCenter (1<<7)

#define bigGrowthAtTheBeginning (1<<8)

#define centerHighestSegmentNotSteeplyFalling (1<<9)

#define bigGrowthAtTheEnd (1<<10)
#define startHasSegmentWithMuchBiggerF0ValueThanCenter (1<<11)

#define bigDropAtTheBeginning (1<<12)

#define highestSegmentAtBeginningStronglyRising (1<<13)

#define centerSegmentIsHighestAndSteeplyFalling (1<<14)
#define impRegion (1<<15)
#define longSegment (1<<16)


static const char *analysisResults[] =
{
    "centerHasSegmentWithBiggerF0ValueThanEnd",
    "highestSegmentLocatedBetweenStartEndCenter",
    "centerHighestSegmentSteeplyFalling",
    "allSegmentsAreFalling",
    "endHasSegmentWithBiggerF0ValueThanStart",
    "endHasSegmentWithMuchBiggerF0ValueThanCenter",
    "startHasSegmentWithSlightlyBiggerF0ValueThanCenter",
    "bigGrowthAtTheBeginning",
    "centerHighestSegmentNotSteeplyFalling",
    "bigGrowthAtTheEnd",
    "startHasSegmentWithMuchBiggerF0ValueThanCenter",
    "bigDropAtTheBeginning",
    "highestSegmentAtBeginningStronglyRising",
    "centerSegmentIsHighestAndSteeplyFalling",
    "impRegion",
    "longSegment"
};

int numberOfMaxResults = 16;
int declarative = 0;
int conclusiveQuestion = 0;
int completenessQuestion = 0;
int imperative = 0;
int notImperative = 0;

int notDeclarative = 0;


double highestSegmentValue = 0.0;
int indexOfHighestValue = 0;

void initialization()
{
   declarative|= (startHasSegmentWithSlightlyBiggerF0ValueThanCenter
                  | allSegmentsAreFalling
                  |centerSegmentIsHighestAndSteeplyFalling);

   notDeclarative |= (endHasSegmentWithBiggerF0ValueThanStart
                       |endHasSegmentWithMuchBiggerF0ValueThanCenter
                       |bigGrowthAtTheEnd
                       |startHasSegmentWithMuchBiggerF0ValueThanCenter
                       |highestSegmentLocatedBetweenStartEndCenter);




   conclusiveQuestion |=  (endHasSegmentWithMuchBiggerF0ValueThanCenter
                          | endHasSegmentWithBiggerF0ValueThanStart
                          | bigGrowthAtTheEnd);

   completenessQuestion |= (startHasSegmentWithMuchBiggerF0ValueThanCenter);




   imperative |= (highestSegmentLocatedBetweenStartEndCenter
                 |centerHighestSegmentNotSteeplyFalling
                  |highestSegmentAtBeginningStronglyRising
                  |impRegion
                  |longSegment);
   notImperative |= (endHasSegmentWithMuchBiggerF0ValueThanCenter
                     | endHasSegmentWithBiggerF0ValueThanStart);

}



QString bin(unsigned n)
{
    QString result;
    unsigned i;
    int j =0;
    for (i = 1 << 31; i > 0; i = i / 2)
    {
        (n & i)? result+='1': result+='0';
        result+=' ';
        j++;
    }
    return result;
}

Classificator::Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart, double length)
    :lastIndexOfBeginningPart(lastIndexOfBeginningPart),lastIndexOfCenterPart(lastIndexOfCenterPart),
      longestSegmentLength(length)
{
    highestValueOfRegresionLinesAtTheBeginning = 0.0;
    highestValueOfRegresionLinesAtTheCenter = 0.0;
    highestValueOfRegresionLinesAtTheEnd = 0.0;
    features = 1;
    initialization();
}

bool Classificator::areAllSegmentsFalling()
{
    for (int i=0;i<segments.size();i++)
    {
        if(segments.at(i).getCoefA()>0.2 )
        {
            return false;
         }
     }
    return true;
}
std::vector<QString> Classificator::getAnalysisResult()
{
    std::vector<QString> results;
    unsigned i;
    int j =numberOfMaxResults-1;
    for (i = 1 << numberOfMaxResults; i > 0; i = i / 2)
    {
        if(features & i)
        {
            results.push_back(analysisResults[j]);
            qDebug()<<analysisResults[j];
            qDebug()<<" ";
        };
        j--;
    }
    return results;
}
void Classificator::analysis()
{
    highestSegmentValue = 0.0;
    indexOfHighestValue = 0;
    bool isDropAtEnd = false;
    bool isGrowthAtEnd = false;
    double min = 400;
    double max = 0.0;
    double range = 0;
    for (int i = 0;i<segments.size();i++)
    {
        if (segments.at(i).getMax()>max)max = segments.at(i).getMax();
        if (segments.at(i).getMin()<min)min = segments.at(i).getMin();

        std::ostringstream ss;
        ss<<i<<" "<<segments.at(i).getCoefA()<<" "<<segments.at(i).getCenterOfRegressionLine()<<" length "<<segments.at(i).getSegmentLength()
         <<" range "<<segments.at(i).getMax()-segments.at(i).getMin()<<" size "<<segments.at(i).getSize();
        stateChanges.push_back(QString::fromStdString(ss.str()));

        if (segments.at(i).getLocationOnTheChart() == BEGINNING)
        {
            if(segments.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheBeginning)
            {
                highestValueOfRegresionLinesAtTheBeginning = segments.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheBeginning = i;

                startHighestSegment = segments.at(i);
            }
            if (segments.at(i).getStartState() == DROP)
                    features |= bigDropAtTheBeginning;
            else if (segments.at(i).getStartState() == GROWTH)
                features |= bigGrowthAtTheBeginning;
        }
        if (segments.at(i).getLocationOnTheChart() == CENTER)
        {
            if (segments.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheCenter)
            {
                highestValueOfRegresionLinesAtTheCenter = segments.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheCenter = i;
                centerHighestSegment = segments.at(i);
            }
        }
        if (segments.at(i).getLocationOnTheChart() == END)
        {
            if (segments.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheEnd)
            {
                highestValueOfRegresionLinesAtTheEnd = segments.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheEnd = i;
                endHighestSegment = segments.at(i);

            }
            if (segments.at(i).getStartState() == GROWTH && segments.at(i).getSegmentLength() >3)
            {
                if (segments.at(i).getCenterOfRegressionLine() > centerHighestSegment.getCenterOfRegressionLine()
                               || segments.at(i).getCenterOfRegressionLine() > (30.0+segments.at(i-1).getCenterOfRegressionLine()))
                {
                    isGrowthAtEnd = true;
                    ss<<i<<" big growth "<<segments.at(i).getStartIndex()<<" "<<segments.at(i).getEndIndex();
                    stateChanges.push_back(QString::fromStdString(ss.str()));
                }
            }
            if (segments.at(i).getStartState() == DROP)
            {
                isDropAtEnd = true;
                ss<<i<<" big drop "<<segments.at(i).getStartIndex()<<" "<<segments.at(i).getEndIndex();
                stateChanges.push_back(QString::fromStdString(ss.str()));
            }
        }
        if (segments.at(i).getCenterOfRegressionLine() > highestSegmentValue)
        {
            highestSegmentValue = segments.at(i).getCenterOfRegressionLine();
            indexOfHighestValue =i;
        }
        if ((isGrowthAtEnd) && (!isDropAtEnd))
            features |= bigGrowthAtTheEnd;

    }




    if (endHighestSegment.getCenterOfRegressionLine() > 1.5*centerHighestSegment.getCenterOfRegressionLine())
    {
        features |= endHasSegmentWithMuchBiggerF0ValueThanCenter;
    }
    if (endHighestSegment.getCenterOfRegressionLine() > (20.0+startHighestSegment.getCenterOfRegressionLine())
            && endHighestSegment.getSegmentLength()>2)
    {
        features |= endHasSegmentWithBiggerF0ValueThanStart;
    }




    if (startHighestSegment.getCenterOfRegressionLine() > 1.45*centerHighestSegment.getCenterOfRegressionLine())
    {
        features |= startHasSegmentWithMuchBiggerF0ValueThanCenter;
    }
    else if (startHighestSegment.getCenterOfRegressionLine() > (centerHighestSegment.getCenterOfRegressionLine()-5.0))
    {
        features |= startHasSegmentWithSlightlyBiggerF0ValueThanCenter;
    }

    if (startHighestSegment.getCoefA() > 0.5 && startHighestSegment.falling )
        features |= highestSegmentAtBeginningStronglyRising;



    if (areAllSegmentsFalling())
    {
        features |= allSegmentsAreFalling;

     }



    if (centerHighestSegment.getCenterOfRegressionLine() > endHighestSegment.getCenterOfRegressionLine())
    {
         features |= centerHasSegmentWithBiggerF0ValueThanEnd;
    }

    if  (features & centerHasSegmentWithBiggerF0ValueThanEnd)
    {
        if(startHighestSegment.getCenterOfRegressionLine()>centerHighestSegment.getCenterOfRegressionLine() && startHighestSegment.getSegmentLength()>(longestSegmentLength-10)
                && std::abs(startHighestSegment.getCoefA())>0.2)
            features |= longSegment;
        else if(startHighestSegment.getCenterOfRegressionLine() < centerHighestSegment.getCenterOfRegressionLine() && centerHighestSegment.getSegmentLength()>(longestSegmentLength-10)
                 && std::abs(centerHighestSegment.getCoefA())>0.3)
            features |= longSegment;

        double CoefA;
        if ((startHighestSegment.getCenterOfRegressionLine() > centerHighestSegment.getCenterOfRegressionLine())
            && startHighestSegment.getEndIndex() > lastIndexOfBeginningPart)
        {
            CoefA = startHighestSegment.getCoefA();

            features |= highestSegmentLocatedBetweenStartEndCenter;
        }
        else if ((centerHighestSegment.getCenterOfRegressionLine() > startHighestSegment.getCenterOfRegressionLine())
                   && centerHighestSegment.getStartIndex() < lastIndexOfBeginningPart)
         {
            CoefA = centerHighestSegment.getCoefA();
            features |= highestSegmentLocatedBetweenStartEndCenter;
         }
        else if(centerHighestSegment.getCenterOfRegressionLine() > startHighestSegment.getCenterOfRegressionLine())
        {
              CoefA = centerHighestSegment.getCoefA();
              if(centerHighestSegment.getImp() && centerHighestSegment.getCoefA()>-0.2)
                  features |= impRegion;
        }
        else if(centerHighestSegment.getCenterOfRegressionLine() < startHighestSegment.getCenterOfRegressionLine())
        {
            if(startHighestSegment.getImp() && startHighestSegment.getCoefA()>-0.2)
                features |= impRegion;
        }
        if ((CoefA < -0.4) || (indexHighestValueOfRegresionLinesAtTheCenter > (segments.size()/2)))
             features |= centerHighestSegmentSteeplyFalling;
        else if (CoefA < 0.0)
             features |= centerHighestSegmentNotSteeplyFalling;

    }
    std::ostringstream ss;
    range = max - min;
    ss<<"start "<<startHighestSegment.getCenterOfRegressionLine()<<" center "<<centerHighestSegment.getCenterOfRegressionLine()
     <<" end "<<endHighestSegment.getCenterOfRegressionLine();
    ss<<"min "<<min<<" max "<<max<<" range "<<range<<"percent "<<range/max*100;
    stateChanges.push_back(QString::fromStdString(ss.str()));
    if((indexOfHighestValue == indexHighestValueOfRegresionLinesAtTheCenter)
            && (features & centerHighestSegmentSteeplyFalling))
        features |= centerSegmentIsHighestAndSteeplyFalling;
}
std::vector<QString> Classificator::classification()
{

    short flag = 0;
    short decl = 1;
    short imper = 2;
    analysis();
    qDebug()<<bin(features);
    qDebug()<<bin(declarative);

    qDebug()<<bin(conclusiveQuestion);
    qDebug()<<bin(completenessQuestion);
    qDebug()<<bin(imperative);
    std::vector<QString> result;

    if ((features & declarative)
            && (!(features & notDeclarative)))
    {
        flag |= decl;
        result.emplace_back("zdanie twierdzace zwykle");
    }

    if (features & completenessQuestion)
         result.emplace_back("pytanie uzupelnienia");

    if (features & conclusiveQuestion)
        result.emplace_back("pytanie rozstrzygniecia");

    if (((features & imperative))
            && (!(features & notImperative)))
    {
        flag |= imper;
        result.emplace_back("rozkazujace");
    }

    if ((flag & decl)&&(flag & imper))
    {
        if (startHighestSegment.getSegmentLength()>10)
            result.erase(result.begin()+0);
        else {
            result.erase(result.begin()+1);
        }
        result.emplace_back("*");
    }

    if(result.size()==0)
        result.emplace_back("unrecognized");
    return result;
}
