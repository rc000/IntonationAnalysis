


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
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),chartView(NULL),
    frames_number(0),frameSize(0),
    ui(new Ui::MainWindow)
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
        ui->tableWidget->setVisible(false);
        ui->tableWidget->setColumnCount(2);
        ui->tableWidget->setColumnWidth(0,200);
        ui->tableWidget->setColumnWidth(1,200);
       // ui->tableWidget->setColumnWidth(2,200);

        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        ui->textBrowser->setVisible(false);
        connect(ui->tableWidget, SIGNAL( cellDoubleClicked (int, int) ),
         this, SLOT( cellClicked( int, int ) ) );
        connect(ui->tableWidget, SIGNAL( cellClicked (int, int) ),
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
void MainWindow::cellSelected(int nRow,int nCol)
{
    activeColumn = nRow;
}
void MainWindow::cellClicked(int nRow, int nCol)
{
   activeColumn = nRow;

  ui->textBrowser->clear();
   chart = new QChart();
  SegmentsDetector segmentsDetector = detectors.at(nRow);

  for (int i = 0;i<segmentsDetector.getAnalysisResults().size();i++)
  {
      ui->textBrowser->append(segmentsDetector.getAnalysisResults().at(i));

     // ui->textBrowser->setText(segmentsDetector.getAnalysisResults().at(i));
  }
  for (int i = 0;i<segmentsDetector.getStateChanges().size();i++)
  {
      qDebug()<<segmentsDetector.getStateChanges().at(i);
      ui->textBrowser->append(segmentsDetector.getStateChanges().at(i));
  }
 seriesSegments = segmentsDetector.getSeriesSegments();


seriesRegresionLines = segmentsDetector.getSeriesRegresionLines();
 seriesSegments->setMarkerShape(QScatterSeries::MarkerShapeCircle);
seriesSegments->setMarkerSize(10.0);
chart->addSeries(seriesSegments);

chart->legend()->hide();

QValueAxis *axisX = new QValueAxis;
axisX->setTickCount(30);
QValueAxis *axisY = new QValueAxis;
axisY->setTickCount(20);
axisY->setRange((int)segmentsDetector.getMinValue()-10,(int)segmentsDetector.getMaxValue()+10);
axisX->setRange((int)segmentsDetector.getIndexOfFirstValue()-10,(int)segmentsDetector.getIndexOfLastValue()+10);


chart->addAxis(axisX, Qt::AlignBottom);
chart->addAxis(axisY, Qt::AlignLeft);
seriesSegments->attachAxis(axisX);
seriesSegments->attachAxis(axisY);


for(int i=0;i<seriesRegresionLines.size();i++)
{
    chart->addSeries(seriesRegresionLines.at(i));
    seriesRegresionLines.at(i)->attachAxis(axisX);
    seriesRegresionLines.at(i)->attachAxis(axisY);

}
setLayout();

qDebug()<<"seriesRegresion main"<<seriesRegresionLines.size();

 qDebug()<<"after setting layout";

}
void MainWindow::setEnabledFeatureButtons(bool state)
{
        ui->bShowWaveform->setEnabled(state);
        ui->bPlay->setEnabled(state);
        ui->bRegression->setEnabled(state);
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
void MainWindow::putValuesIntoVector()
{

    sampleRate = audioBuffers[0].format().sampleRate();
    frameSize = audioBuffers[0].format().sampleRate()/40;

    for (QAudioBuffer audioBuffer : audioBuffers)
    {
        const qint16 *data = audioBuffer.constData<qint16>();
        for(int j=0;j<audioBuffer.sampleCount();j++)
        {
              wholeBuffer.emplace_back(data[j]);
        }
    }
}

ExtractionHelper MainWindow::extractFeatures()
{

    ExtractionHelper extractionHelper(wholeBuffer, peak, frameSize,sampleRate);
    double framesNumber = wholeBuffer.size()/1102;
    extractionHelper.calcF0(framesNumber);

    return extractionHelper;
  }

void MainWindow::decodingFinished()
{
    putValuesIntoVector();


    SegmentsDetector segmentsDetector(extractFeatures());
    qDebug()<<"przed find";
    segmentsDetector.findSegments();
    segmentsDetector.classification();
    setResultInTable(segmentsDetector);


    setEnabledFeatureButtons(true);
    qDebug()<<"decoding finished";
    if (wavFiles.size()>1)
    {
        wavFiles.erase(wavFiles.begin());
        loadWavFile(wavFiles.front());
    }
    wholeBuffer.clear();

}
void MainWindow::on_bShowWaveform_clicked()
{
   series = new QLineSeries();
   chart = new QChart();
   SegmentsDetector segmentsDetector = detectors.at(activeColumn);


  for(size_t i=0;i<segmentsDetector.getFeatures().getWholeSignal().size();i++)
   {
       series->append(i,segmentsDetector.getFeatures().getWholeSignal().at(i));
   }
 std::vector<QLineSeries*>framesLines;
 int counter =0;
 for(size_t i=0;i<segmentsDetector.getFeatures().getWholeSignal().size();i+=(frameSize - (frameSize/3)))
  {
     double start =0.0;
     double end = counter%2 ? -0.4 : 0.4;
     framesLines.push_back(new QLineSeries());
     framesLines.back()->append(i, start);
     framesLines.back()->append(i, end);
     framesLines.back()->append(i+frameSize, end);
     framesLines.back()->append(i+frameSize, start);
     counter++;
 }


 qDebug()<<"after regression lines";
 QValueAxis *axisX = new QValueAxis;
 axisX->setTickCount(20);
 QValueAxis *axisY = new QValueAxis;
 axisY->setTickCount(20);
 axisY->setRange(-0.5,0.5);
 chart->addAxis(axisX, Qt::AlignBottom);
 chart->addAxis(axisY, Qt::AlignLeft);
 chart->addSeries(series);

 series->attachAxis(axisX);
 series->attachAxis(axisY);
 for(int i=0;i<framesLines.size();i++)
 {
     chart->addSeries(framesLines.at(i));
     framesLines.at(i)->attachAxis(axisX);
     framesLines.at(i)->attachAxis(axisY);

 }
  chart->legend()->hide();
 // addAxis();
  setLayout();

}



void MainWindow::setLayout()
{

    if(chartView==nullptr)
    {
        chartView = new QChartView(chart);
       // ui->horizontalLayout_3->replaceWidget(ui->horizontalLayout_3->widget());
        ui->verticalLayout_2->addWidget(chartView);
         ui->textBrowser->setVisible(true);


    }
    else
    {
         chartView->setChart(chart);
        ui->textBrowser->reload();
        chartView->repaint();
    }



}
void MainWindow::addAxis()
{
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(20);
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
    qDebug()<<wavFilesList.at(activeColumn-praatFilesNumber);

    QSound::play(wavFilesList.at(activeColumn-praatFilesNumber));

}
void MainWindow::readBuffer()
{
    audioBuffers.emplace_back(audioDecoder->read());
}


void MainWindow::on_bLoad_pressed()
{

    if(!ui->tableWidget->isVisible())
        ui->tableWidget->setVisible(true);
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
    if(wavFiles.empty()) return;
    loadWavFile(this->wavFiles.front());

}



void MainWindow::on_bRegression_clicked()
{
    for(int i=0;i<seriesRegresionLines.size();i++)
    {
        seriesRegresionLines.at(i)->setVisible(!seriesRegresionLines.at(i)->isVisible());
    }
    setLayout();
}

void MainWindow::on_bPraat_clicked()
{
    QString filepath = (QFileDialog::getOpenFileName(this, tr("choose_import"), ".", tr("txt(*.txt)")));
    processPraatFile(filepath);
    if(!ui->tableWidget->isVisible())
        ui->tableWidget->setVisible(true);

}

void MainWindow::processPraatFile(QString filepath)
{
    praatFilesNumber++;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    std::vector<double> f0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        std::string stringLine = line.toStdString().substr(11,line.size());
         line = QString::fromStdString(stringLine);
        double value;
        if(line.at(0) == '-')
            value = 0.0;
        else
            value = line.toDouble();
        if(value > 50 && value < 400)
            f0.emplace_back(value);
        else {
            f0.emplace_back(0.0);
        }
      }

    file.close();
    ExtractionHelper exHelper;
    exHelper.setF0(f0);
    SegmentsDetector segmentsDetector(exHelper);
    segmentsDetector.findSegments();
    segmentsDetector.classification();

    rowCounter++;
    ui->tableWidget->setRowCount(rowCounter);
    std::size_t found = filepath.toStdString().find_last_of("/");
    ui->tableWidget->setItem(rowCounter-1,0,new QTableWidgetItem(filepath.toStdString().substr(found+1).c_str()));
    setResultInTable(segmentsDetector);
}
void MainWindow::setResultInTable(SegmentsDetector segmentsDetector)
{


    ui->tableWidget->setItem(rowCounter-1,1,new QTableWidgetItem(segmentsDetector.getResult().at(0)));

    /*for(int i = 0;i<segmentsDetector.getResult().size();i++)
    {
        ui->tableWidget->setItem(rowCounter-1,i+1,new QTableWidgetItem(segmentsDetector.getResult().at(i)));
    }*/
   //ui->tableWidget->setItem(rowCounter-1,1,new QTableWidgetItem(segmentsDetector.getResult()));

    detectors.push_back(segmentsDetector);

    obliczone = true;
    if(ui->tableWidget->item(rowCounter-1,0)->text().length()==0) return;
    if(ui->tableWidget->item(rowCounter-1,0)->text().at(0)!=ui->tableWidget->item(rowCounter-1,1)->text().at(0))
    {

        ui->tableWidget->item(rowCounter-1,0)->setBackgroundColor(Qt::red);
        ui->tableWidget->item(rowCounter-1,1)->setBackgroundColor(Qt::red);
       // }
    }
}
void MainWindow::on_bPraatAllFiles_clicked()
{
    QDir directory = QFileDialog::getExistingDirectory(this);

    QStringList list = directory.entryList(QStringList() << "*.txt" << "*.txt",QDir::Files);

    foreach(QString filename, list) {
        processPraatFile(directory.absoluteFilePath(filename));
    }
    if(!ui->tableWidget->isVisible())
        ui->tableWidget->setVisible(true);
}
