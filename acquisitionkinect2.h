#ifndef ACQUISITIONKINECT2_H
#define ACQUISITIONKINECT2_H

#include <opencv2/opencv.hpp>
#include <QObject>

#include "Kinect.h"
#include "Kinect.Face.h"
#include "acquisition.h"
#include "frameset.h"
#include "stdafx.h"

// define the face frame features required to be computed by this application
static const DWORD c_FaceFrameFeatures =
//    FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
//    | FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
     FaceFrameFeatures::FaceFrameFeatures_RotationOrientation;
//    | FaceFrameFeatures::FaceFrameFeatures_Happy
//    | FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
//    | FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
//    | FaceFrameFeatures::FaceFrameFeatures_MouthOpen
//    | FaceFrameFeatures::FaceFrameFeatures_MouthMoved
//    | FaceFrameFeatures::FaceFrameFeatures_LookingAway
//    | FaceFrameFeatures::FaceFrameFeatures_Glasses
//    | FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;

using namespace cv;

class AcquisitionKinect2: public Acquisition {

    static const int        cDepthWidth  = 512;
    static const int        cDepthHeight = 424;

public:

    // Constructor
    AcquisitionKinect2();

    // Destructor
    virtual ~AcquisitionKinect2();

    // Get 1 frame from Kinect 2 (override)
    void getFrame(FrameSet &frame);

    // Process depth data
    void ProcessDepth(INT64 nTime, const UINT16* pBuffer, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);

    // Get kinect reader pointers (COLOR, DEPTH, BODY)
    IColorFrameReader*	GetColorReader(){ InitColorReader(); return m_pColorFrameReader;}
    IDepthFrameReader*	GetDepthReader(){InitDepthReader(); return m_pDepthFrameReader;}
    IBodyFrameReader*	GetBodyReader(){InitBodyReader(); return m_pBodyFrameReader;}
    IBodyIndexFrameReader* GetBodyIndexReader(){InitIndexBodyReader(); return m_pBodyIndexFrameReader;}
    //IFaceFrameReader * GetFaceReader(int id){InitFaceReader(id); return m_pFaceFrameReaders[id];}

protected:

    // Initialize readers
    void InitColorReader();
    void InitDepthReader();
    void InitBodyReader();
    void InitIndexBodyReader();
    void InitFaceReader(int id);

    //To store data acquired
    void storeCoordDepth(FrameSet& frame, cv::Point2d* jointPoints);
    void storeCoordRGB(FrameSet& frame, cv::Point2d* jointPointsRGB);
    void storeCoordReal(FrameSet& frame, Joint* joints);

    // Check if Kinect 2 is active and open
    void CheckKinect();

    IKinectSensor* GetSensor(){return m_pKinectSensor;}
    IKinectSensor* m_pKinectSensor;
    IColorFrameReader *	m_pColorFrameReader;
    IDepthFrameReader *	m_pDepthFrameReader;
    IBodyFrameReader *	m_pBodyFrameReader;
    IBodyIndexFrameReader *m_pBodyIndexFrameReader;

    //Face recognizer
    IFaceFrameReader *	m_pFaceFrameReaders[BODY_COUNT];
    IFaceFrameSource *  m_pFaceFrameSources[BODY_COUNT];

    //To extract angles in degree
    void ExtractFaceRotationInDegrees(Vector4 quaternion);
    int _pitch;
    int _yaw;
    int _roll;

    // Var
    cv::Ptr<BYTE> m_pColorRGBX;
    cv::Ptr<UINT16> m_pDepth;
    bool m_bValid;


private:

    //drawing---------------------------------------------
    ID2D1HwndRenderTarget*  m_pRenderTarget;
    ID2D1SolidColorBrush*   m_pBrushJointTracked;
    ID2D1SolidColorBrush*   m_pBrushJointInferred;
    ID2D1SolidColorBrush*   m_pBrushBoneTracked;
    ID2D1SolidColorBrush*   m_pBrushBoneInferred;
    ID2D1SolidColorBrush*   m_pBrushHandClosed;
    ID2D1SolidColorBrush*   m_pBrushHandOpen;
    ID2D1SolidColorBrush*   m_pBrushHandLasso;

    ICoordinateMapper*      m_pCoordinateMapper;

    cv::Point2d BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height);
    cv::Point2d BodyToScreenRGB(const CameraSpacePoint& bodyPoint, int width, int height);
    void DrawBone(const Joint* pJoints, const cv::Point2d* pJointPoints, JointType joint0, JointType joint1, Mat depth, bool selected);
    void DrawBody(const Joint* pJoints, const cv::Point2d* pJointPoints, Mat depth, bool selected);
    void DrawHand(HandState handState, cv::Point2d pJointPoints, Mat depth);

    int _bestDepthId;


};

#endif // ACQUISITIONKINECT2_H
