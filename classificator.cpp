#include "classificator.h"
#include <string>
#include<QDebug>
#include <sstream>





#define centerHasContourWithBiggerF0ValueThanEnd (1<<1)

#define highestContourLocatedBetweenStartEndCenter (1<<2)

#define centerHighestContourSteeplyFalling (1<<3)


#define allContoursAreFalling (1<<4)
#define endHasContourWithBiggerF0ValueThanStart (1<<5)

#define endHasContourWithMuchBiggerF0ValueThanCenter (1<<6)

#define startHasContourWithSlightlyBiggerF0ValueThanCenter (1<<7)

#define bigGrowthAtTheBeginning (1<<8)

#define centerHighestContourNotSteeplyFalling (1<<9)

#define bigGrowthAtTheEnd (1<<10)
#define startHasContourWithMuchBiggerF0ValueThanCenter (1<<11)

#define bigDropAtTheBeginning (1<<12)

#define highestContourAtBeginningStronglyRising (1<<13)

#define centerContourIsHighestAndSteeplyFalling (1<<14)
#define impRegion (1<<15)


static const char *analysisResults[] =
{
    "centerHasContourWithBiggerF0ValueThanEnd",
    "highestContourLocatedBetweenStartEndCenter",
    "centerHighestContourSteeplyFalling",
    "allContoursAreFalling",
    "endHasContourWithBiggerF0ValueThanStart",
    "endHasContourWithMuchBiggerF0ValueThanCenter",
    "startHasContourWithSlightlyBiggerF0ValueThanCenter",
    "bigGrowthAtTheBeginning",
    "centerHighestContourNotSteeplyFalling",
    "bigGrowthAtTheEnd",
    "startHasContourWithMuchBiggerF0ValueThanCenter",
    "bigDropAtTheBeginning",
    "highestContourAtBeginningStronglyRising",
    "centerContourIsHighestAndSteeplyFalling",
    "impRegion"
};

int numberOfMaxResults = 15;
int declarative = 0;
int conclusiveQuestion = 0;
int completenessQuestion = 0;
int imperative = 0;
int notImperative = 0;

int notDeclarative = 0;


double highestContourValue = 0.0;
int indexOfHighestValue = 0;

void initialization()
{
   declarative|= (startHasContourWithSlightlyBiggerF0ValueThanCenter
                  | allContoursAreFalling
                  |centerContourIsHighestAndSteeplyFalling);

   notDeclarative |= (endHasContourWithBiggerF0ValueThanStart
                       |endHasContourWithMuchBiggerF0ValueThanCenter
                       |bigGrowthAtTheEnd
                       |startHasContourWithMuchBiggerF0ValueThanCenter
                       |highestContourLocatedBetweenStartEndCenter);




   conclusiveQuestion |=  (endHasContourWithMuchBiggerF0ValueThanCenter
                          | endHasContourWithBiggerF0ValueThanStart
                          | bigGrowthAtTheEnd);

   completenessQuestion |= (startHasContourWithMuchBiggerF0ValueThanCenter);




   imperative |= (highestContourLocatedBetweenStartEndCenter
                 |centerHighestContourNotSteeplyFalling
                  |highestContourAtBeginningStronglyRising
                  |impRegion);
   notImperative |= (endHasContourWithMuchBiggerF0ValueThanCenter
                     | endHasContourWithBiggerF0ValueThanStart);

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

Classificator::Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart)
    :lastIndexOfBeginningPart(lastIndexOfBeginningPart),lastIndexOfCenterPart(lastIndexOfCenterPart)
{
    highestValueOfRegresionLinesAtTheBeginning = 0.0;
    highestValueOfRegresionLinesAtTheCenter = 0.0;
    highestValueOfRegresionLinesAtTheEnd = 0.0;
    features = 1;
    initialization();
}

bool Classificator::areAllContoursFalling()
{
    for (int i=0;i<contours.size();i++)
    {
        if(contours.at(i).getCoefA()>0.2 )
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
    highestContourValue = 0.0;
    indexOfHighestValue = 0;
    bool isDropAtEnd = false;
    bool isGrowthAtEnd = false;
    for (int i = 0;i<contours.size();i++)
    {
        std::ostringstream ss;
        ss<<i<<" "<<contours.at(i).getCoefA()<<" "<<contours.at(i).getCenterOfRegressionLine();
        stateChanges.push_back(QString::fromStdString(ss.str()));

        if (contours.at(i).getLocationOnTheChart() == BEGINNING)
        {
            if(contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheBeginning)
            {
                highestValueOfRegresionLinesAtTheBeginning = contours.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheBeginning = i;

                startHighestContour = contours.at(i);
            }
            if (contours.at(i).getStartState() == DROP)
                    features |= bigDropAtTheBeginning;
            else if (contours.at(i).getStartState() == GROWTH)
                features |= bigGrowthAtTheBeginning;
        }
        if (contours.at(i).getLocationOnTheChart() == CENTER)
        {
            if (contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheCenter)
            {
                highestValueOfRegresionLinesAtTheCenter = contours.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheCenter = i;
                centerHighestContour = contours.at(i);
            }
        }
        if (contours.at(i).getLocationOnTheChart() == END)
        {
            if (contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheEnd)
            {
                highestValueOfRegresionLinesAtTheEnd = contours.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheEnd = i;
                endHighestContour = contours.at(i);

            }
            if (contours.at(i).getStartState() == GROWTH && contours.at(i).getContourLength() >3)
            {
                if (contours.at(i).getCenterOfRegressionLine() > centerHighestContour.getCenterOfRegressionLine()
                               || contours.at(i).getCenterOfRegressionLine() > (30.0+contours.at(i-1).getCenterOfRegressionLine()))
                {
                    isGrowthAtEnd = true;
                    ss<<i<<" big growth "<<contours.at(i).getStartIndex()<<" "<<contours.at(i).getEndIndex();
                    stateChanges.push_back(QString::fromStdString(ss.str()));
                }
            }
            if (contours.at(i).getStartState() == DROP)
            {
                isDropAtEnd = true;
                ss<<i<<" big drop "<<contours.at(i).getStartIndex()<<" "<<contours.at(i).getEndIndex();
                stateChanges.push_back(QString::fromStdString(ss.str()));
            }
        }
        if (contours.at(i).getCenterOfRegressionLine() > highestContourValue)
        {
            highestContourValue = contours.at(i).getCenterOfRegressionLine();
            indexOfHighestValue =i;
        }
        if ((isGrowthAtEnd) && (!isDropAtEnd))
            features |= bigGrowthAtTheEnd;
    }




    if (endHighestContour.getCenterOfRegressionLine() > 1.5*centerHighestContour.getCenterOfRegressionLine())
    {
        features |= endHasContourWithMuchBiggerF0ValueThanCenter;
    }
    if (endHighestContour.getCenterOfRegressionLine() > (40.0+startHighestContour.getCenterOfRegressionLine()))
    {
        features |= endHasContourWithBiggerF0ValueThanStart;
    }




    if (startHighestContour.getCenterOfRegressionLine() > 1.4*centerHighestContour.getCenterOfRegressionLine())
    {
        features |= startHasContourWithMuchBiggerF0ValueThanCenter;
    }
    else if (startHighestContour.getCenterOfRegressionLine() > centerHighestContour.getCenterOfRegressionLine())
    {
        features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
    }

    if (startHighestContour.getCoefA() > 0.5 && startHighestContour.getCoefA() < 1.5 )
        features |= highestContourAtBeginningStronglyRising;



    if (areAllContoursFalling())
    {
        features |= allContoursAreFalling;

     }



    if (centerHighestContour.getCenterOfRegressionLine() > endHighestContour.getCenterOfRegressionLine())
    {
         features |= centerHasContourWithBiggerF0ValueThanEnd;
    }

    if  (features & centerHasContourWithBiggerF0ValueThanEnd)
    {
        double CoefA;
        if ((startHighestContour.getCenterOfRegressionLine() > centerHighestContour.getCenterOfRegressionLine())
            && startHighestContour.getEndIndex() > lastIndexOfBeginningPart)
        {
            CoefA = startHighestContour.getCoefA();

            features |= highestContourLocatedBetweenStartEndCenter;
        }
        else if ((centerHighestContour.getCenterOfRegressionLine() > startHighestContour.getCenterOfRegressionLine())
                   && centerHighestContour.getStartIndex() < lastIndexOfBeginningPart)
         {
            CoefA = centerHighestContour.getCoefA();
            features |= highestContourLocatedBetweenStartEndCenter;
         }
        else if(centerHighestContour.getCenterOfRegressionLine() > startHighestContour.getCenterOfRegressionLine())
        {
              CoefA = centerHighestContour.getCoefA();
              if(centerHighestContour.getImp())
                  features |= impRegion;
        }
        else if(centerHighestContour.getCenterOfRegressionLine() < startHighestContour.getCenterOfRegressionLine())
        {
            if(startHighestContour.getImp())
                features |= impRegion;
        }
        if ((CoefA < -0.4) || (indexHighestValueOfRegresionLinesAtTheCenter > (contours.size()/2)))
             features |= centerHighestContourSteeplyFalling;
        else if (CoefA < 0.0)
             features |= centerHighestContourNotSteeplyFalling;

    }
    std::ostringstream ss;

    ss<<"start "<<startHighestContour.getCenterOfRegressionLine()<<" center "<<centerHighestContour.getCenterOfRegressionLine()
     <<" end "<<endHighestContour.getCenterOfRegressionLine();
    stateChanges.push_back(QString::fromStdString(ss.str()));
    if((indexOfHighestValue == indexHighestValueOfRegresionLinesAtTheCenter)
            && (features & centerHighestContourSteeplyFalling))
        features |= centerContourIsHighestAndSteeplyFalling;   
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
        if (startHighestContour.getContourLength()>10)
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
