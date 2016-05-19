#ifndef GESTURERECOGNITIONTEST_H
#define GESTURERECOGNITIONTEST_H

#pragma once

#include "gesturerecognition.h"
#include "frameset.h"

class GestureRecognitionTEST: public GestureRecognition
{
    Q_OBJECT

public:

    GestureRecognitionTEST();

    //Classifier
    void recognize(FrameSet &frame);

    //VAR---------------

    //Image for skeleton joints
    cv::Mat3b imageSkeleton;

    cv::Mat3b imageNew;

    tripletPoint Nold;
    bool firstChange = true;
    double shoulderRotation;

    //Number of frame
    int numFrame;

    //Frameset
    FrameSet _frame;

    //Old Coordinates of joints (last frame)
    std::vector<tripletPoint> jointsOld;

    //Joints with new coordinates
    std::vector<tripletPoint> jointsNewCoord;

    //Velocities of joints
    std::vector<double> vJoints;


private:
    void drawSkeleton();
    void drawHead();
    void UpdateVelocities();
    void changeCoord();

signals:
    void FrameAcquired();

};

#endif // GESTURERECOGNITIONTEST_H
