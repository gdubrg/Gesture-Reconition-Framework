#include "guigesturerecognitionsave.h"
#include "ui_GUIgesturerecognitionsave.h"
#include "conversionutility.h"

#include <QString>
#include <QVector>
#include <string>
#include <iostream>
#include <sstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

using namespace std;
using namespace cv;


GUIgesturerecognitionsave::GUIgesturerecognitionsave(GestureRecognitionSave *prec, QWidget *parent) : QWidget(parent), ui(new Ui::GUIgesturerecognitionsave), _pGestureRec(prec){

    ui->setupUi(this); 

    ui->progressBar->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBarOpening->setValue(0);
    ui->textFilename->setText("default_name");
    ui->labelNFrame->setAlignment(Qt::AlignHCenter);

    //Create all type of action from json file
    QFile jsonFile("./gestures/settings/configuration.json");
    jsonFile.open(QFile::ReadOnly);
    QString contentJson = jsonFile.readAll();
    jsonFile.close();
    QJsonDocument documentJson = QJsonDocument::fromJson(contentJson.toUtf8());
    QJsonObject objectJson = documentJson.object();
    QJsonValue valueJson = objectJson.value("types");
    QJsonArray arrayJson = valueJson.toArray();
    foreach(const QJsonValue &v, arrayJson)
        ui->comboBox->addItem(v.toObject().value("type").toString());

    ui->buttonSubmit->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->labelSaveState->setText("Not saved");
    ui->labelSaveState->setAlignment(Qt::AlignCenter);

    //Start to acquire
    connect(ui->buttonStart, SIGNAL(pressed()), _pGestureRec, SLOT(startRec()));
    connect(ui->buttonStart, SIGNAL(pressed()), this, SLOT(start()));

    //Stop acquisition
    connect(_pGestureRec, SIGNAL(acquisitionCompleted()), this, SLOT(stop()));
    connect(ui->buttonStop, SIGNAL(pressed()), _pGestureRec, SLOT(stopRec()));

    //Update interface
    connect(_pGestureRec, SIGNAL(frameArrived()), this, SLOT(updateInterface()));

    //Update during saving
    connect(_pGestureRec, SIGNAL(saving()), this, SLOT(updateSaving()));

    //Choose a file
    connect(ui->buttonOpen, SIGNAL(pressed()), _pGestureRec, SLOT(chooseFile()));

    //Opening a file
    connect(_pGestureRec, SIGNAL(opening()), this, SLOT(updateOpening()));

    //Handle frames
    connect(_pGestureRec, SIGNAL(handleFrame()), this, SLOT(annotate()));

    //Frame button
    connect(ui->buttonFwd, SIGNAL(pressed()), _pGestureRec, SLOT(fwd()));
    connect(ui->buttonBck, SIGNAL(pressed()), _pGestureRec, SLOT(bck()));
    connect(ui->buttonFwd, SIGNAL(pressed()), this, SLOT(syncSlider()));
    connect(ui->buttonBck, SIGNAL(pressed()), this, SLOT(syncSlider()));

    //Submit Button
    connect(ui->buttonSubmit, SIGNAL(pressed()), this, SLOT(submit()));
    connect(ui->buttonSubmit, SIGNAL(pressed()), _pGestureRec, SLOT(appendAnnotation()));

    //Save Button
    connect(ui->buttonSave, SIGNAL(pressed()), this, SLOT(save()));
    connect(ui->buttonSave, SIGNAL(pressed()), _pGestureRec, SLOT(createAnnotation()));

    //Reset Button
    connect(ui->buttonReset, SIGNAL(pressed()), this, SLOT(reset()));

    //Slider
    connect(ui->sliderFrame, SIGNAL(sliderMoved(int)), _pGestureRec, SLOT(fwd(int)));

}

void GUIgesturerecognitionsave::start(){
    ui->labelControl->setText("Started!");
    _pGestureRec->setFilename(ui->textFilename->text().toStdString());
}

void GUIgesturerecognitionsave::stop(){
    ui->labelControl->setText("Acquisition ended");
    ui->progressBar_2->setValue(0);
    ui->progressBar->setValue(0);
    ui->textFilename->setText("default_name");
}

void GUIgesturerecognitionsave::updateInterface(){
    ui->progressBar->setValue((_pGestureRec->vFrames.size()*100)/1000);
}

void GUIgesturerecognitionsave::updateSaving(){
    ui->labelControl->setText("Saving...");
    ui->progressBar_2->setValue(((_pGestureRec->savingState+1)*100)/_pGestureRec->vFrames.size());
}

void GUIgesturerecognitionsave::updateOpening(){
    ui->progressBarOpening->setValue(((_pGestureRec->openingState+1)*100)/_pGestureRec->openingSize);
    ui->sliderFrame->setSliderPosition(0);
}

void GUIgesturerecognitionsave::annotate(){

    //Show RGB frames
    Mat3b& frameRGB = _pGestureRec->vRGBFrames[_pGestureRec->idxFrame];
    QPixmap pixFrameRGB = cvMatToQPixmap(frameRGB);
    if(pixFrameRGB.isNull())
        ui->labelFrame->setText("Image Null");
    else
        ui->labelFrame->setPixmap(pixFrameRGB.scaled(ui->labelFrame->size(), Qt::IgnoreAspectRatio));

    //Slider
    ui->sliderFrame->setTickInterval(1);

    ui->labelNFrame->setText(QString::number(_pGestureRec->idxFrame));

    ui->labelTotFrame->setText(QString::number(_pGestureRec->vRGBFrames.size()-1));

    ui->buttonSubmit->setEnabled(true);
    ui->buttonSave->setEnabled(true);

    ui->labelSaveState->setText("Not saved");
    ui->labelSaveState->setStyleSheet("background-color:red;");

    //bufferSubmissions.str(std::string());

    ui->sliderFrame->setMaximum((_pGestureRec->vRGBFrames.size()-1));

}

void GUIgesturerecognitionsave::submit(){

    //Set user input data
    _pGestureRec->setDataSubmission(ui->lineBegin->text().toInt(), ui->lineEnd->text().toInt(), ui->comboBox->currentText().toStdString());

    //To store all submissions
    if( ui->lineBegin->text() != NULL){
        bufferSubmissions << ui->lineBegin->text().toStdString() << "\t" << ui->lineEnd->text().toStdString() << "\t";
        bufferSubmissions << ui->comboBox->currentText().toStdString() << "\n";
        ui->textBrowserSub->setText(QString::fromStdString(bufferSubmissions.str()));
    }

    //Reset input fields
    ui->lineBegin->clear();
    ui->lineEnd->clear();
}

void GUIgesturerecognitionsave::save(){

    if(_pGestureRec->arrayAnnotationJson.size() > 0){

        //Reset all texts and buffers
        ui->textBrowserSub->clear();
        ui->lineBegin->clear();
        ui->lineEnd->clear();
        bufferSubmissions.str(std::string());

        ui->labelSaveState->setText("Saved!");
        ui->labelSaveState->setStyleSheet("background-color:green;");

        ui->buttonSubmit->setEnabled(false);
        ui->buttonSave->setEnabled(false);
    }
}

void GUIgesturerecognitionsave::syncSlider(){
    ui->sliderFrame->setSliderPosition(_pGestureRec->idxFrame);
}

void GUIgesturerecognitionsave::reset(){

    //Reset all texts and buffers
    ui->textBrowserSub->clear();
    ui->lineBegin->clear();
    ui->lineEnd->clear();
    bufferSubmissions.str(std::string());

    QJsonArray tmp;
    _pGestureRec->arrayAnnotationJson = tmp;


}



