#include "classificator.h"
#include <string>
#include<QDebug>

#define startPartHasContourWithBiggestF0Value (1<<1)
#define centerPartHasContourWithBiggestF0Value (1<<2)
#define endPartHasContourWithBiggestF0Value (1<<3)
#define centerHasContourWithBiggerF0ValueThanStart (1<<4)

#define sentenceHasFallingTendention (1<<5)

#define startAndEndPartsHaveBiggerF0ValueThanCenter (1<<6)

#define endHasContourWithSlightlyBiggerF0ValueThanCenter (1<<7)


#define centerHighestContourNotFalling (1<<8)

#define centerHasContourWithBiggerF0ValueThanEnd (1<<9)

#define highestContourLocatedBetweenStartEndCenter (1<<10)

#define centerHighestContourSteeplyFalling (1<<11)

#define allStartContoursAreHigherThanCenterContours (1<<12)
#define allStartContoursAreHigherThanEndContours (1<<13)

#define allCenterContoursAreHigherThanStartContours (1<<14)
#define allCenterContoursAreHigherThanEndContours (1<<15)

#define allEndContoursAreHigherThanStartContours (1<<16)
#define allEndContoursAreHigherThanCenterContours (1<<17)

#define allContoursAreFalling (1<<18)
#define endHasContourWithBiggerF0ValueThanStart (1<<19)

#define endHasContourWithMuchBiggerF0ValueThanCenter (1<<20)

#define startHasContourWithSlightlyBiggerF0ValueThanCenter (1<<21)
#define sentenceHasRisingTendention (1<<22)
#define sentenceHasConstantTendention (1<<23)
#define startHasContourWithMuchBiggerF0ValueThanCenter (1<<24)

#define beginningIntonationIsSteeplyFalling (1<<25)
#define beginningIntonationIsNotSteeplyFalling (1<<26)
#define centerHighestContourNotSteeplyFalling (1<<27)



int declarative = 0;
int declarativeIntonationOnCenter = 0;
int conclusiveQuestion = 0;
int completenessQuestion = 0;
int imperative = 0;


void initialization()
{
   declarative|= (startHasContourWithSlightlyBiggerF0ValueThanCenter | allContoursAreFalling
                  | sentenceHasFallingTendention
                  | sentenceHasConstantTendention);
   declarativeIntonationOnCenter |= (centerHasContourWithBiggerF0ValueThanStart
                                     | centerHasContourWithBiggerF0ValueThanEnd
                                     |centerHighestContourSteeplyFalling);

   conclusiveQuestion |=  (endHasContourWithMuchBiggerF0ValueThanCenter
                          | startAndEndPartsHaveBiggerF0ValueThanCenter
                          | sentenceHasRisingTendention
                          | endHasContourWithSlightlyBiggerF0ValueThanCenter);

   completenessQuestion |= (startHasContourWithMuchBiggerF0ValueThanCenter
                           |centerHighestContourNotFalling
                           |centerHasContourWithBiggerF0ValueThanStart
                           |highestContourLocatedBetweenStartEndCenter );

   imperative |= highestContourLocatedBetweenStartEndCenter;

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

Classificator::Classificator(int lastIndexOfBeginningPart,int lastIndexOfCenterPart,bool sex)
    :lastIndexOfBeginningPart(lastIndexOfBeginningPart),lastIndexOfCenterPart(lastIndexOfCenterPart),sex(sex)
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
                    qDebug()<<"second "<<contours.at(j).getCenterOfRegressionLine();
                    qDebug()<<"first "<<contours.at(i).getCenterOfRegressionLine();
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
        if(contours.at(i).getWspA()>0.2 && contours.at(0).getContourLength()>1 )
        {
            qDebug()<<"if(contours.at("<<i<<").getContourState() && "<<contours.at(i).getContourLength();
            return false;
         }
    }
     qDebug()<<"allAreFalling "<<(contours.at(0).getWspA())<<" "<<contours.at(0).getContourLength()
               <<" "<<contours.at(0).getStartState()
               <<" "<<contours.at(0).getContourState();
    return true;
}
bool Classificator::isQuestion()
{
    bool endPartIsTheLowest = false;
    for (int i = 0;i<contours.size();i++)
    {
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
    }


    if (highestValueOfRegresionLinesAtTheEnd > 1.5 *highestValueOfRegresionLinesAtTheCenter)
    {
        features |= endHasContourWithMuchBiggerF0ValueThanCenter;
        qDebug()<<"Pytanie koniec";
        //return true;
    }
    else if (highestValueOfRegresionLinesAtTheEnd > 1.1
             *highestValueOfRegresionLinesAtTheCenter)
    {
        qDebug()<<"at the end "<<highestValueOfRegresionLinesAtTheEnd;
        qDebug()<<"center "<<highestValueOfRegresionLinesAtTheCenter;
        features |= endHasContourWithSlightlyBiggerF0ValueThanCenter;
    }

    if (highestValueOfRegresionLinesAtTheBeginning > 1.5 *highestValueOfRegresionLinesAtTheCenter)
    {
        //features |= startHasContourWithMuchBiggerF0ValueThanCenter;
        if ((contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getCenterOfRegressionLine()
                - contours.at(indexHighestValueOfRegresionLinesAtTheBeginning+1).getCenterOfRegressionLine())
                > 10.0)
        {
            features |= startHasContourWithMuchBiggerF0ValueThanCenter;
            qDebug()<<"MUCH BIGGER "<<contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getCenter()
                   <<" "<<contours.at(indexHighestValueOfRegresionLinesAtTheBeginning+1).getCenter();
        }
        else
        {
            features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
        qDebug()<<"NOT MUCH BIGGER";
        }

        //return true;
    }
    else if (highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
    {
        features |= startHasContourWithSlightlyBiggerF0ValueThanCenter;
        qDebug()<<"Zdanie Twierdzace start";
         //return true;
    }
    if (highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheBeginning)
    {
        qDebug()<<"CENTER BLABLA "<<indexHighestValueOfRegresionLinesAtTheCenter<<" "<< contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getCenterOfRegressionLine();
         features |= centerHasContourWithBiggerF0ValueThanStart;
    }
    if (highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheEnd)
    {
         features |= centerHasContourWithBiggerF0ValueThanEnd;
    }
    if (highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning
            && highestValueOfRegresionLinesAtTheCenter >highestValueOfRegresionLinesAtTheEnd)
    {
        double wspA = contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getWspA();
        if (wspA < 0.0)
        {
            if (wspA < -0.5)
                features |= centerHighestContourSteeplyFalling;
            else
                features |= centerHighestContourNotSteeplyFalling;
        }
        else
            features |= centerHighestContourNotFalling;

    }

    if (highestValueOfRegresionLinesAtTheEnd > highestValueOfRegresionLinesAtTheCenter
            && highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
    {
        qDebug()<<"start end";
        features |= startAndEndPartsHaveBiggerF0ValueThanCenter;
    }

    if (highestValueOfRegresionLinesAtTheEnd > 1.2 *highestValueOfRegresionLinesAtTheBeginning)
    {
        features |= endHasContourWithBiggerF0ValueThanStart;
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

    if (areAllContoursFalling())
    {
        features |= allContoursAreFalling;
        qDebug()<<"Zdanie Twierdzace allAreFalling";

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
            && differenceBetweenCenterEnd <tenPercentOfAverageValueCenterEnd)
    {
        features |= sentenceHasRisingTendention;
    }
    else if (differenceBetweenBeginningCenter < tenPercentOfAverageValueBegginingCenter
             && differenceBetweenCenterEnd < tenPercentOfAverageValueCenterEnd)
    {
        qDebug()<<"stala tendencja xD";
        qDebug()<<differenceBetweenBeginningCenter;

        features |= sentenceHasConstantTendention;

    }

    if ( highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheEnd
         && highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheEnd)
        endPartIsTheLowest = true;

    if ( (features & centerHasContourWithBiggerF0ValueThanEnd)
          && (((highestValueOfRegresionLinesAtTheBeginning > highestValueOfRegresionLinesAtTheCenter)
            && (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getEndIndex() > lastIndexOfBeginningPart))
            || ((highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning)
            &&  (contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getStartIndex()) < lastIndexOfBeginningPart)))
    {
        qDebug()<<"rozkaz "<<contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getStartIndex()
               <<" "<<lastIndexOfBeginningPart;
        qDebug()<<highestValueOfRegresionLinesAtTheBeginning<<" "<<highestValueOfRegresionLinesAtTheCenter;

        features |= highestContourLocatedBetweenStartEndCenter;
    }
  /*  if (highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheBeginning
            && highestValueOfRegresionLinesAtTheCenter >  highestValueOfRegresionLinesAtTheEnd
            && contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getWspA() > -1.0)
    {
            qDebug()<<"Pytanie srodek";
            return true;
    }
    if (highestValueOfRegresionLinesAtTheEnd > 1.1 *highestValueOfRegresionLinesAtTheCenter
            && highestValueOfRegresionLinesAtTheBeginning > 1.1 *highestValueOfRegresionLinesAtTheCenter )
    {
        qDebug()<<"Pytanie poczatek i koniec";
        return true;
    }
    if (highestValueOfRegresionLinesAtTheEnd > 1.2 *highestValueOfRegresionLinesAtTheBeginning
            && highestValueOfRegresionLinesAtTheCenter > 1.2 *highestValueOfRegresionLinesAtTheBeginning )
    {
         qDebug()<<"Pytanie srodek i koniec";
        return true;
    }
    qDebug()<<"Nie wiem "<<highestValueOfRegresionLinesAtTheBeginning<<" "<<highestValueOfRegresionLinesAtTheCenter<<
              " "<<highestValueOfRegresionLinesAtTheEnd;
              */
    return false;

}
bool Classificator::isAffirmative()
{

        //return true;

 /*   if (highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheBeginning
            && highestValueOfRegresionLinesAtTheCenter > highestValueOfRegresionLinesAtTheEnd
            && contours.at(indexHighestValueOfRegresionLinesAtTheCenter).getWspA() < -1.0)
            return true;
*/
    return false;
}



std::string Classificator::notAllContoursAreFalling()
{
    return "notAllContoursAreFalling";
}

std::string Classificator::classification()
{

    isQuestion();
    qDebug()<<bin(features);
    qDebug()<<bin(declarative);

    qDebug()<<bin(conclusiveQuestion);
    qDebug()<<bin(completenessQuestion);
    qDebug()<<bin(imperative);
    QString result;
    if ((features & declarative) > startHasContourWithSlightlyBiggerF0ValueThanCenter
            && (! (features & imperative)))
        result += "twierdzenie zwykle";
    if ((features & declarativeIntonationOnCenter) == declarativeIntonationOnCenter)
        result += " twierdzenie z intonacja na srodek";
    if ((features & conclusiveQuestion) >= endHasContourWithSlightlyBiggerF0ValueThanCenter)
        result += "pytanie rozstrzygniecia";
    if (((features & completenessQuestion) )
            &&(!(features & (conclusiveQuestion | sentenceHasConstantTendention))))
                            //| endHasContourWithBiggerF0ValueThanStart) == 0)))
        result += "pytanie uzupelnienia";
    if ((features & imperative) && (features & centerHighestContourNotSteeplyFalling))
        result += "zdanie rozkazujace";


    return result.toStdString();


   /* std::string sex;
    (this->sex == WOMAN)?sex="głos kobiecy ":sex="głos męski ";

    if (!areAllContoursFalling())
    {
        if (isQuestion())
        {
            if (compareTwoParts(BEGINNING,CENTER))
            {
                //if (classification & endHasContourWithBiggerF0ValueThanCenter)
                if (highestValueOfRegresionLinesAtTheEnd > highestValueOfRegresionLinesAtTheCenter)
                {
                     if (compareTwoParts(END,CENTER))
                     {
                         if (contours.at(indexHighestValueOfRegresionLinesAtTheEnd).getContourState() == FALLING)
                             return sex +"100% pytanie rozstrzygniecia, wszystkie warunki spelnione";
                         return sex +"Pytanie rozstrzygniecia, wiekszosc warunkow spelniona";
                     }
                    return sex +"Prawdopodobnie pytanie rozstrzygniecia";
                }
                if (contours.at(indexHighestValueOfRegresionLinesAtTheBeginning).getContourState() == RISING)
                return sex +"pytanie uzupelnienia, wszystkie warunki spelnione";
            }
            else if (compareTwoParts(END,BEGINNING))
                return sex +"Prawdopodobnie pytanie rozstrzygniecia";

            return sex +"prawdopodobnie pytanie uzupelnienia";
        }
    }
     if (isAffirmative())
     {
         if (compareTwoParts(BEGINNING,CENTER))
         {
             if (compareTwoParts(CENTER,END))
                 return sex +"Na 100% zdanie twierdzace";
             return sex +"Prawdopodobnie zdanie twierdzace";
         }
         if (areAllContoursFalling())
             return sex +"Na 100% zdanie twierdzace";

         return sex +"To moze byc zdanie twierdzace";
       }

      return sex +"nie wiem";*/
}
