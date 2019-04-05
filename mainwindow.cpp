


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
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setColumnWidth(0,200);
        ui->tableWidget->setColumnWidth(1,200);
        ui->tableWidget->setColumnWidth(2,200);

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
 qDebug()<<nRow<<" "<<nCol;
   chart = new QChart();
  ContoursDetector contoursDetector = detectors.at(nRow);

  for (int i = 0;i<contoursDetector.getAnalysisResults().size();i++)
  {
      qDebug()<<contoursDetector.getAnalysisResults().at(i);
      ui->textBrowser->append(contoursDetector.getAnalysisResults().at(i));

     // ui->textBrowser->setText(contoursDetector.getAnalysisResults().at(i));
  }
  for (int i = 0;i<contoursDetector.getStateChanges().size();i++)
  {
      qDebug()<<contoursDetector.getStateChanges().at(i);
      ui->textBrowser->append(contoursDetector.getStateChanges().at(i));
  }
 seriesContours = contoursDetector.getSeriesContours();


seriesRegresionLines = contoursDetector.getSeriesRegresionLines();
qDebug()<<"before adding seriesContours";
seriesContours->setMarkerShape(QScatterSeries::MarkerShapeCircle);
seriesContours->setMarkerSize(8.0);
chart->addSeries(seriesContours);

chart->legend()->hide();

QValueAxis *axisX = new QValueAxis;
axisX->setTickCount(30);
QValueAxis *axisY = new QValueAxis;
axisY->setTickCount(20);
axisY->setRange((int)contoursDetector.getMinValue()-10,(int)contoursDetector.getMaxValue()+10);
axisX->setRange((int)contoursDetector.getIndexOfFirstValue()-10,(int)contoursDetector.getIndexOfLastValue()+10);


chart->addAxis(axisX, Qt::AlignBottom);
chart->addAxis(axisY, Qt::AlignLeft);
seriesContours->attachAxis(axisX);
seriesContours->attachAxis(axisY);
qDebug()<<"before regression lines";
seriesRegresionLines.push_back(new QLineSeries());
seriesRegresionLines.back()->append(contoursDetector.getLastIndexOfFirstPart(), contoursDetector.getMaxValue());
seriesRegresionLines.back()->append(contoursDetector.getLastIndexOfFirstPart(), 20);
seriesRegresionLines.push_back(new QLineSeries());
seriesRegresionLines.back()->append(contoursDetector.getLastIndexOfCenterPart(), contoursDetector.getMaxValue());
seriesRegresionLines.back()->append(contoursDetector.getLastIndexOfCenterPart(), 20);
qDebug()<<"after regression lines";

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


    ContoursDetector contoursDetector(extractFeatures());
    qDebug()<<"przed find";
    contoursDetector.findContours();
    contoursDetector.classification();
    for(int i = 0;i<contoursDetector.getResult().size();i++)
    {
        ui->tableWidget->setItem(rowCounter-1,i+1,new QTableWidgetItem(contoursDetector.getResult().at(i)));
    }
   //ui->tableWidget->setItem(rowCounter-1,1,new QTableWidgetItem(contoursDetector.getResult()));

    detectors.push_back(contoursDetector);

    obliczone = true;
    if(ui->tableWidget->item(rowCounter-1,0)->text().length()==0) return;
    if(ui->tableWidget->item(rowCounter-1,0)->text().at(0)!=ui->tableWidget->item(rowCounter-1,1)->text().at(0))
    {
        /*if(ui->tableWidget->item(rowCounter-1,2)->text().at(0)!=""
                || (ui->tableWidget->item(rowCounter-1,0)->text().at(0)!=ui->tableWidget->item(rowCounter-1,2)->text().at(0)))
        {*/
        ui->tableWidget->item(rowCounter-1,0)->setBackgroundColor(Qt::red);
        ui->tableWidget->item(rowCounter-1,1)->setBackgroundColor(Qt::red);
       // }
    }

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
   ContoursDetector contoursDetector = detectors.at(activeColumn);


  for(size_t i=0;i<contoursDetector.getFeatures().getWholeSignal().size();i++)
   {
       series->append(i,contoursDetector.getFeatures().getWholeSignal().at(i));
   }
 std::vector<QLineSeries*>framesLines;
 int counter =0;
 for(size_t i=0;i<contoursDetector.getFeatures().getWholeSignal().size();i+=(frameSize - (frameSize/3)))
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
    qDebug()<<wavFilesList.at(activeColumn);

    QSound::play(wavFilesList.at(activeColumn));

}
void MainWindow::readBuffer()
{
    audioBuffers.emplace_back(audioDecoder->read());
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
    if(wavFiles.empty()) return;
    loadWavFile(this->wavFiles.front());

}

void MainWindow::on_pushButton_clicked()
{
    chart = new QChart();

    QScatterSeries *seriesContours1 = new QScatterSeries();
    seriesContours1->append(1990,151.3);
    seriesContours1->append(1995,135.5);
    seriesContours1->append(2000,102.1);
    seriesContours1->append(2005,93.0);
    seriesContours1->append(2010,69.2);
    seriesContours1->append(2015,65.1);

    std::vector<double> blabla;
    blabla.emplace_back(151.3);
    blabla.emplace_back(135.5);
    blabla.emplace_back(102.1);
    blabla.emplace_back(93.0);
    blabla.emplace_back(69.2);
    blabla.emplace_back(65.1);

    seriesContours1->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesContours1->setMarkerSize(20.0);
    chart->addSeries(seriesContours1);

    chart->legend()->hide();

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Lata");
    axisX->setTickCount(6);
    QValueAxis *axisY = new QValueAxis;
    //axisY->setTickCount(6);
    axisY->setRange(60,160);
    axisX->setRange(1990,2015);
    axisY->setTitleText("Wydobycie węgla w Polsce w mln ton");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    seriesContours1->attachAxis(axisX);
    seriesContours1->attachAxis(axisY);

    double prevA = 0.0;
    double prevB = 0.0;

        double A = 0.0;
        double B = 0.0;
        double sigX = 0.0;
        double sigY = 0.0;
        double sigXY = 0.0;
        double sigSqrX = 0.0;
        double sigSqrY = 0.0;
        int j;
        for (j=0;j<blabla.size();j++)
        {
            sigX+=j;
            sigY+=blabla.at(j);
            sigXY+=j*blabla.at(j);
            sigSqrX+=j*j;
            sigSqrY+=blabla.at(j)*blabla.at(j);
        }
        A = (j * sigXY - sigX * sigY) / (j * sigSqrX - sigX * sigX);
        B =  (sigY - A * sigX) / j;

        prevA=A;
        prevB=B;

        QLineSeries *lineSeries = new QLineSeries();
        lineSeries->append(1990, B);
        lineSeries->append(2015,A * 5 + B);

        chart->addSeries(lineSeries);
        lineSeries->attachAxis(axisX);
        lineSeries->attachAxis(axisY);


qDebug()<<A;

    setLayout();




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
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    std::vector<double> f0;
    while(!in.atEnd()) {
        QString line = in.readLine();

        std::string stringLine = line.toStdString();
        std::string number = stringLine.substr(11,stringLine.length());
        line = QString::fromStdString(number);
        double value;
        if(line.at(0) == '-')
            value = 0.0;
        else
            value = line.toDouble();
        f0.emplace_back(value);
        qDebug()<<line<<" value "<<value;
      }

    file.close();
    ExtractionHelper exHelper;
    exHelper.setF0(f0);
    ContoursDetector contoursDetector(exHelper);
    qDebug()<<"przed find";
    contoursDetector.findContours();
    contoursDetector.classification();

    rowCounter++;
    ui->tableWidget->setRowCount(rowCounter);

    std::size_t found = filepath.toStdString().find_last_of("/");
    ui->tableWidget->setItem(rowCounter-1,0,new QTableWidgetItem(filepath.toStdString().substr(found+1).c_str()));


    for(int i = 0;i<contoursDetector.getResult().size();i++)
    {
        ui->tableWidget->setItem(rowCounter-1,i+1,new QTableWidgetItem(contoursDetector.getResult().at(i)));
    }
   //ui->tableWidget->setItem(rowCounter-1,1,new QTableWidgetItem(contoursDetector.getResult()));

    detectors.push_back(contoursDetector);

    obliczone = true;
    if(ui->tableWidget->item(rowCounter-1,0)->text().length()==0) return;
    if(ui->tableWidget->item(rowCounter-1,0)->text().at(0)!=ui->tableWidget->item(rowCounter-1,1)->text().at(0))
    {

        ui->tableWidget->item(rowCounter-1,0)->setBackgroundColor(Qt::red);
        ui->tableWidget->item(rowCounter-1,1)->setBackgroundColor(Qt::red);
       // }
    }
}
