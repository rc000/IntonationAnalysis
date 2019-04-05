#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QMediaRecorder>
#include<QAudioRecorder>
#include <QAudioDecoder>
#include<QScatterSeries>
#include <QtCharts/QLineSeries>

#include<QUrl>
#include <QAudioProbe>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include "extractionHelper.h"
#include <borderlayout.h>
#include <classificator.h>
//#include <contour.h>
#include "contoursdetector.h"

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addAxis();
    void startRecording();
    ExtractionHelper extractFeatures();
    void removeChartView();
    void putValuesIntoVector();
    void setEnabledFeatureButtons(bool state);
    void setLayout();
    void loadWavFile(QString wavFilePath);






 public slots:
    void readBuffer( );
    void decodingFinished();
    void getBuffer(QAudioBuffer buffer);
    void on_bShowWaveform_clicked();

    void on_bPlay_clicked();
    void cellClicked(int nRow, int nCol);
    void cellSelected(int nRow, int nCol);

private slots:
    void on_bLoad_pressed();
    void on_bTestBase_clicked();
    void on_pushButton_clicked();

    void on_bRegression_clicked();

    void on_bPraat_clicked();

private:
    Ui::MainWindow *ui;
    BorderLayout *layout;

    QWidget *emptyWidget;
    QWidget *centralWidget;

    std::vector<double>wholeBuffer;
    std::vector<std::vector<double>> framesVector;
    int frameSize;
    int frames_number;
    int whole_signal_size;
    int sampleRate;
    qint16 *whole_signal;
   // qint16 *whole_signal;

    QAudioProbe *audioProbe;
    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    ExtractionHelper  *extractionHelper;
    int activeColumn = -1;
    std::vector<QAudioBuffer>audioBuffers;
     QAudioDecoder *audioDecoder;
    qreal peak = SHRT_MAX;

    QScatterSeries *seriesContours = new QScatterSeries();
    std::vector<QLineSeries*>seriesRegresionLines;
    int rowCounter = 0;
    QAudioFormat desiredFormat;
    QString wavFilePath;
    std::vector<QString>wavFiles;
    std::vector<ContoursDetector> detectors;
    QStringList wavFilesList;

};


#endif // MainWindow_H
