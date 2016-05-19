#ifndef GESTURERECOGNITIONHMMOFFLINE_H
#define GESTURERECOGNITIONHMMOFFLINE_H

#include "gesturerecognition.h"
#include "frameset.h"
#include "gmmstd_hmm_GMM.h"
#include "gmmstd_gmm_tiny.h"

class GestureRecognitionHMMOffline: public GestureRecognition{

public:

    GestureRecognitionHMMOffline();

    //Classifier
    void recognize(FrameSet &frame);

    //HMM TRAIN
    virtual void Train(const TrainingSet &tr);

    //HMM TEST
    virtual void Test(const TestingSet &ts);


};

#endif // GESTURERECOGNITIONHMMOFFLINE_H
