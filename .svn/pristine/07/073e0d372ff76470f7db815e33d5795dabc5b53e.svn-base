#include "guigesturerecognitiondtw.h"
#include "ui_GUIgesturerecognitiondtw.h"

#include <QString>
#include <QVector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

static const int maxY = thresholdLastFrame + 5;
static const int maxX = 800;


GUIgesturerecognitiondtw::GUIgesturerecognitiondtw(GestureRecognitionDTW *prec, QWidget *parent) : QWidget(parent), ui(new Ui::GUIgesturerecognitiondtw), _pGestureRec(prec)
{
    ui->setupUi(this);

    //Chart
    //0
    ui->chart->addGraph();
    //1
    ui->chart->addGraph();
    ui->chart->graph(1)->setPen(QPen(Qt::red));
    ui->chart->graph(1)->setLineStyle(QCPGraph::lsImpulse);
    //2
    ui->chart->addGraph();
    ui->chart->graph(2)->setPen(QPen(Qt::green));
    //3
    ui->chart->addGraph();
    ui->chart->graph(3)->setPen(QPen(Qt::green));

    ui->chart->xAxis->setLabel("Frames");
    ui->chart->yAxis->setLabel("DTW Value");
    ui->chart->xAxis->setRange(0, maxX);
    ui->chart->yAxis->setRange(0, maxY);

    for(int i=0; i<maxX;++i){
        thresh1.push_back(thresholdLastFrame);
        thresh2.push_back(thresholdDTW);
        frameThresh.push_back(i);
    }

    connect(_pGestureRec, SIGNAL(WindowSizeSignal()), this, SLOT(UpdateInterface()));

    connect(_pGestureRec, SIGNAL(classificationDone()), this, SLOT(updateClassLabel()));

}

void GUIgesturerecognitiondtw::UpdateInterface(){

    //Print window size
    int ws = _pGestureRec->getWindowSize();
    stringstream ss;
    ss << ws;
    ui->labelWS->setText(QString::fromStdString(ss.str()));

    //Print gesture labels
    string tmpLabel;
    for(int i=0;i<_pGestureRec->allLabels.size();++i){
        tmpLabel.append(_pGestureRec->allLabels[i].substr(1, _pGestureRec->allLabels[i].find_first_of(";", 0)-1));
        tmpLabel.append("\n\n");
    }
    ui->labelGestureLabel->setText(QString::fromStdString(tmpLabel));

    //Print best dtw only
    ui->labelDTWvalue->setText(QString::number(_pGestureRec->bestDTW));


    //Chart 0 (frame and dtw value)
    nFrames++;
    if(nFrames > maxX){
        nFrames = 0;
        ui->chart->graph(0)->clearData();
        ui->chart->graph(1)->clearData();
        DTWValues.clear();
        windowBorder.clear();
    }

    frames.push_back(nFrames);
    if(_pGestureRec->bestDTW > maxY)
        DTWValues.push_back(maxY);
    else
        DTWValues.push_back(_pGestureRec->bestDTW);
    ui->chart->graph(0)->setData(frames, DTWValues);

    //Chart 1 (window size)
    if(_pGestureRec->getWindowSize() == 1 && nFrames != 1)
        windowBorder.push_back(maxY);
    else
        windowBorder.push_back(0);

    ui->chart->graph(1)->setData(frames, windowBorder);

    //Chart 2 (thresholds)
    ui->chart->graph(2)->setData(frameThresh, thresh1);
    ui->chart->graph(3)->setData(frameThresh, thresh2);


    //For all charts
    ui->chart->replot();

}

void GUIgesturerecognitiondtw::updateClassLabel(){

    //Print label of gesture classified
    if(_pGestureRec->classificationString != "Unknown")
        ui->labelClassDTW->setText(QString::fromStdString(_pGestureRec->classificationString.substr(1, _pGestureRec->classificationString.find_first_of(";", 0)-1)));
    else
        ui->labelClassDTW->setText(QString::fromStdString(_pGestureRec->classificationString));

    //Store all gestures classified
    if(_pGestureRec->classificationString != "Unknown"){
        allGesturesClassified.append(_pGestureRec->classificationString.substr(1, _pGestureRec->classificationString.find_first_of(";", 0)-1));
        allGesturesClassified.append("\n");
    }
    ui->labelClass->setText(QString::fromStdString(allGesturesClassified));

    //Print all scores
    if(_pGestureRec->classificationString != "Unknown"){
        stringstream sscores;
        for(int i=0; i<_pGestureRec->allLabels.size(); ++i){
            string tmps = _pGestureRec->allLabels[i];
            sscores << tmps.substr(1, tmps.find_first_of(";", 0)-1) << "\t\t";
            sscores << _pGestureRec->allScores[i] << "\n";
        }
        ui->labelAllScores->setText(QString::fromStdString(sscores.str()));
    }

}

