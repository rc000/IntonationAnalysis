#define REAL 0
#define IMAG 1

#include<fstream>
#include <QScatterSeries>
#include<QFileInfo>
#include<QTime>
#include<QValueAxis>
#include <math.h>
#include <limits>
#include<QSound>
#include "recordingsamplewindow.h"
#include "ui_recordingsamplewindow.h"
/*
RecordingSampleWindow::RecordingSampleWindow(QWidget *parent) :
    QDialog(parent),chartView(NULL),
    recorded(false),connected(false),
     ui(new Ui::RecordingSampleWindow),count(0)
{
        ui->setupUi(this);
        audioRecorder = new QAudioRecorder(this);
        audioProbe = new QAudioProbe(this);

}
RecordingSampleWindow::~RecordingSampleWindow()
{
    delete ui;
}

void RecordingSampleWindow::SetLabelText()
{
    QString label=speakerLabel+QString::number(speakerNumber)+sampleLabel+QString::number(sampleNumber);
    ui->lSpeakerNumber->setText(label);
}




void RecordingSampleWindow::startReckording()
{
    audioBuffers.clear();
    qDebug()<<"Start Recording";
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/wav");
    audioSettings.setSampleRate(44100);
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setBitRate(32000);

    audioRecorder->setAudioSettings(audioSettings);
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(QFileInfo("records/recorded_sentence").absoluteFilePath()));
    if(!connected)
    {
        if (audioProbe->setSource(audioRecorder))
        {
            connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)),
                this, SLOT(getBuffer(QAudioBuffer)));
            connected=true;
        }
    }
    audioRecorder->record();
}

void RecordingSampleWindow::getBuffer(QAudioBuffer buffer)
{
    audioBuffers.emplace_back(buffer);
}
void RecordingSampleWindow::calculation()
{
    int sample_per_frame = audioBuffers[0].format().sampleRate()/20;
    int size = audioBuffers.size()*2;

    qint16 **frames = new qint16 *[size];
    for (int i = 0; i < size; i++)
            frames[i] = new qint16[sample_per_frame];
    for (int i =0; i<sample_per_frame; i++)
            frames[size-1][i] = 0;

    qDebug()<<"calcualtion";

    for(int i=0;i<audioBuffers.size();i++)
    {
         whole_signal_size+=audioBuffers[i].sampleCount();
    }

    whole_signal=new qint16[whole_signal_size];

    int index_frame = 0;
    int index = 0;
    qDebug()<<"przed ifem ";
    for(int i=0;i<audioBuffers.size();i++)
    {
        qDebug()<<"if i "<<i<<" size "<<audioBuffers.size();
        const qint16 *data = audioBuffers[i].constData<qint16>();
        for(int j=0;j<audioBuffers[i].sampleCount();j++)
        {
            index++;
            whole_signal[index]=data[j];
            qreal peak = SHRT_MAX;
            samples[samples.size()-1].buffer_emplace_back(whole_signal[index]/peak);

            if (j<sample_per_frame)
            {
                frames[index_frame][j] = data[j];
            }
            else
            {
                frames[index_frame+1][j-sample_per_frame] = data[j];
            }
        }
        index_frame+=2;

    }
    qDebug()<<"before calculate";

    for(int i=0;i<audioBuffers.size()*2;i++)
    {
        calculate(frames[i], sample_per_frame, i);
    }
 }
void RecordingSampleWindow::calculate(qint16 *frame,int sample_per_frame, int frame_number)
{

    /*qDebug()<<"ilosc sampli "<<buffer.format().sampleRate();
    qDebug()<<"calculate";
    QAudioBuffer::S16S *frames = buffer.data<QAudioBuffer::S16S>();


    qDebug()<<"ramki "<<buffer.frameCount();
    qDebug()<<"sample "<<buffer.sampleCount();
    //removeChartView();

    qDebug()<<" buffer format sampleSize "<<buffer.format().sampleSize();
    qreal peak = getPeakValue(buffer.format());
    qInfo()<<peak;
    buffer.constData<double>();
    const qint16 *data = buffer.constData<qint16>();
    count = buffer.sampleCount();
    qDebug()<<"before extractFeatures";//8
    qreal peak = SHRT_MAX;
    extractFeatures(frame,peak, sample_per_frame, frame_number);
    qDebug()<<"after extractFEatures";


    fftw_complex *x=new fftw_complex [sample_per_frame]; // input
    fftw_complex *y=new fftw_complex [sample_per_frame]; // input

    for(size_t i=0; i<sample_per_frame; i++)
    {
        //samples[samples.size()-1].buffer_emplace_back(frame[i]/peak);

        x[i][REAL] = frame[i]/peak;
        x[i][IMAG] = 0;
    }

    fftw_plan myPlan= fftw_plan_dft_1d(sample_per_frame, x, y, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(myPlan);
    fftw_destroy_plan(myPlan);
    fftw_cleanup();

    for(int i=0; i<sample_per_frame; i++)
    {
      double magnitude= sqrt(y[i][REAL]*y[i][REAL] + y[i][IMAG]*y[i][IMAG]);
      samples[samples.size()-1].fftBuffer_emplace_back(magnitude);
    }
}


void RecordingSampleWindow::extractFeatures(const qint16* data,qreal peak,int sample_per_frame,int frame_number)
{

    FeaturesExtractor  featuresExtractor(data, peak, sample_per_frame);
    samples[samples.size()-1].energy_emplace_back(featuresExtractor.calcEnergy());
    samples[samples.size()-1].zcr_emplace_back(featuresExtractor.calcZCR());
    samples[samples.size()-1].f0_emplace_back(featuresExtractor.calcF0(whole_signal, whole_signal_size, frame_number));
}

qreal RecordingSampleWindow::getPeakValue(const QAudioFormat &format)
{
    qreal ret(0);
    if (format.isValid()){
        switch (format.sampleType()) {
            case QAudioFormat::Unknown:
            break;
            case QAudioFormat::Float:
                if (format.sampleSize() != 32) // other sample formats are not supported
                    ret = 0;
                else
                    ret = 1.00003;
            break;
            case QAudioFormat::SignedInt:
                if (format.sampleSize() == 32)
#ifdef Q_OS_WIN
                    ret = INT_MAX;
#endif
#ifdef Q_OS_UNIX
                    ret = SHRT_MAX;
#endif
                else if (format.sampleSize() == 16)
                    ret = SHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = CHAR_MAX;
                break;
            case QAudioFormat::UnSignedInt:
                if (format.sampleSize() == 32)
                    ret = UINT_MAX;
                else if (format.sampleSize() == 16)
                    ret = USHRT_MAX;
                else if (format.sampleSize() == 8)
                    ret = UCHAR_MAX;
            break;
        default:
            break;
        }
    }
    return ret;
}
void RecordingSampleWindow::removeChartView()
{
     if(chartView!=NULL)
     {
         ui->gridLayout->removeWidget(chartView);
     }
}

void RecordingSampleWindow::on_bRecordBaseSamples_clicked()
{
    qDebug()<<"record";
    if(!recorded)
    {
        SetLabelText();
        if (audioRecorder->state() == QMediaRecorder::StoppedState)
        {
            ui->bRecordBaseSamples->setText("Stop");
            audioRecorder->setAudioInput(audioRecorder->defaultAudioInput());
            samples.emplace_back();
            startReckording();


        }
        else if (audioRecorder->state() == QMediaRecorder::RecordingState)
        {
            audioRecorder->stop();
            calculation();
            ui->bRecordBaseSamples->setText("start");
        }
   }

}

void RecordingSampleWindow::on_bRecordTestSample_clicked()
{
    audioBuffers.clear();
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(1);
    desiredFormat.setCodec("audio/wav");
    desiredFormat.setSampleType(QAudioFormat::UnSignedInt);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);

    audioDecoder = new QAudioDecoder();
    audioDecoder->setAudioFormat(desiredFormat);

    audioDecoder->setSourceFilename("C:\\Users\\Rafau\\Downloads\\31.07\\31.07\\build-SpeakerRecognizer-Desktop_Qt_5_9_6_MSVC2017_64bit-Debug\\records\\kiedy.wav");
    connect(audioDecoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(audioDecoder,SIGNAL(finished()),this,SLOT(decodingFinished()));
    audioDecoder->start();
}

void RecordingSampleWindow::decodingFinished()
{
    samples.emplace_back(SingleSample());
    qDebug()<<"decoding finished";
    calculation();
}
void RecordingSampleWindow::on_bShowWaveform_clicked()
{
   removeChartView();
   series = new QLineSeries();
   chart = new QChart();
    qDebug()<<"size";
    qDebug()<<samples[samples.size()-1].buffer_size();
   for(size_t i=0;i<samples[samples.size()-1].buffer_size();i++)
           {
               series->append(i/44,samples[samples.size()-1].buffer_value(i));
           }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();

  chart->setTitle("Simple line chart example");
  chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  ui->gridLayout->addWidget(chartView);
}

void RecordingSampleWindow::on_bShowSpectrum_clicked()
{
   removeChartView();
   series = new QLineSeries();
   chart = new QChart();
   for(size_t i=0;i<samples[samples.size()-1].fftBuffer_size();i++)
           {
               series->append(i,samples[samples.size()-1].fftBuffer_value(i));
           }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();

  chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  ui->gridLayout->addWidget(chartView);
}

void RecordingSampleWindow::on_bShowEnergy_clicked()
{
   removeChartView();
   series = new QLineSeries();
   chart = new QChart();

   for(size_t i=0;i<samples[samples.size()-1].energy_size();i++)
           {
               series->append(i,samples[samples.size()-1].energy_value(i));
           }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();

  chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  ui->gridLayout->addWidget(chartView);
}

void RecordingSampleWindow::addAxis()
{
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(10);
    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(10);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}
void RecordingSampleWindow::on_bPlay_clicked()
{
    qDebug()<<recentlyRecorded;
    QSound::play("records/"+recentlyRecorded+".wav");

}

void RecordingSampleWindow::readBuffer()
{

    audioBuffers.emplace_back(audioDecoder->read());
    qDebug()<<"loadedBuffer.format().sampleRate() "<<audioDecoder->audioFormat().sampleRate();
   // calculate(loadedBuffer);
}


void RecordingSampleWindow::on_bF0_clicked()
{
   removeChartView();
   series = new QLineSeries();
   chart = new QChart();

   QScatterSeries *series0 = new QScatterSeries();
   series0->setName("scatter1");
   series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
   series0->setMarkerSize(8.0);

   double down=0.0;
   double up=0.0;
   double prev_value=-1;
   qDebug()<<"samples[samples.size()-1].f0_size() "<<samples[samples.size()-1].f0_size();

   for(size_t i=1;i<samples[samples.size()-1].f0_size();i++)
           {
              double value =samples[samples.size()-1].f0_value(i);

                        if(prev_value==-1)
                            prev_value=value;
                        series0->append(i,value);
                        if(value>prev_value)
                        {
                            up+=value-prev_value;
                           // qDebug()<<"up "<<i<<" c "<<value <<" p  "<<prev_value<<" r "<<value-prev_value;
                        }
                        if(value<prev_value)
                        {
                            down-=prev_value-value;
                         //   qDebug()<<"down "<<i<<" c "<<value <<" p  "<<prev_value<<" r "<<prev_value-value;
                        }
                        prev_value=value;


           }

 /*  qDebug()<<"up ";
   qDebug()<<up;
   qDebug()<<"down";
   qDebug()<<down;//
  chart->addSeries(series0);
  chart->legend()->hide();
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(20);
  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(20);
  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);

  series0->attachAxis(axisX);
  series0->attachAxis(axisY);

  chartView = new QChartView(chart);
  //chartView->setRenderHint(QPainter::Antialiasing);
  ui->gridLayout->addWidget(chartView);
}

*/
