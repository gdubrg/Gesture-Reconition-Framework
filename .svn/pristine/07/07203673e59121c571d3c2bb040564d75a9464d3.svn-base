#ifndef GESTURERECOGNITIONDTW_H
#define GESTURERECOGNITIONDTW_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <sstream>

#include "gesturerecognition.h"
#include "frameset.h"

static const double thresholdDTW = 2;
static const double thresholdLastFrame = 10;

class GestureRecognitionDTW: public GestureRecognition{

    Q_OBJECT

public:

    //Constructor
    GestureRecognitionDTW();
    ~GestureRecognitionDTW();

    //Classifier
    void recognize(FrameSet &frame);

    //Getter
    int getWindowSize();

    virtual void Train(const TrainingSet &tr);

    virtual void Test(const TestingSet &ts);

    //Vars-------------------------------------------
    std::string classificationString;

    //To store all gestures labels
    std::vector<std::string> allLabels;
    std::vector<int> allLabelsMSR;

    //Score of best DTW
    double bestDTW;

    //To store all scores
    std::vector<double> allScores;

private:

    //To store all joint coordinates per frame
    std::vector<tripletPoint> allJoint;

    //To store all frame
    std::vector<std::vector<tripletPoint>> allFrames;

    //To store all gestures of train
    std::vector<std::vector<std::vector<tripletPoint>>> allGestures;


    //Centre the data
    void centreNormData(std::vector<tripletPoint> &allJoint);

    //Get sequence
    void getWindowSequence(FrameSet &frame);

    //Computer the min DTW distance
    double dtw(std::vector<tripletPoint> seq1, std::vector<tripletPoint> seq2);

    //Compute Euclidean Distance
    double eucDist(tripletPoint a, tripletPoint b);

    //Computer Euclidean Distance between set of joints
    double eucDistJoints(std::vector<tripletPoint> set1, std::vector<tripletPoint> set2);

    //Load gesture file
    void loadFile(std::string filename);

    //DTW method
    double dtw(std::vector<std::vector<tripletPoint>> seq1, std::vector<std::vector<tripletPoint>> seq2);

    //MHI
    void mhi(FrameSet &frame, int nFrame);
    cv::Mat1f mhiImage;
    int nFrame = 0;

    //Evaluation
    int totRightClassified = 0;
    int totWrongClassified = 0;
    int totTest = 0;


signals:
    void classificationDone();

};

#endif // GESTURERECOGNITIONDTW_H
