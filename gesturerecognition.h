#ifndef GESTURERECOGNITION_H
#define GESTURERECOGNITION_H

#include "frameset.h"
#include <QObject>
#include "trainingset.h"
#include "testingset.h"

class GestureRecognition : public QObject{

    Q_OBJECT

public:

    //Constructor
    GestureRecognition();

    //Classify the gesture
    virtual void recognize(FrameSet &frame) = 0;

// public slots:
//    virtual void Load(const QString &Path)=0;
//    virtual void Save(const QString &filename)=0;
    virtual void Train(const TrainingSet &tr){;}

signals:

    void FrameProcessed();
    void WindowSizeSignal();
    //void classificationDone();




};

#endif // GESTURERECOGNITION_H
