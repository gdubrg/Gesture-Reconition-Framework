#ifndef GESTURERECOGNITIONSAVE_H
#define GESTURERECOGNITIONSAVE_H

#include "gesturerecognition.h"
#include "frameset.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QKeyEvent>

class GestureRecognitionSave: public GestureRecognition
{
    Q_OBJECT

public:

    GestureRecognitionSave();

    //Classifier
    void recognize(FrameSet &frame);

    //Var
    bool acquisition = false;
    std::vector<FrameSet> vFrames;
    std::string filename;
    int savingState = 0;
    std::vector<cv::Mat3b> vRGBFrames;
    int openingState = 0;
    int openingSize = 0;
    int idxFrame;

    //For json annotation
    int begin;
    int end;
    QString typeAction;

    //Json document for annotations
    QJsonDocument documentAnnotationJson;
    //Json array for annotations
    QJsonArray arrayAnnotationJson;
    //Json object for annotation details
    QJsonObject objectAnnotationJson;

    std::string currentType;
    std::string currentFilename;

    void saveToFile();
    void openData(QJsonObject objectJson);
    void keyPressEvent(QKeyEvent *event);

    //Setter
    void setFilename(std::string);
    void setDataSubmission(int lineBegin, int lineEnd, std::string current);

public slots:
    void startRec();
    void stopRec();
    void chooseFile();
    void fwd();
    void fwd(int);
    void bck();
    void appendAnnotation();
    void createAnnotation();

private:


signals:
    void acquisitionCompleted();
    void frameArrived();
    void saving();
    void opening();
    void handleFrame();

};

#endif // GESTURERECOGNITIONSAVE_H
