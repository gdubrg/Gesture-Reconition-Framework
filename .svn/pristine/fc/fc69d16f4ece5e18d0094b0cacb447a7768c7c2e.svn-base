#include "guigesturerecognitiontest.h"
#include "ui_GUIgesturerecognitiontest.h"
#include "conversionutility.h"

#include <QString>
#include <QVector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;


GUIgesturerecognitiontest::GUIgesturerecognitiontest(GestureRecognitionTEST *prec, QWidget *parent) : QWidget(parent), ui(new Ui::GUIgesturerecognitiontest), _pGestureRec(prec){

    ui->setupUi(this);

    //Joint Chart

    //Hand Right
    ui->widgetChart->addGraph();
    ui->widgetChart->graph(0)->setPen(QPen(Qt::blue));

    //Hand Left
    ui->widgetChart->addGraph();
    ui->widgetChart->graph(1)->setPen(QPen(Qt::green));

    //Elbow Right
    ui->widgetChart->addGraph();
    ui->widgetChart->graph(2)->setPen(QPen(Qt::darkBlue));

    //Elbow Left
    ui->widgetChart->addGraph();
    ui->widgetChart->graph(3)->setPen(QPen(Qt::darkGreen));

    //General setting chart
    ui->widgetChart->xAxis->setRange(0, 800);
    ui->widgetChart->yAxis->setRange(0, 1);
    ui->widgetChart->xAxis->setLabel("Frames");
    ui->widgetChart->yAxis->setLabel("Motion Value");
    ui->widgetChart->legend->setVisible(true);


    //Head Chart
    ui->widgetHead->addGraph();
    ui->widgetHead->graph(0)->setPen(QPen(Qt::blue));

    ui->widgetHead->addGraph();
    ui->widgetHead->graph(1)->setPen(QPen(Qt::green));

    ui->widgetHead->addGraph();
    ui->widgetHead->graph(2)->setPen(QPen(Qt::red));

    ui->widgetHead->xAxis->setRange(0, 800);
    ui->widgetHead->yAxis->setRange(-70, +70);
    ui->widgetHead->xAxis->setLabel("Frames");
    ui->widgetHead->yAxis->setLabel("Head Angles");
    ui->widgetHead->legend->setVisible(true);

    //Rotation Chart
    ui->widgetRot->addGraph();
    ui->widgetRot->xAxis->setRange(0, 800);
    ui->widgetRot->yAxis->setRange(-90, 90);
    ui->widgetRot->xAxis->setLabel("Frames");
    ui->widgetRot->yAxis->setLabel("Shoulder Rotation");
    ui->widgetRot->legend->setVisible(true);

    connect(_pGestureRec, SIGNAL(FrameAcquired()), this, SLOT(UpdateInterface()));
}

void GUIgesturerecognitiontest::UpdateInterface(){

    //Skeleton Image updating
    Mat& imageSkeleton = _pGestureRec->imageSkeleton; _pGestureRec->_frame.roll;
    QPixmap pixImageSkeleton = cvMatToQPixmap(imageSkeleton);


    if(pixImageSkeleton.isNull())
        ui->labelSkeleton->setText("Image Null");
    else
        ui->labelSkeleton->setPixmap(pixImageSkeleton.scaled(ui->labelSkeleton->size(), Qt::IgnoreAspectRatio));

    Mat& imageNew = _pGestureRec->imageNew;
    QPixmap pixImageNew = cvMatToQPixmap(imageNew);

    if(pixImageNew.isNull())
        ui->labelNewC->setText("Image Null");
    else
        ui->labelNewC->setPixmap(pixImageNew.scaled(ui->labelNewC->size(), Qt::IgnoreAspectRatio));

    ui->labelNewC->setHidden(true);
    ui->groupBox_2->setHidden(true);

    //Update frame number
    numFrames++;

    if(numFrames > 800){
        numFrames = 0;

        ui->widgetChart->graph(0)->clearData();
        ui->widgetChart->graph(1)->clearData();
        ui->widgetChart->graph(2)->clearData();
        ui->widgetChart->graph(3)->clearData();

        ui->widgetHead->graph(0)->clearData();
        ui->widgetHead->graph(1)->clearData();
        ui->widgetHead->graph(2)->clearData();

        ui->widgetRot->graph()->clearData();

        vHandRight.clear();
        vHandLeft.clear();
        vElbowLeft.clear();
        vElbowRight.clear();

        headPitch.clear();
        headYaw.clear();
        headRoll.clear();

        shoulderRotation.clear();
    }

    frames.push_back(numFrames);

    //Chart Hand Right
    vHandRight.push_back(_pGestureRec->vJoints[FS_handRight]);
    ui->widgetChart->graph(0)->setData(frames, vHandRight);
    ui->widgetChart->graph(0)->setName("Hand Right");

    //Chart Hand Left
    vHandLeft.push_back(_pGestureRec->vJoints[FS_handLeft]);
    ui->widgetChart->graph(1)->setData(frames, vHandLeft);
    ui->widgetChart->graph(1)->setName("Hand Left");

    //Chart Elbow Right
    vElbowRight.push_back(_pGestureRec->vJoints[FS_elbowRight]);
    ui->widgetChart->graph(2)->setData(frames, vElbowRight);
    ui->widgetChart->graph(2)->setName("Elbow Right");

    //Chart Elbow Right
    vElbowLeft.push_back(_pGestureRec->vJoints[FS_elbowLeft]);
    ui->widgetChart->graph(3)->setData(frames, vElbowLeft);
    ui->widgetChart->graph(3)->setName("Elbow Left");

    //Pitch
    headPitch.push_back(_pGestureRec->_frame.pitch);
    ui->widgetHead->graph(0)->setData(frames, headPitch);
    ui->widgetHead->graph(0)->setName("Pitch");

    //Yaw
    headYaw.push_back(_pGestureRec->_frame.yaw);
    ui->widgetHead->graph(1)->setData(frames, headYaw);
    ui->widgetHead->graph(1)->setName("Yaw");

    //Roll
    headRoll.push_back(_pGestureRec->_frame.roll);
    ui->widgetHead->graph(2)->setData(frames, headRoll);
    ui->widgetHead->graph(2)->setName("Roll");

    //Shoulder Rotation
    shoulderRotation.push_back(_pGestureRec->shoulderRotation);
    ui->widgetRot->graph()->setData(frames, shoulderRotation);
    ui->widgetRot->graph()->setName("Rotation Angle");

    //For all charts
    ui->widgetChart->replot();
    ui->widgetHead->replot();
    ui->widgetRot->replot();
}


