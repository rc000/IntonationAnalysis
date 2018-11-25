#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QMediaRecorder>
#include<QAudioRecorder>
#include <QAudioDecoder>
#include<QUrl>
#include <QAudioProbe>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include "FeaturesExtractor.h"
#include<singlesamplefeatures.h>
#include<borderlayout.h>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
QT_END_NAMESPACE

class QAudioLevel;


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
    std::vector<SingleSampleFeatures>samples;
    QSharedPointer<FeaturesExtractor> featuresExtractor;
    std::vector<QAudioBuffer>audioBuffers;
    QAudioDecoder *audioDecoder;
    qreal peak = SHRT_MAX;
    QString wavFilePath;

};

#endif // MainWindow_H
