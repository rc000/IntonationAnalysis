#include "classificator.h"
#include <string>
#include<QDebug>
#include <sstream>


#define centerHasContourWithBiggerF0ValueThanStart (1<<1)

#define sentenceHasFallingTendention (1<<2)

#define centerHighestContourNotFalling (1<<3)

#define centerHasContourWithBiggerF0ValueThanEnd (1<<4)

#define highestContourLocatedBetweenStartEndCenter (1<<5)

#define centerHighestContourSteeplyFalling (1<<6)


#define allContoursAreFalling (1<<7)
#define endHasContourWithBiggerF0ValueThanStart (1<<8)

#define endHasContourWithMuchBiggerF0ValueThanCenter (1<<9)

#define startHasContourWithSlightlyBiggerF0ValueThanCenter (1<<10)
#define sentenceHasRisingTendention (1<<11)
#define sentenceHasConstantTendention (1<<12)
#define startHasContourWithMuchBiggerF0ValueThanCenter (1<<18)


#define centerHighestContourNotSteeplyFalling (1<<14)
#define startIsTheLowest (1<<15)

#define bigGrowthAtTheEnd (1<<17)
#define bigGrowthAtTheBeginning (1<<13)
#define bigDropAtTheBeginning (1<<19)

#define highestContourAtBeginningStronglyRising (1<<20)

#define centerContourIsHighestAndSteeplyFalling (1<<21)


static const char *analysisResults[] =
{
    "centerHasContourWithBiggerF0ValueThanStart",
    "sentenceHasDROPingTendention",
    "centerHighestContourNotFalling",
    "centerHasContourWithBiggerF0ValueThanEnd",
    "highestContourLocatedBetweenStartEndCenter",
    "centerHighestContourSteeplyFalling",
    "allContoursAreFalling",
    "endHasContourWithBiggerF0ValueThanStart",
    "endHasContourWithMuchBiggerF0ValueThanCenter",
    "startHasContourWithSlightlyBiggerF0ValueThanCenter",
    "sentenceHasRisingTendention",
    "sentenceHasConstantTendention",
    "bigGrowthAtTheBeginning",
    "centerHighestContourNotSteeplyFalling",
    "startIsTheLowest",
    "highestContourStronglyRising",
    "bigGrowthAtTheEnd",
    "startHasContourWithMuchBiggerF0ValueThanCenter",
    "bigDropAtTheBeginning",
    "highestContourAtBeginningStronglyRising",
    "centerContourIsHighestAndSteeplyFalling"
};

int numberOfMaxResults = 21;
int declarative = 0;
int declarativeIntonationOnCenter = 0;
int conclusiveQuestion = 0;
int completenessQuestion = 0;
int notCompletenessQuestion1 = 0;
int completenessQuestionCenterIntonation = 0;
int imperative = 0;
int notImperative = 0;

int notDeclarative1 = 0;


double highestContourValue = 0.0;
int indexOfHighestValue = 0;

void initialization()
{
   declarative|= (startHasContourWithSlightlyBiggerF0ValueThanCenter | allContoursAreFalling
                  | sentenceHasFallingTendention
                  | sentenceHasConstantTendention
                  |centerContourIsHighestAndSteeplyFalling);
                  //| bigDropAtTheBeginning);
   notDeclarative1 |= (endHasContourWithBiggerF0ValueThanStart //| highestContourStronglyRising
                   //| bigGrowthAtTheBeginning
                       |endHasContourWithMuchBiggerF0ValueThanCenter
                       |bigGrowthAtTheEnd
                       |highestContourLocatedBetweenStartEndCenter);
                   //    |bigDropAtTheBeginning);


   declarativeIntonationOnCenter |= (centerHasContourWithBiggerF0ValueThanStart
                                     | centerHasContourWithBiggerF0ValueThanEnd
                                     |centerHighestContourSteeplyFalling);

   conclusiveQuestion |=  (endHasContourWithMuchBiggerF0ValueThanCenter
                          //| startIsTheLowest
                          | sentenceHasRisingTendention
                          | endHasContourWithBiggerF0ValueThanStart
                          //| highestContourStronglyRising
                          | bigGrowthAtTheEnd);

   completenessQuestion |= (startHasContourWithMuchBiggerF0ValueThanCenter);// | bigGrowthAtTheBeginning | bigDropAtTheBeginning);
   notCompletenessQuestion1 |= (allContoursAreFalling);

   completenessQuestionCenterIntonation |= (centerHighestContourNotFalling
                                        |centerHasContourWithBiggerF0ValueThanStart
                                        |highestContourLocatedBetweenStartEndCenter );


   imperative |= (highestContourLocatedBetweenStartEndCenter
                  |centerHighestContourNotSteeplyFalling
                  |highestContourAtBeginningStronglyRising);
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
bool Classificator::compareTwoParts(int firstPartToCompare, int secondPartToCompare)
{
    for (int i=0;i<contours.size();i++)
    {
        if (contours.at(i).getLocationOnTheChart()==firstPartToCompare)
        {
            for (int j=0;j<contours.size();j++)
            {
                if (contours.at(j).getLocationOnTheChart()==secondPartToCompare
                        && (contours.at(j).getCenterOfRegressionLine() - contours.at(i).getCenterOfRegressionLine()) >5.0)
                {
                    return false;
                }
            }
        }
        continue;
    }
    return true;
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
bool Classificator::analysis()
{
    highestContourValue = 0.0;
    indexOfHighestValue = 0;
    bool isDropAtEnd = false;
    bool isGrowthAtEnd = false;
    for (int i = 0;i<contours.size();i++)
    {
        std::ostringstream ss;
        ss<<i<<" "<<contours.at(i).getCoefA();
        stateChanges.push_back(QString::fromStdString(ss.str()));
        qDebug()<<i<<" kontur "<<contours.at(i).getContourLength();
        if (contours.at(i).getLocationOnTheChart() == BEGINNING)
        {
            if(contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheBeginning)
            {
                highestValueOfRegresionLinesAtTheBeginning = contours.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheBeginning = i;
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
            }
        }
        if (contours.at(i).getLocationOnTheChart() == END)
        {
            if (contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheEnd)
            {
                highestValueOfRegresionLinesAtTheEnd = contours.at(i).getCenterOfRegressionLine();
                indexHighestValueOfRegresionLinesAtTheEnd = i;
            }
            if (contours.at(i).getStartState() == GROWTH)
                isGrowthAtEnd = true;
            if (contours.at(i).getStartState() == DROP)
                isDropAtEnd = true;
        }
        if (contours.at(i).getCenterOfRegressionLine() > highestContourValue)
        {
            highestContourValue = contours.at(i).getCenterOfRegressionLine();
            indexOfHighestValue =i;
        }

        if ((isGrowthAtEnd) && (!isDropAtEnd))
            features |= bigGrowthAtTheEnd;
    }


    if ((contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getCoefA() > 0.5))
        features |= highestContourAtBeginningStronglyRising;

    if (highestValueOfRegresionLinesAtTheEnd > 1.5*highestValueOfRegresionLinesAtTheCenter)
    {
        features |= endHasContourWithMuchBiggerF0ValueThanCenter;
    }

    if (highestValueOfRegresionLinesAtTheBeginning > 1.5 *highestValueOfRegresionLinesAtTheCenter)
    {
        features |= startHasContourWithMuchBiggerF0ValueThanCenter;
    }
    else if (highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
    {
        features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
    }


    if (highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheBeginning)
    {
         features |= centerHasContourWithBiggerF0ValueThanStart;
    }
    if (highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheEnd)
    {
         features |= centerHasContourWithBiggerF0ValueThanEnd;
    }

    if (highestValueOfRegresionLinesAtTheEnd > 1.1*highestValueOfRegresionLinesAtTheBeginning)
    {
        features |= endHasContourWithBiggerF0ValueThanStart;
    }

    if (areAllContoursFalling())
    {
        features |= allContoursAreFalling;

     }



    double CoefA;
    if  (features & centerHasContourWithBiggerF0ValueThanEnd)
    {
        if ((highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
                && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getEndIndex() > lastIndexOfBeginningPart))
        {
            CoefA = contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getCoefA();
            features |= highestContourLocatedBetweenStartEndCenter;
        }
        else if ((highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning)
                 &&  ((contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getStartIndex()) < lastIndexOfBeginningPart))
         {
            CoefA = contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getCoefA();
            features |= highestContourLocatedBetweenStartEndCenter;
         }
        else if (highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning
                   && highestValueOfRegresionLinesAtTheCenter >highestValueOfRegresionLinesAtTheEnd)
            CoefA = contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getCoefA();

        if (CoefA < 0.0)
        {
            if ((CoefA < -0.4) || (indexHighestValueOfRegresionLinesAtTheCenter > (contours.size()/2)))
                features |= centerHighestContourSteeplyFalling;
            else
                features |= centerHighestContourNotSteeplyFalling;
        }
        else
            features |= centerHighestContourNotFalling;

    }


    if ((features & centerHasContourWithBiggerF0ValueThanStart )
           && (features & endHasContourWithBiggerF0ValueThanStart) )
    {
        features |= startIsTheLowest;
    }

    if((indexOfHighestValue == indexHighestValueOfRegresionLinesAtTheCenter)
            && (features & centerHighestContourSteeplyFalling))
        features |= centerContourIsHighestAndSteeplyFalling;
    return false;

}



std::vector<QString> Classificator::classification()
{

    int flag =0;
    analysis();
    qDebug()<<bin(features);
    qDebug()<<bin(declarative);

    qDebug()<<bin(conclusiveQuestion);
    qDebug()<<bin(completenessQuestion);
    qDebug()<<bin(imperative);
    std::vector<QString> result;

    if ((features & declarative) >= startHasContourWithSlightlyBiggerF0ValueThanCenter
           // && (! (features & imperative)))
            && (!(features & notDeclarative1)))
    {
        flag |=1;
        result.emplace_back("zdanie twierdzace zwykle");
    }

    if (((features & completenessQuestion)>bigGrowthAtTheBeginning))
            // &&(!(features & notCompletenessQuestion1)))
         result.emplace_back("pytanie uzupelnienia");
    if (features & conclusiveQuestion)
        result.emplace_back("pytanie rozstrzygniecia");
    if (((features & imperative))
            && (!(features & notImperative)))
    {
        flag |= 2;
        result.emplace_back("rozkazujace");
    }

    if ((flag & 1)&&(flag & 2))
    {
        if (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getContourLength()>10)
            result.erase(result.begin()+0);
        else {
            result.erase(result.begin()+1);
        }
    }

    if(result.size()==0)
        result.emplace_back("unrecognized");
    return result;
}
