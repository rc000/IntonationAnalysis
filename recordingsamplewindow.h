#ifndef RECORDINGSAMPLEWINDOW_H
#define RECORDINGSAMPLEWINDOW_H
/*
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
#include<singlesample.h>
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {class RecordingSampleWindow; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;
QT_END_NAMESPACE

class QAudioLevel;


class RecordingSampleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RecordingSampleWindow(QWidget *parent = 0);
    ~RecordingSampleWindow();
    int getSpeakerNumber();
    int getSampleNumber();
    qreal getPeakValue(const QAudioFormat &format);
    int getNumberOfSpeakers();
    int getNumberOfSamples();
    void addAxis();
    void SetNumberOfSpeakers(int);
    void SetNumberOfSamples(int);
    void SetLabelText();
    void startReckording();
    void extractFeatures(const qint16* frame, qreal peak,int sample_per_frame, int frame_number);
    void removeChartView();
    void calculation();
    void calculate(qint16* frame, int sample_per_frame, int frame_number);

public slots:
    void readBuffer();
    void decodingFinished();
    void getBuffer(QAudioBuffer buffer);
    void on_bRecordBaseSamples_clicked();
    void on_bShowWaveform_clicked();
    void on_bRecordTestSample_clicked();
    void on_bShowSpectrum_clicked();
    void on_bShowEnergy_clicked();
    void on_bPlay_clicked();



private slots:
    void on_bF0_clicked();


private:
    Ui::RecordingSampleWindow *ui;

    int speakerNumber,sampleNumber;
    int numberOfSpeakers,numberOfSamples;
    QString speakerLabel, sampleLabel;
    QAudioRecorder *audioRecorder;
    QAudioProbe *audioProbe;
    bool recorded;
    bool connected;

    QLineSeries *series;
    QChart *chart;
    QChartView *chartView;
    std::vector<SingleSample>samples;
    QSharedPointer<FeaturesExtractor> featuresExtractor;
    int count;
    QString recentlyRecorded;
    double **splited_data;
    int number_splited_frames=8;
    std::vector<QAudioBuffer>audioBuffers;
    QAudioBuffer loadedBuffer;
    QAudioDecoder *audioDecoder;
    qint16 *whole_signal;
    int whole_signal_size;


};
*/
#endif // RECORDINGSAMPLEWINDOW_H
