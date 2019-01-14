#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QMediaRecorder>
#include<QAudioRecorder>
#include <QAudioDecoder>
#include<QScatterSeries>
#include<QUrl>
#include <QAudioProbe>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include "FeaturesExtractor.h"
#include<singleframefeatures.h>
#include<borderlayout.h>
#include<classificator.h>
#include<contour.h>
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
QT_END_NAMESPACE

class QAudioLevel;

class SingleSegment {
public:
    SingleSegment(){}
    size_t getStart(){return start;}
    size_t getEnd(){return end;}
    size_t getCenter(){return center;}

    size_t getSize(){return values.size();}
    void setStart(size_t start){this->start = start;}
    void setEnd(size_t end){this->end = end;}
    void setCenter(){this->center = start + (end-start)/2;}
    void setLocation(int location){locationOnTheChart = location;}

    int getLocation(){ return locationOnTheChart;}
    double getValue(int i){return values.at(i);}
    void addValue(double value){values.emplace_back(value);}
    void clear(){values.clear();}
    void setIsSpaceBeforeContour(bool isSpace){this->spaceBeforeContour = isSpace;}
    bool IsSpaceBeforeContour(){return spaceBeforeContour;}
    void setPreviousContourOrSpaceLength(size_t length){this->previousContourOrSpaceLength = length;}
    size_t getPreviousContourOrSpaceLength(){return previousContourOrSpaceLength;}
    size_t getSegmentLength(){return end-start;}

    bool isContourValidate()
    {
        for (int i =0;i<values.size();i++)
        {
            if (values.at(i)==0)
                return false;
        }
        return true;
    }

private:
    size_t start;
    size_t center;
    size_t end;
    bool spaceBeforeContour;
    size_t previousContourOrSpaceLength;
    std::vector<double> values;
    int locationOnTheChart;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addAxis();
    void startRecording();
    void extractFeatures(qreal peak,int sample_per_frame, int frame_number);
    void removeChartView();
    void calculation();
    void setEnabledFeatureButtons(bool state);
    void setLayout();
    int findIndexOfLastF0Value();


    void findContours(bool draw);
    void foundNewContour(int i,double &averageValue,int &numberOfPositiveValues);
    void lookForLastContour(double &averageValue, int &numberOfPositiveValues);
    void calcRegressionLines(Classificator &classificator,bool draw);


 public slots:
    void readBuffer();
    void decodingFinished();
    void getBuffer(QAudioBuffer buffer);
    void on_bRecordBaseSamples_clicked();
    void on_bShowWaveform_clicked();
    void on_bShowSpectrum_clicked();
    void on_bShowEnergy_clicked();
    void on_bPlay_clicked();
private slots:
    void on_bF0_clicked();
    void on_bLoad_pressed();

private:
    Ui::MainWindow *ui;
    BorderLayout *layout;

    QWidget *emptyWidget;
    QWidget *centralWidget;

    bool recorded;
    bool connected;
    qint16 **frames;
    int samples_per_frame;
    int frames_number;
    int whole_signal_size;
    int sampleRate;
    qint16 *whole_signal;

    QAudioRecorder *audioRecorder;
    QAudioProbe *audioProbe;
    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    std::vector<SingleFrameFeatures>framesFeatures;
    QSharedPointer<FeaturesExtractor> featuresExtractor;
    std::vector<QAudioBuffer>audioBuffers;
    QAudioDecoder *audioDecoder;
    qreal peak = SHRT_MAX;
    QString wavFilePath;
    std::vector<SingleSegment> SegmentsVector;
    SingleSegment currentSegment;
    QScatterSeries *seriesContours = new QScatterSeries();
    std::vector<QLineSeries*>seriesRegresionLines;

    double maxValue = 0.0;
    double minValue = 400.0;
    int firstValueIndex,lastValueIndex;
    int firstPart = 0;
    int centerPart = 0;
};


#endif // MainWindow_H
