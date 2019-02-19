#include "classificator.h"
#include <string>
#include<QDebug>
#include <sstream>

/*NOT USED FOR NOW
#define startPartHasContourWithBiggestF0Value (1<<1)
#define centerPartHasContourWithBiggestF0Value (1<<2)
#define endPartHasContourWithBiggestF0Value (1<<3)
#define startAndEndPartsHaveBiggerF0ValueThanCenter (1<<6)
#define endHasContourWithSlightlyBiggerF0ValueThanCenter (1<<7)

*/
#define centerHasContourWithBiggerF0ValueThanStart (1<<1)

#define sentenceHasFallingTendention (1<<2)

#define centerHighestContourNotFalling (1<<3)

#define centerHasContourWithBiggerF0ValueThanEnd (1<<4)

#define highestContourLocatedBetweenStartEndCenter (1<<5)

#define centerHighestContourSteeplyFalling (1<<6)

/* NOT USED FOR NOW
#define allStartContoursAreHigherThanCenterContours (1<<12)
#define allStartContoursAreHigherThanEndContours (1<<13)

#define allCenterContoursAreHigherThanStartContours (1<<14)
#define allCenterContoursAreHigherThanEndContours (1<<15)

#define allEndContoursAreHigherThanStartContours (1<<16)
#define allEndContoursAreHigherThanCenterContours (1<<17)
#define beginningIntonationIsSteeplyFalling (1<<25)
#define beginningIntonationIsNotSteeplyFalling (1<<26)
#define centerIsTheHighest (1<<28)

*/
#define allContoursAreFalling (1<<7)
#define endHasContourWithBiggerF0ValueThanStart (1<<8)

#define endHasContourWithMuchBiggerF0ValueThanCenter (1<<9)

#define startHasContourWithSlightlyBiggerF0ValueThanCenter (1<<10)
#define sentenceHasRisingTendention (1<<11)
#define sentenceHasConstantTendention (1<<12)
#define startHasContourWithMuchBiggerF0ValueThanCenter (1<<13)


#define centerHighestContourNotSteeplyFalling (1<<14)
#define startIsTheLowest (1<<15)
#define highestContourStronglyRising (1<<16)
#define bigGrowthAtTheEnd (1<<17)
#define bigGrowthAtTheBeginning (1<<18)
#define bigDropAtTheBeginning (1<<19)
#define highestContourAtBeginningStronglyRising (1<<20)



static const char *analysisResults[] =
{
    "centerHasContourWithBiggerF0ValueThanStart",
    "sentenceHasFallingTendention",
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
    "startHasContourWithMuchBiggerF0ValueThanCenter",
    "centerHighestContourNotSteeplyFalling",
    "startIsTheLowest",
    "highestContourStronglyRising",
    "bigGrowthAtTheEnd",
    "bigGrowthAtTheBeginning",
    "bigDropAtTheBeginning",
    "highestContourAtBeginningStronglyRising"
};

int numberOfMaxResults = 20;
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
                  | sentenceHasConstantTendention);
                  //| bigDropAtTheBeginning);
   notDeclarative1 |= (endHasContourWithBiggerF0ValueThanStart | highestContourStronglyRising
                   | bigGrowthAtTheBeginning
                       |bigDropAtTheBeginning);


   declarativeIntonationOnCenter |= (centerHasContourWithBiggerF0ValueThanStart
                                     | centerHasContourWithBiggerF0ValueThanEnd
                                     |centerHighestContourSteeplyFalling);

   conclusiveQuestion |=  (endHasContourWithMuchBiggerF0ValueThanCenter
                          | startIsTheLowest
                          | sentenceHasRisingTendention
                          //| highestContourStronglyRising
                          | bigGrowthAtTheEnd);

   completenessQuestion |= (startHasContourWithMuchBiggerF0ValueThanCenter | bigGrowthAtTheBeginning | bigDropAtTheBeginning);
   notCompletenessQuestion1 |= (conclusiveQuestion | sentenceHasConstantTendention);

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
        if(contours.at(i).getWspA()>0.2 && contours.at(i).getContourLength()>1 )
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
    bool endPartIsTheLowest = false;
    highestContourValue = 0.0;
    indexOfHighestValue = 0;
    bool isDropAtEnd = false;
    bool isGrowthAtEnd = false;
    for (int i = 0;i<contours.size();i++)
    {
        std::ostringstream ss;
        ss<<i<<" "<<contours.at(i).getWspA();
        stateChanges.push_back(QString::fromStdString(ss.str()));
        qDebug()<<i<<" kontur "<<contours.at(i).getContourLength();
        if (contours.at(i).getLocationOnTheChart() == BEGINNING && contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheBeginning )
        {
            highestValueOfRegresionLinesAtTheBeginning = contours.at(i).getCenterOfRegressionLine();
            indexHighestValueOfRegresionLinesAtTheBeginning = i;
        }
        if (contours.at(i).getLocationOnTheChart() == CENTER && contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheCenter )
        {
            highestValueOfRegresionLinesAtTheCenter = contours.at(i).getCenterOfRegressionLine();
            indexHighestValueOfRegresionLinesAtTheCenter = i;
        }
        if (contours.at(i).getLocationOnTheChart() == END && contours.at(i).getCenterOfRegressionLine() > highestValueOfRegresionLinesAtTheEnd )
        {
            highestValueOfRegresionLinesAtTheEnd = contours.at(i).getCenterOfRegressionLine();
            indexHighestValueOfRegresionLinesAtTheEnd = i;
        }
        if (contours.at(i).getCenterOfRegressionLine() > highestContourValue)
        {
            highestContourValue = contours.at(i).getCenterOfRegressionLine();
            indexOfHighestValue =i;
        }
        if (contours.at(i).getLocationOnTheChart() == END)
        {
            if (contours.at(i).getStartState() == RISE)
                isGrowthAtEnd = true;
            if (contours.at(i).getStartState() == FALL)
                isDropAtEnd = true;
        }

        if ((isGrowthAtEnd) && (!isDropAtEnd))
            features |= bigGrowthAtTheEnd;

        if (contours.at(i).getStartState()!=0)
        {
            QString state;
            if (contours.at(i).getLocationOnTheChart() == BEGINNING)
            {
                state = "Beginning";
                if (contours.at(i).getStartState() == FALL)
                {
                    if(contours.at(i+1).getWspA()<0.0)
                    {
                        qDebug()<<"DROP "<<i<<" "<<contours.at(i).getStartIndex()<<" "<<contours.at(i).getContourLength();
                        features |= bigDropAtTheBeginning;
                    }

                }
                else
                    features |= bigGrowthAtTheBeginning;

            }
            else if (contours.at(i).getLocationOnTheChart() == CENTER) state = "CENTER";
            else state = "END";

            if (contours.at(i).getStartState() == RISE) state +=" GROWTH";
            else if (contours.at(i).getStartState() == FALL)
            {
                qDebug()<<"DROP "<<i<<" "<<contours.at(i).getStartIndex()<<" "<<contours.at(i).getContourLength();

                state += " DROP";
            }
            stateChanges.push_back(state);
        }


    }
    qDebug()<<"higheest Value "<<highestContourValue<<" index "<<indexOfHighestValue;
    qDebug()<<"highestValueBeginning "<<contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getWspA()
           <<" "<<contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getContourLength()
          <<" "<<indexHighestValueOfRegresionLinesAtTheBeginning;

    if ((contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getWspA() > 0.8)
            && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getContourLength()>5))
        features |= highestContourAtBeginningStronglyRising;

    if (highestValueOfRegresionLinesAtTheEnd > 1.2 *highestValueOfRegresionLinesAtTheCenter)
    {
        features |= endHasContourWithMuchBiggerF0ValueThanCenter;
    }
    /* NOT USED FOR NOW
    else if (highestValueOfRegresionLinesAtTheEnd > 1.1
             *highestValueOfRegresionLinesAtTheCenter)
    {
        qDebug()<<"at the end "<<highestValueOfRegresionLinesAtTheEnd;
        qDebug()<<"center "<<highestValueOfRegresionLinesAtTheCenter;
        features |= endHasContourWithSlightlyBiggerF0ValueThanCenter;
    }*/

    if (highestValueOfRegresionLinesAtTheBeginning > 1.5 *highestValueOfRegresionLinesAtTheCenter)
    {
        if ((contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getCenterOfRegressionLine()
                - contours.at(indexHighestValueOfRegresionLinesAtTheBeginning+1).getCenterOfRegressionLine())
                > 10.0)
        {
            features |= startHasContourWithMuchBiggerF0ValueThanCenter;
         }
        else
        {
            features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
        }

        //return true;
    }
    else if (highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
    {
        features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
        if ((contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getWspA() > 1.5)
                && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getContourLength() > 10)
                && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning+1).getStartIndex() > lastIndexOfBeginningPart))
            features |= highestContourStronglyRising;
         //return true;
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
    /* NOT USED FOR NOW
     *
     if (highestValueOfRegresionLinesAtTheEnd > highestValueOfRegresionLinesAtTheCenter
          && highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
    {
        qDebug()<<"start end";
        features |= startAndEndPartsHaveBiggerF0ValueThanCenter;
    }
    if (compareTwoParts(BEGINNING,CENTER))
    {
       features |= allStartContoursAreHigherThanCenterContours;
    }
    else if (compareTwoParts(CENTER,BEGINNING))
    {
        features |= allCenterContoursAreHigherThanStartContours;
    }

    if (compareTwoParts(BEGINNING,END))
    {
        features |= allStartContoursAreHigherThanEndContours;
    }
    else if (compareTwoParts(END, BEGINNING))
    {
        features |= allEndContoursAreHigherThanStartContours;
    }
    if (compareTwoParts(CENTER,END))
    {
        features |= allCenterContoursAreHigherThanEndContours;
    }
    else if (compareTwoParts(END, CENTER))
    {
        features |= allEndContoursAreHigherThanCenterContours;
    }
    */
    if (areAllContoursFalling())
    {
        features |= allContoursAreFalling;

     }
    int differenceBetweenBeginningCenter = std::abs(highestValueOfRegresionLinesAtTheBeginning
                                                    - highestValueOfRegresionLinesAtTheCenter);
    int differenceBetweenCenterEnd = std::abs(highestValueOfRegresionLinesAtTheCenter
                                              - highestValueOfRegresionLinesAtTheEnd);
    int tenPercentOfAverageValueBegginingCenter = ((highestValueOfRegresionLinesAtTheBeginning
                                                   +highestValueOfRegresionLinesAtTheCenter)/2)/10;

    int tenPercentOfAverageValueCenterEnd = ((highestValueOfRegresionLinesAtTheEnd
                                                   +highestValueOfRegresionLinesAtTheCenter)/2)/10;
    if (highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter
            && highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheEnd)
    {
        features |= sentenceHasFallingTendention;
    }
    else if (highestValueOfRegresionLinesAtTheBeginning < highestValueOfRegresionLinesAtTheCenter
            && highestValueOfRegresionLinesAtTheBeginning < highestValueOfRegresionLinesAtTheEnd
            && differenceBetweenCenterEnd < tenPercentOfAverageValueCenterEnd)
    {
        features |= sentenceHasRisingTendention;
    }
    else if (differenceBetweenBeginningCenter < tenPercentOfAverageValueBegginingCenter
             && differenceBetweenCenterEnd < tenPercentOfAverageValueCenterEnd)
    {
        features |= sentenceHasConstantTendention;
    }

    if ( highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheEnd
         && highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheEnd)
        endPartIsTheLowest = true;

    double wspA;
    if  (features & centerHasContourWithBiggerF0ValueThanEnd)
    {
        if ((highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
                && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getEndIndex() > lastIndexOfBeginningPart))
        {
            wspA = contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getWspA();
            features |= highestContourLocatedBetweenStartEndCenter;
        }
        else if ((highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning)
                 &&  ((contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getStartIndex()) < lastIndexOfBeginningPart))
         {
            wspA = contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getWspA();
            features |= highestContourLocatedBetweenStartEndCenter;
         }
        else if (highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning
                   && highestValueOfRegresionLinesAtTheCenter >highestValueOfRegresionLinesAtTheEnd)
            wspA = contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getWspA();

        if (wspA < 0.0)
        {
            if ((wspA < -0.4) || (indexHighestValueOfRegresionLinesAtTheCenter > (contours.size()/2)))
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

    return false;

}



std::string Classificator::classification()
{

    analysis();
    qDebug()<<bin(features);
    qDebug()<<bin(declarative);

    qDebug()<<bin(conclusiveQuestion);
    qDebug()<<bin(completenessQuestion);
    qDebug()<<bin(imperative);
    QString result;
    if ((features & declarative) > startHasContourWithSlightlyBiggerF0ValueThanCenter
            && (! (features & imperative))
            && (!(features & notDeclarative1)))
        result += "zdanie twierdzace zwykle";

    if (((features & imperative))
            && (!(features & notImperative)))
        result += "rozkazujace";

    if ((indexHighestValueOfRegresionLinesAtTheCenter == indexOfHighestValue) ||
            (features & highestContourLocatedBetweenStartEndCenter))
    {
       // if ((features & completenessQuestionCenterIntonation)  & centerHighestContourNotFalling)
         //   result += "pytanie uzupelnienia  z int na srodek";



        if (((features & declarativeIntonationOnCenter) & centerHighestContourSteeplyFalling)
            && (!(features & notDeclarative1)))
            result += " twierdzenie z intonacja na srodek";

        else if (features & endHasContourWithBiggerF0ValueThanStart)
        {

            if (features & conclusiveQuestion)
                result += "pytanie rozstrzygniecia";
        }

    }

    else {
        if (((features & completenessQuestion) )
                 &&(!(features & notCompletenessQuestion1)))
             result += "pytanie uzupelnienia";
        if (features & conclusiveQuestion)
            result += "pytanie rozstrzygniecia";

    }
    contours.clear();
    return result.toStdString();
}
