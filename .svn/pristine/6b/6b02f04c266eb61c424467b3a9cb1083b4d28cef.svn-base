#ifndef ACQUISITIONKINECT1_H
#define ACQUISITIONKINECT1_H

#include <opencv2\opencv.hpp>

#include "OpenCVFrameHelper.h"
#include "OpenCVHelper.h"
#include "acquisition.h"
#include "frameset.h"


class AcquisitionKinect1: public Acquisition{

    static const int        cColorWidth  = 640;
    static const int        cColorHeight = 480;

public:

    // Constructor
    AcquisitionKinect1();

    // Destructor
    ~AcquisitionKinect1();

    //Get 1 frame from Kinect 1 (override)
    void getFrame(FrameSet &frameset);

    //?
    void UnInitialize();


private:

    // Current Kinect
    INuiSensor* m_pNuiSensor;

    HANDLE m_hProcessStopEvent;

    HANDLE m_pColorStreamHandle;
    HANDLE m_hNextColorFrameEvent;

    HANDLE m_pDepthStreamHandle;
    HANDLE m_hNextDepthFrameEvent;

    HRESULT	CreateFirstConnected();

    // Handle new color data
    cv::Mat ProcessColor();

    // Handle new depth data
    cv::Mat ProcessDepth();

    int IdSkeletonCheck(Mat1b framePlayer);

    Microsoft::KinectBridge::OpenCVFrameHelper m_frameHelper;
    OpenCVHelper m_openCVHelper;

    cv::Mat						colorMat;
    cv::Mat						depthMat;
    NUI_SKELETON_FRAME			skeletonFrame;
    cv::Mat1b					playerIdxMat;
};


#endif // ACQUISITIONKINECT1_H
