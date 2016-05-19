#ifndef GESTURERECOGNITIONDHMM_H
#define GESTURERECOGNITIONDHMM_H

#pragma once

#include "gesturerecognition.h"
#include "frameset.h"
#include "msdhmm_hmm.h"

class GestureRecognitionDHMM: public GestureRecognition{

public:

    GestureRecognitionDHMM();

    //Classifier
    void recognize(FrameSet &frame);

    //Train
    virtual void Train(const TrainingSet &tr);

    //Test
    virtual void Test(const TestingSet &ts);

    std::string _pathScoresTest = "classification/test/";
    std::string _pathScoresTrain = "classification/train/";

};

#endif // GESTURERECOGNITIONDHMM_H
