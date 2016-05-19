#include <qtimer.h>
#include <qvalidator.h>
#include <chrono>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conversionutility.h"

#include "guigesturerecognitionhmm.h"
#include "guigesturerecognitiondtw.h"
#include "guigesturerecognitiontest.h"
#include "guigesturerecognitionsave.h"
#include "guitestmyo.h"


using namespace std;
using namespace std::chrono;
using namespace cv;

int totFrame;
double totTime;

MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //-----------------------------------------------GUI-----------------------------------

    ui->setupUi(this);
    setFixedSize(this->geometry().width(), this->geometry().height());

    // collegamento actions e altro
    //connect(ui->actionBatchProcess1,SIGNAL(triggered()),this, SLOT(BatchProcess1()));
    connect(ui->actionRun,SIGNAL(triggered(bool)),this, SLOT(Run(bool)));
    connect(ui->actionClose,SIGNAL(triggered(bool)),this, SLOT(close()));
    connect(ui->buttonStart, SIGNAL(released()), this, SLOT(doAll()));
    connect(ui->buttonStart, SIGNAL(released()), this, SLOT(resetGUI()));

    //--------------------------------------------FPS---------------------------------

    totFrame = 0;
    totTime = 0;

    //--------------------------------------------Timer-------------------------------
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processStep()));

    _pkinect = nullptr;



}

void::MainWindow::doAll(){

    // uccido eventuali kinect precedenti
    if (_pkinect)
        delete _pkinect;
    _pkinect = nullptr;

    //----------------------------DEVICES-----------------------------------


    // Kinect 1
    if(ui->radioKinect1->isChecked()){
        _pkinect = new AcquisitionKinect1();
    }

    // Kinect 2
    if(ui->radioKinect2->isChecked()){
        _pkinect = new AcquisitionKinect2();
        _pkinect->setSkeleton(MODE_CLOSEST);
        _pkinect->setResolution(RESOLUTION_VGA);
    }

    // Myo
//    hub("com.example.hello-myo");
//    cout << "Attempting to find a Myo..." << endl;
//    myo::Myo* myo = hub.waitForMyo(10000);
//    if(!myo)
//        cout << "Unable to find a Myo..." << endl;
//    cout << "Connected to a Myo armband!" << endl;
//    //DataCollector collector;
//    //hub.addListener(&collector);
    if(ui->radioMyo->isChecked()){
        _pmyo = new AcquisitionMyo();
    }

    // File
    if(ui->radioFile->isChecked())
        _pkinect =  new AcquisitionFile();

    //-----------------------------------METHODS--------------------------------------

    // HMM
    if(ui->radioHMM->isChecked()){
        _precognizer =  new GestureRecognitionHMM();
        GUIgesturerecognitionhmm *pGUI = new GUIgesturerecognitionhmm (dynamic_cast<GestureRecognitionHMM*>(_precognizer));
        pGUI->setParent(ui->widgetL);
        pGUI->show();
    }

    // DTW
    if(ui->radioDTW->isChecked()){
        _precognizer = new GestureRecognitionDTW();
        GUIgesturerecognitiondtw *pGUI = new GUIgesturerecognitiondtw (dynamic_cast<GestureRecognitionDTW*>(_precognizer));
        pGUI->setParent(ui->widgetL);
        pGUI->show();
    }


    //---------------------------------UTILITY-------------------------------------

    // Test
    if(ui->radioTest->isChecked()){
        _precognizer = new GestureRecognitionTEST();
        GUIgesturerecognitiontest *pGUI = new GUIgesturerecognitiontest (dynamic_cast<GestureRecognitionTEST*>(_precognizer));
        pGUI->setParent(ui->widgetL);
        pGUI->show();
    }

    // Save Annotation
    if(ui->radioSave->isChecked()){
        _precognizer = new GestureRecognitionSave();
        GUIgesturerecognitionsave *pGUI = new GUIgesturerecognitionsave (dynamic_cast<GestureRecognitionSave*>(_precognizer));
        pGUI->setParent(ui->widgetL);
        pGUI->show();
    }

    // Test MYO
    if(ui->radioTestMyo->isChecked()){
        _precognizer = new TestMyo();
        GUItestMyo *pGUI = new GUItestMyo (dynamic_cast<TestMyo*>(_precognizer));
        pGUI->setParent(ui->widgetL);
        pGUI->show();
    }

    ui->widgetL->setVisible(true);

    // Timer
    timer->start(30);

}

void::MainWindow::resetGUI(){
    ui->groupSource->hide();
    ui->groupMethod->hide();
    ui->buttonStart->hide();
}

void MainWindow::close(){

    ui->widgetL->setVisible(false);
    qDeleteAll(ui->widgetL->children());

    ui->groupSource->setVisible(true);
    ui->groupMethod->setVisible(true);
    ui->buttonStart->setVisible(true);

    ui->radioKinect1->setAutoExclusive(false);
    ui->radioKinect1->setChecked(false);
    ui->radioKinect1->setAutoExclusive(true);

    ui->radioKinect2->setAutoExclusive(false);
    ui->radioKinect2->setChecked(false);
    ui->radioKinect2->setAutoExclusive(true);

    ui->radioFile->setAutoExclusive(false);
    ui->radioFile->setChecked(false);
    ui->radioFile->setAutoExclusive(true);

    ui->radioHMM->setAutoExclusive(false);
    ui->radioHMM->setChecked(false);
    ui->radioHMM->setAutoExclusive(true);

    ui->radioDTW->setAutoExclusive(false);
    ui->radioDTW->setChecked(false);
    ui->radioDTW->setAutoExclusive(true);

    ui->radioTest->setAutoExclusive(false);
    ui->radioTest->setChecked(false);
    ui->radioTest->setAutoExclusive(true);

    ui->radioSave->setAutoExclusive(false);
    ui->radioSave->setChecked(false);
    ui->radioSave->setAutoExclusive(true);

    Mat frameBlack;
    frameBlack = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    QPixmap pixBlack = cvMatToQPixmap(frameBlack);
    ui->labelRGB->setPixmap(pixBlack.scaled(ui->labelRGB->size(), Qt::IgnoreAspectRatio));

    Mat depthBlack;
    depthBlack = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    QPixmap depthpix= cvMatToQPixmap(depthBlack);
    ui->labelDepth->setPixmap(depthpix.scaled(ui->labelDepth->size(), Qt::IgnoreAspectRatio));

    ui->labelTimeAcq->setText("");
    ui->labelTimeElab->setText("");
    ui->labelTotal->setText("");
    ui->labelFPS->setText("");

    timer->stop();
}


void MainWindow::Run(bool brun){
    if (timer!= nullptr){
       if (brun)
           timer->start(30);
       else
           timer->stop();
    }
}

void MainWindow::processStep(){

    high_resolution_clock timer;
    using ms = duration<float, std::milli>;


    //Get next frame KINECT
    auto start = timer.now();
    if(_pkinect != nullptr)
        _pkinect->getFrame(frame);
    auto stop = timer.now();

    //GEt next frame MYO
    if(_pmyo != nullptr){
        _pmyo->getFrame(frame);
    }

    //if(frame.RGB.cols > 0)
        totFrame++;


    auto deltaTime = duration_cast<ms>(stop - start).count();
    //totTime += deltaTime;

    //Try to classify
    start = timer.now();
    if(_precognizer != nullptr)
        _precognizer->recognize(frame);
    stop = timer.now();

    auto deltaTimeRec = duration_cast<ms>(stop - start).count();

    totTime = totTime + deltaTime + deltaTimeRec;
    if((deltaTime + deltaTimeRec) > 30){
        totTime += 30;
    }

    double fps = (totFrame/totTime)*1000;
    //cout << (totFrame/totTime)*1000 << endl;
    //cout << totFrame << endl;

    //Update main GUI (RGB + DEPTH)
    Mat& frameRGB = frame.RGB;
    Mat& frameDepth = frame.depth;

    QPixmap pixFrameRGB = cvMatToQPixmap(frameRGB);
    QPixmap pixFrameDepth = cvMatToQPixmap(frameDepth);

    if(pixFrameRGB.isNull())
        ui->labelRGB->setText("Image Null");
    else
        ui->labelRGB->setPixmap(pixFrameRGB.scaled(ui->labelRGB->size(), Qt::IgnoreAspectRatio));

    if(pixFrameDepth.isNull())
        ui->labelDepth->setText("Image Null");
    else
        ui->labelDepth->setPixmap(pixFrameDepth.scaled(ui->labelDepth->size(), Qt::IgnoreAspectRatio));


    if(totFrame%2 == 0){
        ui->labelTimeAcq->setText(QString::number(deltaTime));
        ui->labelTimeElab->setText(QString::number(deltaTimeRec));
        ui->labelTotal->setText(QString::number(deltaTime+deltaTimeRec));
        ui->labelFPS->setText(QString::number((int)fps));
    }


}

MainWindow::~MainWindow()
{
    // uccido _pkinect
    delete _pkinect;_pkinect=nullptr;

    delete ui;
}
