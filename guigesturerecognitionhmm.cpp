#include "guigesturerecognitionhmm.h"
#include "ui_guigesturerecognitionhmm.h"
#include "conversionutility.h"

GUIgesturerecognitionhmm::GUIgesturerecognitionhmm(GestureRecognitionHMM *prec, QWidget *parent) : QWidget(parent), ui(new Ui::GUIgesturerecognitionhmm), _pGestureRec(prec)
{
    ui->setupUi(this);

    connect(_pGestureRec, SIGNAL(FrameProcessed()), this, SLOT(UpdateInterface()));
}


void GUIgesturerecognitionhmm::UpdateInterface(){

    Mat& detection = _pGestureRec->getDetection();
    Mat& okDetection = _pGestureRec->getGestureStarted();
    Mat& hmmState = _pGestureRec->getStates();
    Mat& histo = _pGestureRec->getHisto();
    Mat& robustness = _pGestureRec->getRobustness();
    Mat& gesture = _pGestureRec->getGesture();
    Mat& histoBack = _pGestureRec->getHistoBack();
    Mat& histoPoint = _pGestureRec->getHistoPoint();
    Mat& histoSl = _pGestureRec->getHistoSl();
    Mat& histoSr = _pGestureRec->getHistoSr();
    Mat& histoZoom = _pGestureRec->getHistoZoom();
    Mat& histoZoomOut = _pGestureRec->getHistoZoomOut();
    Mat& histoSU = _pGestureRec->getHistoSU();
    Mat& histoSD = _pGestureRec->getHistoSD();
    Mat& histoExit = _pGestureRec->getHistoExit();
    Mat& histoRot = _pGestureRec->getHistoRot();

    QPixmap pixDetection = cvMatToQPixmap(detection);
    QPixmap pixOkDetection = cvMatToQPixmap(okDetection);
    QPixmap pixHmmState = cvMatToQPixmap(hmmState);
    QPixmap pixHisto = cvMatToQPixmap(histo);
    QPixmap pixRobustness = cvMatToQPixmap(robustness);
    QPixmap pixGesture = cvMatToQPixmap(gesture);
    QPixmap pixHistoBack = cvMatToQPixmap(histoBack);
    QPixmap pixHistoPoint = cvMatToQPixmap(histoPoint);
    QPixmap pixHistoSl = cvMatToQPixmap(histoSl);
    QPixmap pixHistoSr = cvMatToQPixmap(histoSr);
    QPixmap pixHistoZoom = cvMatToQPixmap(histoZoom);
    QPixmap pixHistoZoomOut = cvMatToQPixmap(histoZoomOut);
    QPixmap pixHistoSU = cvMatToQPixmap(histoSU);
    QPixmap pixHistoSD = cvMatToQPixmap(histoSD);
    QPixmap pixHistoExit = cvMatToQPixmap(histoExit);
    QPixmap pixHistoRot = cvMatToQPixmap(histoRot);

    if(pixDetection.isNull())
        ui->labelDetection->setText("Image NULL");
    else
        ui->labelDetection->setPixmap(pixDetection.scaled(ui->labelDetection->size(), Qt::IgnoreAspectRatio));

    if(pixHmmState.isNull())
        ui->labelState->setText("Image NULL");
    else
        ui->labelState->setPixmap(pixHmmState.scaled(ui->labelState->size(), Qt::IgnoreAspectRatio));

    if(pixHisto.isNull())
        ui->labelHisto->setText("Image NULL");
    else
        ui->labelHisto->setPixmap(pixHisto.scaled(ui->labelHisto->size(), Qt::IgnoreAspectRatio));

    if(pixGesture.isNull())
        ui->labelGesture->setText("Image NULL");
    else
        ui->labelGesture->setPixmap(pixGesture.scaled(ui->labelGesture->size(), Qt::IgnoreAspectRatio));

    if(pixHistoBack.isNull())
        ui->labelBack->setText("Image NULL");
    else
        ui->labelBack->setPixmap(pixHistoBack.scaled(ui->labelBack->size(), Qt::IgnoreAspectRatio));

    if(pixHistoPoint.isNull())
        ui->labelPoint->setText("Image NULL");
    else
        ui->labelPoint->setPixmap(pixHistoPoint.scaled(ui->labelPoint->size(), Qt::IgnoreAspectRatio));

    if(pixHistoSl.isNull())
        ui->labelSl->setText("Image NULL");
    else
        ui->labelSl->setPixmap(pixHistoSl.scaled(ui->labelSl->size(), Qt::IgnoreAspectRatio));

    if(pixHistoSr.isNull())
        ui->labelSr->setText("Image NULL");
    else
        ui->labelSr->setPixmap(pixHistoSr.scaled(ui->labelSr->size(), Qt::IgnoreAspectRatio));

    if(pixHistoZoom.isNull())
        ui->labelZoom->setText("Image NULL");
    else
        ui->labelZoom->setPixmap(pixHistoZoom.scaled(ui->labelZoom->size(), Qt::IgnoreAspectRatio));

    if(pixHistoZoomOut.isNull())
        ui->labelZoomOut->setText("Image NULL");
    else
        ui->labelZoomOut->setPixmap(pixHistoZoomOut.scaled(ui->labelZoomOut->size(), Qt::IgnoreAspectRatio));

    if(pixHistoSU.isNull())
        ui->labelSU->setText("Image NULL");
    else
        ui->labelSU->setPixmap(pixHistoSU.scaled(ui->labelSU->size(), Qt::IgnoreAspectRatio));

    if(pixHistoSD.isNull())
        ui->labelSD->setText("Image NULL");
    else
        ui->labelSD->setPixmap(pixHistoSD.scaled(ui->labelSD->size(), Qt::IgnoreAspectRatio));

    if(pixHistoExit.isNull())
        ui->labelExit->setText("Image NULL");
    else
        ui->labelExit->setPixmap(pixHistoExit.scaled(ui->labelExit->size(), Qt::IgnoreAspectRatio));

    if(pixHistoRot.isNull())
        ui->labelRot->setText("Image NULL");
    else
        ui->labelRot->setPixmap(pixHistoRot.scaled(ui->labelRot->size(), Qt::IgnoreAspectRatio));


    //Semaforo
    if(_pGestureRec->getLevelRobustness() > 125){
        ui->labelGood->setStyleSheet("QLabel {background-color: rgb(0, 255, 0); color: black; }");
        ui->labelModerate->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelLow->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
    }
    else if (_pGestureRec->getLevelRobustness() > 110){
        ui->labelGood->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelModerate->setStyleSheet("QLabel {background-color: yellow; color: black; }");
        ui->labelLow->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
    }
    else if (_pGestureRec->getLevelRobustness() > 50){
        ui->labelGood->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelModerate->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelLow->setStyleSheet("QLabel {background-color: red; color: black; }");
    }

    else {
        ui->labelGood->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelModerate->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
        ui->labelLow->setStyleSheet("QLabel {background-color: lightgrey; color: black; }");
    }

}
