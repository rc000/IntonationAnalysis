


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<fstream>
#include<QFileInfo>
#include<QTime>
#include<QValueAxis>
#include<math.h>
#include<limits>
#include<QSound>
#include<QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),chartView(NULL),
    recorded(false),connected(false),frames_number(0),samples_per_frame(0),
    frames(nullptr),ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        audioRecorder = new QAudioRecorder(this);
        audioProbe = new QAudioProbe(this);
        emptyWidget = new QWidget;
        layout = new BorderLayout;
        layout->addWidget(ui->leftButtons, BorderLayout::West);
        layout->addWidget(ui->wBottom, BorderLayout::South);
        centralWidget = new QWidget;
        centralWidget ->setLayout(layout);
        setCentralWidget(centralWidget);
        setEnabledFeatureButtons(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setEnabledFeatureButtons(bool state)
{
        ui->bF0->setEnabled(state);
        ui->bShowEnergy->setEnabled(state);
        ui->bShowSpectrum->setEnabled(state);
        ui->bShowWaveform->setEnabled(state);
        ui->bPlay->setEnabled(state);
}
void MainWindow::startRecording()
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
void MainWindow::getBuffer(QAudioBuffer buffer)
{
    audioBuffers.emplace_back(buffer);
}
void MainWindow::calculation()
{
    for (int i = 0;i<frames_number;i++)
    {
        delete frames[i];
    }
    if(frames!=nullptr)
        delete frames;

    sampleRate = audioBuffers[0].format().sampleRate();
    samples_per_frame = audioBuffers[0].format().sampleRate()/20;
    frames_number = audioBuffers.size()*2;
    frames = new qint16 *[frames_number];
    for (int i = 0; i < frames_number; i++)
            frames[i] = new qint16[samples_per_frame];
    for (int i =0; i<samples_per_frame; i++)
            frames[frames_number-1][i] = 0;

    for(int i=0;i<audioBuffers.size();i++)
    {
         whole_signal_size+=audioBuffers[i].sampleCount();
    }

    whole_signal=new qint16[whole_signal_size];

    int index_frame = 0;
    int index = 0;
    for(int i=0;i<audioBuffers.size();i++)
    {
        const qint16 *data = audioBuffers[i].constData<qint16>();
        for(int j=0;j<audioBuffers[i].sampleCount();j++)
        {
            index++;
            whole_signal[index]=data[j];
            qreal peak = SHRT_MAX;
            framesFeatures[framesFeatures.size()-1].buffer_emplace_back(whole_signal[index]/peak);

            if (j<samples_per_frame)
            {
                frames[index_frame][j] = data[j];
            }
            else
            {
                frames[index_frame+1][j-samples_per_frame] = data[j];
            }
        }
        index_frame+=2;
        //delete data;
    }

    delete FeaturesExtractor::whole_signal;
    FeaturesExtractor::whole_signal = whole_signal;
    FeaturesExtractor::whole_signal_size = whole_signal_size;
    for(int i=0;i<audioBuffers.size()*2;i++)
    {
        extractFeatures(peak,samples_per_frame,i);
    }
 }

void MainWindow::extractFeatures(qreal peak,int sample_per_frame,int frame_number)
{
    FeaturesExtractor  featuresExtractor(frames[frame_number], peak, sample_per_frame,sampleRate);
    /*framesFeatures[framesFeatures.size()-1].energy_emplace_back(featuresExtractor.calcEnergy());
    framesFeatures[framesFeatures.size()-1].zcr_emplace_back(featuresExtractor.calcZCR());*/
    framesFeatures[framesFeatures.size()-1].f0_emplace_back(featuresExtractor.calcF0(frame_number));
    //framesFeatures[framesFeatures.size()-1].fftBuffer_emplace_back(featuresExtractor.calcFFT());
}
void MainWindow::on_bRecordBaseSamples_clicked()
{
    qDebug()<<"record";
    if(!recorded)
    {
        if (audioRecorder->state() == QMediaRecorder::StoppedState)
        {
            ui->bRecordBaseSamples->setText("Stop");
            audioRecorder->setAudioInput(audioRecorder->defaultAudioInput());
            framesFeatures.emplace_back();
            startRecording();
        }
        else if (audioRecorder->state() == QMediaRecorder::RecordingState)
        {
            audioRecorder->stop();
            calculation();
            ui->bRecordBaseSamples->setText("start");
        }
   }
}
void MainWindow::decodingFinished()
{
    framesFeatures.emplace_back(SingleFrameFeatures());
    calculation();
    setEnabledFeatureButtons(true);
    qDebug()<<"decoding finished";
}
void MainWindow::on_bShowWaveform_clicked()
{
   series = new QLineSeries();
   chart = new QChart();
   for(size_t i=0;i<framesFeatures[framesFeatures.size()-1].buffer_size();i++)
   {
       series->append(i/44,framesFeatures[framesFeatures.size()-1].buffer_value(i));
   }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();
  setLayout();

}

void MainWindow::on_bShowSpectrum_clicked()
{
   series = new QLineSeries();
   chart = new QChart();
   for(size_t i=0;i<framesFeatures[framesFeatures.size()-1].fftBuffer_size();i++)
   {
       series->append(i,framesFeatures[framesFeatures.size()-1].fftBuffer_value(i));
   }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();
  setLayout();
}

void MainWindow::on_bShowEnergy_clicked()
{
   series = new QLineSeries();
   chart = new QChart();
   for(size_t i=0;i<framesFeatures[framesFeatures.size()-1].energy_size();i++)
   {
       series->append(i,framesFeatures[framesFeatures.size()-1].energy_value(i));
   }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();
  setLayout();
}
void MainWindow::setLayout()
{
    chartView = new QChartView(chart);
    layout->addWidget(chartView, BorderLayout::Center);
    centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
void MainWindow::addAxis()
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
void MainWindow::on_bPlay_clicked()
{
    qDebug()<<wavFilePath;
    QSound::play(wavFilePath);

}
void MainWindow::readBuffer()
{
    audioBuffers.emplace_back(audioDecoder->read());
}

void MainWindow::on_bF0_clicked()
{

   chart = new QChart();

   seriesContours->setMarkerShape(QScatterSeries::MarkerShapeCircle);
   seriesContours->setMarkerSize(5.0);

   ContoursExtractor contoursExtractor(framesFeatures);

  chart->addSeries(seriesContours);

  chart->legend()->hide();

  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(30);
  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(20);
  axisY->setRange((int)minValue-10,(int)maxValue+10);
  axisX->setRange((int)firstValueIndex-10,(int)lastValueIndex+10);

  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);
  seriesContours->attachAxis(axisX);
  seriesContours->attachAxis(axisY);
  seriesRegresionLines.push_back(new QLineSeries());
  seriesRegresionLines.back()->append(firstPart, maxValue);
  seriesRegresionLines.back()->append(firstPart, 20);
  seriesRegresionLines.push_back(new QLineSeries());
  seriesRegresionLines.back()->append(centerPart, maxValue);
  seriesRegresionLines.back()->append(centerPart, 20);
  for(int i=0;i<seriesRegresionLines.size();i++)
  {
      chart->addSeries(seriesRegresionLines.at(i));
      seriesRegresionLines.at(i)->attachAxis(axisX);
      seriesRegresionLines.at(i)->attachAxis(axisY);

  }
   setLayout();
}
void MainWindow::on_bLoad_pressed()
{
    setEnabledFeatureButtons(false);
    audioBuffers.clear();
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(1);
    desiredFormat.setCodec("audio/wav");
    desiredFormat.setSampleType(QAudioFormat::UnSignedInt);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);
    audioDecoder = new QAudioDecoder();
    audioDecoder->setAudioFormat(desiredFormat);
    wavFilePath = QFileDialog::getOpenFileName(this, tr("choose_import"), ".", tr("wav(*.wav)"));
    audioDecoder->setSourceFilename(wavFilePath);

    connect(audioDecoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(audioDecoder,SIGNAL(finished()),this,SLOT(decodingFinished()));
    audioDecoder->start();
}
