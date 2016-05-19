#ifndef GESTURERECOGNITIONHMM_H
#define GESTURERECOGNITIONHMM_H

#pragma once

#define NCLASSES 10

#include <opencv2/opencv.hpp>
#include <QString>
#include <string>
#include <vector>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>

#include "gesturerecognition.h"
#include "frameset.h"
#include "gmmstd_hmm_GMM.h"
#include "gmmstd_gmm_tiny.h"

class GestureRecognitionHMM: public GestureRecognition {

public:

    // Constructor
    GestureRecognitionHMM();

    //Classifier
    void recognize(FrameSet &frame);

    std::string typeAction;

    //Getter
    cv::Mat& getFrameRGB();
    cv::Mat& getFrameDepth();
    cv::Mat& getDetection();
    cv::Mat& getStates();
    cv::Mat& getGestureStarted();
    cv::Mat& getRobustness();
    cv::Mat& getHisto();
    cv::Mat& getGesture();
    cv::Mat& getHistoBack();
    cv::Mat& getHistoPoint();
    cv::Mat& getHistoSl();
    cv::Mat& getHistoSr();
    cv::Mat& getHistoZoom();

    cv::Mat& getHistoZoomOut();
    cv::Mat& getHistoSU();
    cv::Mat& getHistoSD();
    cv::Mat& getHistoExit();
    cv::Mat& getHistoRot();

    QString getOutputBuffer();
    double getLevelRobustness();

    //Setter
    void resetOutputBuffer();


private:
    //VARIABILI
    int idx;
    tripletPoint t;
    cv::Mat black;
    cv::Mat preClass;
    cv::Mat okGesture;
    cv::Mat hmmState;
    cv::Mat histoLk;
    cv::Mat rob;

    cv::Mat histoBack;
    cv::Mat histoPoint;
    cv::Mat histoSl;
    cv::Mat histoSr;
    cv::Mat histoZoom;
    cv::Mat histoRot;
    cv::Mat histoSU;
    cv::Mat histoSD;
    cv::Mat histoExit;
    cv::Mat histoZoomOut;

    cv::Mat histoImages[NCLASSES];

    double minimumDist;
    double robustness;
    int contLabel;
    int position;
    double tmpMaxLk;
    bool ok;

    std::string outputBuffer;

    //Vettore di HMM
    std::vector<gmmstd::CHMM_GMM> vHMM;

    //Vettore con tipi HMM
    std::vector<string> vTypeHMM;

    //Vettore per accumulare i joint del singolo frame
    std::vector<tripletPoint> vJoint;

    //Vettore per accumulare le features del singolo frame
    std::vector<double> vFrameFeatures;

    //Vettore per accumulare tutte le features
    std::vector<vector<double>> vFeatures;

    int IdSkeletonCheck(cv::Mat1b framePlayer);
    void ChargeHmm(std::string path);
    //void GetDataSkeleton(int idx);
    void GetFeatures(FrameSet frame);
    std::string Classify();
    std::string PreClassify();
    void InsertText();
    bool CheckGesture(bool ok);
    std::string ConvertText(std::string text);
    bool CheckRobustness(double rob, std::string classification);
    bool CheckInstant(int position, std::vector<gmmstd::CHMM_GMM> vHMM);
    void AllRed(cv::Mat img);
    void ColorCircle(cv::Mat img, int position, cv::Scalar color);
    void ColorCircleSmall(cv::Mat img, int position, cv::Scalar color);
    void ColorLine(int position, cv::Scalar color);
    void UpdateHisto(int position, double value, cv::Scalar color);
    void UpdateHisto(std::vector<double> vValue);
    void UpdateHistoProb();
    void ResetHistoProb();
    bool CheckEnd();
    bool sendCommand(std::string command);

};

class CommandSender : public QObject
{
    Q_OBJECT

public:
    CommandSender (const QString &command){
        _command = command;
    }

    QString _command;



public slots:
    void doWork();

signals:
    void resultReady(const QString &result);
};

#endif // GESTURERECOGNITIONHMM_H
