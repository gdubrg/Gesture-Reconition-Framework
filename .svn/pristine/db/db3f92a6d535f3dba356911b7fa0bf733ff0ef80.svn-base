#include <strsafe.h>

#include "acquisitionkinect1.h"
#include "resource.h"
#include "stdafx.h"
#include "frameset.h"

using namespace std;
using namespace cv;


//Constructor
AcquisitionKinect1::AcquisitionKinect1():
    m_hNextColorFrameEvent(INVALID_HANDLE_VALUE),
    m_pColorStreamHandle(INVALID_HANDLE_VALUE),
    m_hProcessStopEvent(NULL),
    m_pNuiSensor(NULL)

{

    cout << "Preparing Kinect 1 connection..." << endl;
    CreateFirstConnected();
    cout << "Kinect connected!" << endl;


    DWORD width, height;
    m_frameHelper.GetColorFrameSize(&width, &height);
    Size size(width, height);
    colorMat.create(size, m_frameHelper.COLOR_TYPE);

    DWORD width_d, height_d;
    m_frameHelper.GetDepthFrameSize(&width_d, &height_d);
    Size size_d(width_d, height_d);
    depthMat.create(size_d, m_frameHelper.DEPTH_RGB_TYPE);

    playerIdxMat=Mat_<char>(size_d);


}



//Destructor
AcquisitionKinect1::~AcquisitionKinect1(){

    if (m_pNuiSensor)
    {
        m_pNuiSensor->NuiShutdown();
    }

    if (m_hNextColorFrameEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hNextColorFrameEvent);
    }

    SafeRelease(m_pNuiSensor);

}


//Get next frame
void AcquisitionKinect1::getFrame(FrameSet &frame){

    frame.kinectVersion = 1;

    // Initialize array of events to wait for
    HANDLE hEvents[4] = { m_hProcessStopEvent, NULL, NULL, NULL };
    int numEvents;
    if (m_frameHelper.IsInitialized())
    {
        m_frameHelper.GetColorHandle(hEvents + 1);
        m_frameHelper.GetDepthHandle(hEvents + 2);
        m_frameHelper.GetSkeletonHandle(hEvents + 3);
        numEvents = 4;
    }
    else
    {
        numEvents = 1;
    }

    int eventId = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

    bool skeletonAcquired = false, colorAcquired = false, depthAcquired = false;

    if (eventId != WAIT_TIMEOUT && eventId != WAIT_OBJECT_0){

        while (!skeletonAcquired || !colorAcquired || !depthAcquired){

            // Update skeleton frame
            if (SUCCEEDED(m_frameHelper.UpdateSkeletonFrame())&&!skeletonAcquired)
            {
                m_frameHelper.GetSkeletonFrame(&skeletonFrame);
                skeletonAcquired = true;
            }

            // Update color frame
            if (SUCCEEDED(m_frameHelper.UpdateColorFrame())&&!colorAcquired)
            {
                m_frameHelper.GetColorImage(&colorMat);
                colorAcquired = true;
            }

            // Update depth frame
            if (SUCCEEDED(m_frameHelper.UpdateDepthFrame()) && !depthAcquired && skeletonAcquired)
            {

                HRESULT hr = m_frameHelper.GetDepthImageAsArgb(&depthMat, &playerIdxMat);
                hr = m_openCVHelper.DrawSkeletonsInDepthImage(&depthMat, &skeletonFrame, NUI_IMAGE_RESOLUTION_640x480);
                depthAcquired = true;
            }

        }

        //RGB Frame
        frame.RGB = colorMat;

        //Depth Frame
        frame.depth = depthMat;

        //Frame with Player
        frame.playerIdx = playerIdxMat;

        //Find player idx
        frame.idx = IdSkeletonCheck(playerIdxMat);

        int idxP = frame.idx;

        //Body Joints
        if(frame.idx >= 0){

            //MAPPING KINECT1 -> FRAMESET

            //Head
            frame.bodyJoint[FS_head].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x;
            frame.bodyJoint[FS_head].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y;
            frame.bodyJoint[FS_head].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z;

            //Shoulder Center
            frame.bodyJoint[FS_shoulderCenter].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;
            frame.bodyJoint[FS_shoulderCenter].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y;
            frame.bodyJoint[FS_shoulderCenter].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z;

            //Shoulder Left
            frame.bodyJoint[FS_shoulderLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].x;
            frame.bodyJoint[FS_shoulderLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].y;
            frame.bodyJoint[FS_shoulderLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].z;

            //Elbow Left
            frame.bodyJoint[FS_elbowLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x;
            frame.bodyJoint[FS_elbowLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y;
            frame.bodyJoint[FS_elbowLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z;

            //Wrist Left
            frame.bodyJoint[FS_wristLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x;
            frame.bodyJoint[FS_wristLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y;
            frame.bodyJoint[FS_wristLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z;

            //Hand Left
            frame.bodyJoint[FS_handLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x;
            frame.bodyJoint[FS_handLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y;
            frame.bodyJoint[FS_handLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z;

            //Shoulder Right
            frame.bodyJoint[FS_shoulderRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x;
            frame.bodyJoint[FS_shoulderRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
            frame.bodyJoint[FS_shoulderRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z;

            //Elbow Right
            frame.bodyJoint[FS_elbowRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
            frame.bodyJoint[FS_elbowRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
            frame.bodyJoint[FS_elbowRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z;

            //Wrist Right
            frame.bodyJoint[FS_wristRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x;
            frame.bodyJoint[FS_wristRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y;
            frame.bodyJoint[FS_wristRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z;

            //Hand Right
            frame.bodyJoint[FS_handRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
            frame.bodyJoint[FS_handRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
            frame.bodyJoint[FS_handRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;

            //Spine
            frame.bodyJoint[FS_spine].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SPINE].x;
            frame.bodyJoint[FS_spine].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SPINE].y;
            frame.bodyJoint[FS_spine].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_SPINE].z;

            //Hip Center
            frame.bodyJoint[FS_hipCenter].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x;
            frame.bodyJoint[FS_hipCenter].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y;
            frame.bodyJoint[FS_hipCenter].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z;

            //Hip Left
            frame.bodyJoint[FS_hipLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x;
            frame.bodyJoint[FS_hipLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y;
            frame.bodyJoint[FS_hipLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z;

            //Knee Left
            frame.bodyJoint[FS_kneeLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x;
            frame.bodyJoint[FS_kneeLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y;
            frame.bodyJoint[FS_kneeLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z;

            //Ankle Left
            frame.bodyJoint[FS_ankleLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].x;
            frame.bodyJoint[FS_ankleLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].y;
            frame.bodyJoint[FS_ankleLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].z;

            //Foot Left
            frame.bodyJoint[FS_footLeft].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].x;
            frame.bodyJoint[FS_footLeft].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y;
            frame.bodyJoint[FS_footLeft].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].z;

            //Hip Right
            frame.bodyJoint[FS_hipRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x;
            frame.bodyJoint[FS_hipRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y;
            frame.bodyJoint[FS_hipRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z;

            //Knee Right
            frame.bodyJoint[FS_kneeRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x;
            frame.bodyJoint[FS_kneeRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y;
            frame.bodyJoint[FS_kneeRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z;

            //Ankle Right
            frame.bodyJoint[FS_ankleRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].x;
            frame.bodyJoint[FS_ankleRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].y;
            frame.bodyJoint[FS_ankleRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].z;

            //Foot Right
            frame.bodyJoint[FS_footRight].x = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].x;
            frame.bodyJoint[FS_footRight].y = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y;
            frame.bodyJoint[FS_footRight].z = skeletonFrame.SkeletonData[idxP].SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].z;

        }

    }

}

int AcquisitionKinect1::IdSkeletonCheck(Mat1b framePlayer){
    int idx = -1;
    for(int r=0;r<framePlayer.rows;++r){
        for(int c=0;c<framePlayer.cols;++c){
            if(framePlayer(r,c) > 0){
                idx = framePlayer(r,c);
                break;
            }
        }
    }
    return idx-1; //indice i si riferisce a id = idx -1
}



Mat AcquisitionKinect1::ProcessColor(){

    Mat RGB;

    HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;

    // Attempt to get the color frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pColorStreamHandle, 0, &imageFrame);
    if (FAILED(hr))
    {
        return RGB;
    }

    INuiFrameTexture * pTexture = imageFrame.pFrameTexture;
    NUI_LOCKED_RECT LockedRect;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);

    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {
        BYTE * pBuffer = (BYTE*)LockedRect.pBits;
        Mat imgtmp(480, 640, CV_8UC4, pBuffer);
        RGB = imgtmp;
    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    // Release the frame
    m_pNuiSensor->NuiImageStreamReleaseFrame(m_pColorStreamHandle, &imageFrame);

    return RGB;

}

Mat AcquisitionKinect1::ProcessDepth() {

    Mat depth;

    HRESULT hr;
    NUI_IMAGE_FRAME imageFrame;

    // Attempt to get the color frame
    hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &imageFrame);
    if (FAILED(hr))
    {
        return depth;
    }

    INuiFrameTexture* pTexture;

    // Get the depth image pixel texture
    hr = m_pNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(
        m_pDepthStreamHandle, &imageFrame, false, &pTexture);

    NUI_LOCKED_RECT LockedRect;

    // Lock the frame data so the Kinect knows not to modify it while we're reading it
    pTexture->LockRect(0, &LockedRect, NULL, 0);



    // Make sure we've received valid data
    if (LockedRect.Pitch != 0)
    {


        DWORD depthHeight, depthWidth;
        NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, depthWidth, depthHeight);

        Mat depthImage(depthHeight, depthWidth, CV_16U);


        // Copy image information into Mat
        USHORT* pBufferRun = reinterpret_cast<USHORT*>(pTexture);

        for (UINT y = 0; y < depthHeight; ++y)
        {
            // Get row pointer for depth Mat
            USHORT* pDepthRow = depthImage.ptr<USHORT>(y);

            for (UINT x = 0; x < depthWidth; ++x)
            {
                pDepthRow[x] = pBufferRun[y * depthWidth + x];
            }
        }

        depth = depthImage;
    }

    // We're done with the texture so unlock it
    pTexture->UnlockRect(0);

    // Release the frame
    m_pNuiSensor->NuiImageStreamReleaseFrame(m_pDepthStreamHandle, &imageFrame);

    return depth;

}

HRESULT AcquisitionKinect1::CreateFirstConnected(){
    INuiSensor * pNuiSensor = NULL;
    HRESULT hr;

    int iSensorCount = 0;
    hr = NuiGetSensorCount(&iSensorCount);
    if (FAILED(hr))
    {
        return hr;
    }

    // Look at each Kinect sensor
    for (int i = 0; i < iSensorCount; ++i)
    {
        // Create the sensor so we can check status, if we can't create it, move on to the next
        hr = NuiCreateSensorByIndex(i, &pNuiSensor);
        if (FAILED(hr))
        {
            continue;
        }

        hr = m_frameHelper.Initialize(pNuiSensor);

        // Get the status of the sensor, and if connected, then we can initialize it
        hr = pNuiSensor->NuiStatus();
        if (S_OK == hr)
        {
            m_pNuiSensor = pNuiSensor;
            break;
        }

        // This sensor wasn't OK, so release it since we're not using it
        pNuiSensor->Release();
    }

    if (NULL != m_pNuiSensor)
    {
        // Initialize the Kinect and specify that we'll be using color, depth and skeleton
        hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);

        if (SUCCEEDED(hr))
        {
            // Create an event that will be signaled when depth data is available
            m_hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            // Open a color image stream to receive depth frames
            hr = m_pNuiSensor->NuiImageStreamOpen(
                        NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
                        NUI_IMAGE_RESOLUTION_640x480,
                        0,
                        2,
                        m_hNextDepthFrameEvent,
                        &m_pDepthStreamHandle);

            if (FAILED(hr)) return hr;

            // Create an event that will be signaled when color data is available
            m_hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            // Open a color image stream to receive color frames
            hr = m_pNuiSensor->NuiImageStreamOpen(
                        NUI_IMAGE_TYPE_COLOR,
                        NUI_IMAGE_RESOLUTION_640x480,
                        0,
                        2,
                        m_hNextColorFrameEvent,
                        &m_pColorStreamHandle);

            if (FAILED(hr)) return hr;

        }
    }

    return hr;
}

void AcquisitionKinect1::UnInitialize(){

    // Close Kinect
    if (m_pNuiSensor)
    {
        m_pNuiSensor->NuiShutdown();
        m_pNuiSensor = NULL;
    }

    // Close handles for created events
    if (m_hNextColorFrameEvent && (m_hNextColorFrameEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(m_hNextColorFrameEvent);
        m_hNextColorFrameEvent = NULL;
    }

    if (m_hNextDepthFrameEvent && (m_hNextDepthFrameEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(m_hNextDepthFrameEvent);
        m_hNextDepthFrameEvent = NULL;
    }
    /*
    if (m_hNextSkeletonFrameEvent && (m_hNextSkeletonFrameEvent != INVALID_HANDLE_VALUE))
    {
        CloseHandle(m_hNextSkeletonFrameEvent);
        m_hNextSkeletonFrameEvent = NULL;
    }*/

}
