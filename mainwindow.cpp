


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<fstream>
#include<QFileInfo>
#include<QTime>
#include<QValueAxis>
#include<cmath>
#include<limits>
#include<QSound>
#include<QFileDialog>
#include<Windows.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),chartView(NULL),
    frames_number(0),samples_per_frame(0),
    frames(nullptr),ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        audioProbe = new QAudioProbe(this);
        emptyWidget = new QWidget;
        layout = new BorderLayout;
        layout->addWidget(ui->leftButtons, BorderLayout::West);
        layout->addWidget(ui->wBottom, BorderLayout::South);

        QWidget *widget = new QWidget;
        widget->setLayout(ui->horizontalLayout_2);
        layout->addWidget(widget, BorderLayout::Center);


        centralWidget = new QWidget;
        centralWidget ->setLayout(layout);

        setCentralWidget(centralWidget);
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setColumnWidth(0,200);
        ui->tableWidget->setColumnWidth(1,200);

        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        ui->textBrowser->setVisible(false);
        connect(ui->tableWidget, SIGNAL( cellDoubleClicked (int, int) ),
         this, SLOT( cellSelected( int, int ) ) );
        setEnabledFeatureButtons(false);
        desiredFormat.setChannelCount(1);
        desiredFormat.setCodec("audio/wav");
        desiredFormat.setSampleType(QAudioFormat::UnSignedInt);
        desiredFormat.setSampleRate(44100);
        desiredFormat.setSampleSize(16);


}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::cellSelected(int nRow, int nCol)
{
   activeColumn = nRow;
  ui->textBrowser->clear();
 qDebug()<<nRow<<" "<<nCol;
 qDebug()<<"file "<<wavFilesList.at(nRow);
  chart = new QChart();
  ContoursExtractor contoursExtractor = extractors.at(nRow);

  for (int i = 0;i<contoursExtractor.getAnalysisResults().size();i++)
  {
      qDebug()<<contoursExtractor.getAnalysisResults().at(i);
      ui->textBrowser->append(contoursExtractor.getAnalysisResults().at(i));

     // ui->textBrowser->setText(contoursExtractor.getAnalysisResults().at(i));
  }
  for (int i = 0;i<contoursExtractor.getStateChanges().size();i++)
  {
      qDebug()<<contoursExtractor.getStateChanges().at(i);
      ui->textBrowser->append(contoursExtractor.getStateChanges().at(i));
  }
 seriesContours = contoursExtractor.getSeriesContours();
 seriesContours->setMarkerShape(QScatterSeries::MarkerShapeCircle);
 seriesContours->setMarkerSize(5.0);

seriesRegresionLines = contoursExtractor.getSeriesRegresionLines();
qDebug()<<"before adding seriesContours";
chart->addSeries(seriesContours);

chart->legend()->hide();


QValueAxis *axisX = new QValueAxis;
axisX->setTickCount(30);
QValueAxis *axisY = new QValueAxis;
axisY->setTickCount(20);
axisY->setRange((int)contoursExtractor.getMinValue()-10,(int)contoursExtractor.getMaxValue()+10);
axisX->setRange((int)contoursExtractor.getIndexOfFirstValue()-10,(int)contoursExtractor.getIndexOfLastValue()+10);

chart->addAxis(axisX, Qt::AlignBottom);
chart->addAxis(axisY, Qt::AlignLeft);
seriesContours->attachAxis(axisX);
seriesContours->attachAxis(axisY);
qDebug()<<"before regression lines";
seriesRegresionLines.push_back(new QLineSeries());
seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfBeginningPart(), contoursExtractor.getMaxValue());
seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfBeginningPart(), 20);
seriesRegresionLines.push_back(new QLineSeries());
seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfCenterPart(), contoursExtractor.getMaxValue());
seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfCenterPart(), 20);
qDebug()<<"after regression lines";

for(int i=0;i<seriesRegresionLines.size();i++)
{
    chart->addSeries(seriesRegresionLines.at(i));
    seriesRegresionLines.at(i)->attachAxis(axisX);
    seriesRegresionLines.at(i)->attachAxis(axisY);

}
qDebug()<<"seriesRegresion main"<<seriesRegresionLines.size();

 setLayout();
 qDebug()<<"after setting layout";

}
void MainWindow::setEnabledFeatureButtons(bool state)
{
        ui->bF0->setEnabled(state);
        ui->bShowEnergy->setEnabled(state);
        ui->bShowSpectrum->setEnabled(state);
        ui->bShowWaveform->setEnabled(state);
        ui->bPlay->setEnabled(state);
}
bool obliczone = false;
void MainWindow::startRecording()
{
   /* audioBuffers.clear();
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/wav");
    audioSettings.setSampleRate(44100);
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioSettings.setBitRate(32000);
    audioRecorder->setAudioSettings(audioSettings);
    audioRecorder->setOu
tputLocation(QUrl::fromLocalFile(QFileInfo("records/recorded_sentence").absoluteFilePath()));
    if(!connected)
    {
        if (audioProbe->setSource(audioRecorder))
        {
            connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)),
                this, SLOT(getBuffer(QAudioBuffer)));
            connected=true;
        }
    }
    audioRecorder->record();*/
}
void MainWindow::getBuffer(QAudioBuffer buffer)
{
    audioBuffers.emplace_back(buffer);
}
void MainWindow::calculation()
{
    qDebug()<<"usuwanko";
     for (int i = 0;i<frames_number;i++)
    {
        qDebug()<<"usuwanie frama "<<i;
        delete frames[i];
    }
    if(frames!=nullptr)
    {
        qDebug()<<"usuwanie frames";
        delete frames;
    }
    qDebug()<<"po usuwaniu";
     sampleRate = audioBuffers[0].format().sampleRate();
    samples_per_frame = audioBuffers[0].format().sampleRate()/20;

    frames_number = audioBuffers.size()*2;
    frames = new qint16 *[frames_number];

    for (int i = 0; i < frames_number; i++)
            frames[i] = new qint16[samples_per_frame];
    for (int i =0; i<samples_per_frame; i++)
            frames[frames_number-1][i] = 0;

    qDebug()<<"po petlach "<<audioBuffers.size();
whole_signal_size = 0;

    for(int i=0;i<audioBuffers.size();i++)
    {
         whole_signal_size+=audioBuffers[i].sampleCount();
         qDebug()<<"size "<<whole_signal_size;
    }



    whole_signal=new qint16[whole_signal_size];
    //qDebug()<<"whole signal "<<whole_signal_size * sizeof (qint16);

    int index_frame = 0;
    int index = 0;
    for(size_t i=0;i<audioBuffers.size();i++)
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

    qDebug()<<"po kolejnych petlach";



    FeaturesExtractor::whole_signal = whole_signal;
    FeaturesExtractor::whole_signal_size = whole_signal_size;

    for(int i=0;i<audioBuffers.size()*2;i++)
    {
         extractFeatures(peak,samples_per_frame,i);
    }
   ContoursExtractor contoursExtractor(framesFeatures);
   contoursExtractor.findContours();
   ui->tableWidget->setItem(rowCounter-1,1,new QTableWidgetItem(contoursExtractor.getResult()));
   extractors.push_back(contoursExtractor);
   size_t bytes = sizeof(extractors[0]) * extractors.size();
   qDebug()<<"rozmiar "<<bytes;

   obliczone = true;

       //framesFeatures.clear();



 }

void MainWindow::extractFeatures(qreal peak,int sample_per_frame,int frame_number)
{

    FeaturesExtractor  featuresExtractor(frames[frame_number], peak, sample_per_frame,sampleRate);
    framesFeatures[framesFeatures.size()-1].energy_emplace_back(featuresExtractor.calcEnergy());
     /*framesFeatures[framesFeatures.size()-1].zcr_emplace_back(featuresExtractor.calcZCR());*/
    framesFeatures[framesFeatures.size()-1].f0_emplace_back(featuresExtractor.calcF0(frame_number));
    //framesFeatures[framesFeatures.size()-1].fftBuffer_emplace_back(featuresExtractor.calcFFT());
}

void MainWindow::decodingFinished()
{
    framesFeatures.clear();
    framesFeatures.emplace_back(SingleFrameFeatures());
    calculation();
    setEnabledFeatureButtons(true);
    qDebug()<<"decoding finished";
    if (wavFiles.size()>1)
    {
        wavFiles.erase(wavFiles.begin());
        loadWavFile(wavFiles.front());
    }

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
  /* series = new QLineSeries();
   chart = new QChart();
   for(size_t i=0;i<framesFeatures[framesFeatures.size()-1].fftBuffer_size();i++)
   {
       series->append(i,framesFeatures[framesFeatures.size()-1].fftBuffer_value(i));
   }
  chart->legend()->hide();
  chart->addSeries(series);
  addAxis();
  setLayout();*/
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
    qDebug()<<"set layout";

    if(chartView==nullptr)
    {
        chartView = new QChartView(chart);
       // ui->horizontalLayout_3->replaceWidget(ui->horizontalLayout_3->widget());
        ui->verticalLayout_2->addWidget(chartView);
        qDebug()<<"if";
        ui->textBrowser->setVisible(true);


    }
    else
    {
        qDebug()<<"else"<<endl;
        chartView->setChart(chart);
        ui->textBrowser->reload();
        chartView->repaint();
    }
   /* QWidget *widget = new QWidget;
    qDebug()<<"set layout2";

    widget->setLayout(ui->horizontalLayout_2);
    layout->addWidget(widget, BorderLayout::Center);
    qDebug()<<"set layout3";

    centralWidget = new QWidget;
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);*/

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
    qDebug()<<activeColumn<<" ac";
    qDebug()<<wavFilesList.size();
    qDebug()<<wavFilesList.at(activeColumn);

    QSound::play(wavFilesList.at(activeColumn));

}
void MainWindow::readBuffer()
{
    audioBuffers.emplace_back(audioDecoder->read());
    qDebug()<<"odczytane "<<audioBuffers.size();
}

void MainWindow::on_bF0_clicked()
{

   chart = new QChart();
   qDebug()<<extractors.size();
   qDebug()<<activeColumn;
    ContoursExtractor contoursExtractor = extractors.at(activeColumn);

   seriesContours = contoursExtractor.getSeriesContours();
   seriesContours->setMarkerShape(QScatterSeries::MarkerShapeCircle);
   seriesContours->setMarkerSize(5.0);

  seriesRegresionLines = contoursExtractor.getSeriesRegresionLines();
  chart->addSeries(seriesContours);

  chart->legend()->hide();

  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(30);
  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(20);
  axisY->setRange((int)contoursExtractor.getMinValue()-10,(int)contoursExtractor.getMaxValue()+10);
  axisX->setRange((int)contoursExtractor.getIndexOfFirstValue()-10,(int)contoursExtractor.getIndexOfLastValue()+10);

  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);
  seriesContours->attachAxis(axisX);
  seriesContours->attachAxis(axisY);
  seriesRegresionLines.push_back(new QLineSeries());
  seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfBeginningPart(), contoursExtractor.getMaxValue());
  seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfBeginningPart(), 20);
  seriesRegresionLines.push_back(new QLineSeries());
  seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfCenterPart(), contoursExtractor.getMaxValue());
  seriesRegresionLines.back()->append(contoursExtractor.getLastIndexOfCenterPart(), 20);
  for(int i=0;i<seriesRegresionLines.size();i++)
  {
      chart->addSeries(seriesRegresionLines.at(i));
      seriesRegresionLines.at(i)->attachAxis(axisX);
      seriesRegresionLines.at(i)->attachAxis(axisY);

  }
  qDebug()<<"seriesRegresion main"<<seriesRegresionLines.size();

   setLayout();
}
void MainWindow::on_bLoad_pressed()
{


    setEnabledFeatureButtons(false);
    audioBuffers.clear();

    wavFilePath = (QFileDialog::getOpenFileName(this, tr("choose_import"), ".", tr("wav(*.wav)")));
    wavFilesList.push_back(wavFilePath);
    loadWavFile(wavFilePath);
}

void MainWindow::loadWavFile(QString wavFilePath)
{
    qDebug()<<wavFilePath;
    audioDecoder = new QAudioDecoder();
    rowCounter++;
    ui->tableWidget->setRowCount(rowCounter);
    audioDecoder->setAudioFormat(desiredFormat);
    audioDecoder->setSourceFilename(wavFilePath);
    std::size_t found = wavFilePath.toStdString().find_last_of("/");
    ui->tableWidget->setItem(rowCounter-1,0,new QTableWidgetItem(wavFilePath.toStdString().substr(found+1).c_str()));
    this->audioDecoder = audioDecoder;
    connect(audioDecoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(audioDecoder,SIGNAL(finished()),this,SLOT(decodingFinished()));
    audioBuffers.clear();
    audioDecoder->start();
}
void MainWindow::on_bTestBase_clicked()
{
    QDir directory = QFileDialog::getExistingDirectory(this);

    QStringList list = directory.entryList(QStringList() << "*.wav" << "*.wav",QDir::Files);

    obliczone = false;
    qDebug()<<"Ram pam";

    foreach(QString filename, list) {
        wavFilesList.append(directory.absoluteFilePath(filename));
        qDebug()<<"file "<<directory.absoluteFilePath(filename);
       this->wavFiles.emplace_back(directory.absoluteFilePath(filename));
    }
    loadWavFile(this->wavFiles.front());

}
