#include "acquisitionkinect2.h"
#include "frameset.h"

#include <strsafe.h>

#include "stdafx.h"

#define M_PI 3.14159265358979323846

static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;

using namespace std;
using namespace cv;

// Constructor
AcquisitionKinect2::AcquisitionKinect2():
    m_pCoordinateMapper(NULL),
    m_pRenderTarget(NULL),
    m_pBrushJointTracked(NULL),
    m_pBrushJointInferred(NULL),
    m_pBrushBoneTracked(NULL),
    m_pBrushBoneInferred(NULL),
    m_pBrushHandClosed(NULL),
    m_pBrushHandOpen(NULL),
    m_pBrushHandLasso(NULL) {

    // Initialize pointers
    m_pKinectSensor = 0;
    m_pColorFrameReader = 0;
    m_pDepthFrameReader = 0;
    m_pBodyFrameReader = 0;
    m_pBodyIndexFrameReader = 0;
    HRESULT hr;

    for(int i=0; i<BODY_COUNT;++i){
        m_pFaceFrameReaders[i] = nullptr;
        m_pFaceFrameSources[i] = nullptr;
    }

    //Initialize variables
    modeSkeleton = 0; //default: no skeleton selected


    hr = GetDefaultKinectSensor(&m_pKinectSensor);
    if (FAILED(hr))
    {
        m_bValid = false;
        cout << "Kinect 2.0 not found\n" << endl;
        return;
    }
    else
        cout << "Kinect 2.0 found\n" << endl;


    if (m_pKinectSensor)
    {
        cout << "Opening Kinect 2.0...\n" << endl;
        hr = m_pKinectSensor->Open();

    }

    // Wait until available...
    BOOLEAN bAvailable;
    bAvailable= false;
    for (int timeout=1; !bAvailable && timeout<50; timeout++)
    {
        m_pKinectSensor->get_IsAvailable(&bAvailable);
        Sleep(200);
    }

    if (bAvailable)
        cout << "Kinect available \n" << endl;
    else
        cout << "Kinect not available \n" << endl;

    if (!m_pKinectSensor || FAILED(hr))
    {

        cout << "Error opening Kinect 2.0 \n" << endl;
        m_bValid = false;
        return;
    }

    m_bValid = true;

    m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);


    //FACE reader (here, we can't initialize like others)
    if (SUCCEEDED(hr))
    {
        // create a face frame source + reader to track each body in the fov
        for (int i = 0; i < BODY_COUNT; i++)
        {
            if (SUCCEEDED(hr))
            {
                // create the face frame source by specifying the required face frame features
                hr = CreateFaceFrameSource(m_pKinectSensor, 0, c_FaceFrameFeatures, &m_pFaceFrameSources[i]);
            }
            if (SUCCEEDED(hr))
            {
                // open the corresponding reader
                hr = m_pFaceFrameSources[i]->OpenReader(&m_pFaceFrameReaders[i]);
            }
        }
    }
}

// Destructor
AcquisitionKinect2::~AcquisitionKinect2(){
    // done with color frame reader
    SafeRelease(m_pColorFrameReader);
    SafeRelease(m_pDepthFrameReader);
    SafeRelease(m_pBodyFrameReader);


    // close the Kinect Sensor
    if (m_pKinectSensor)
    {
        printf("closing Kinect 2.0\n");
        m_pKinectSensor->Close();
    }

    SafeRelease(m_pKinectSensor);
}

// Initialize COLOR reader
void AcquisitionKinect2::InitColorReader(){

    // init solo se non lo è già stato fatto
    if (m_pColorFrameReader)
        return ;


    HRESULT hr;

    m_pColorFrameReader= nullptr;
    IColorFrameSource* pColorFrameSource = NULL;
    if (!m_bValid) return;

    hr = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
    if (SUCCEEDED(hr))
    {

        hr = pColorFrameSource->OpenReader(&m_pColorFrameReader);
    }
    if (!SUCCEEDED(hr))
        printf("Error opening color frame source Kinect 2.0 \n");
    else
        printf("Color frame source opened! \n");

    SafeRelease(pColorFrameSource);
}

// Initialize DEPTH  reader
void AcquisitionKinect2::InitDepthReader(){

    if (m_pDepthFrameReader)
        return;

    HRESULT hr;

    m_pColorFrameReader= nullptr;
    IDepthFrameSource* pDepthFrameSource = NULL;
    if (!m_bValid) return;

    hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
    if (SUCCEEDED(hr))
    {
        hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
    }
    if (!SUCCEEDED(hr))
        printf("Error opening depth frame source Kinect 2.0 \n");
    else
        printf("Depth frame source opened! \n");

    SafeRelease(pDepthFrameSource);
}

// Initialize BODY reader
void AcquisitionKinect2::InitBodyReader()
{
    if (m_pBodyFrameReader)
        return;
    HRESULT hr;

    m_pBodyFrameReader = nullptr;
    IBodyFrameSource* pBodyFrameSource = NULL;
    if (!m_bValid) return;

    hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
    if (SUCCEEDED(hr))
    {
        hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
    }
    if (!SUCCEEDED(hr))
        printf("Error opening body frame source Kinect 2.0 \n");
    else
        printf("Body frame source opened! \n");

    SafeRelease(pBodyFrameSource);

}

// Initialize BODY INDEX reader
void AcquisitionKinect2::InitIndexBodyReader(){

    if (m_pBodyIndexFrameReader)
        return;
    HRESULT hr;
    m_pBodyIndexFrameReader = nullptr;
    IBodyIndexFrameSource* pBodyIndexFrameSource = NULL;
    if (!m_bValid) return;

    hr = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
    if (SUCCEEDED(hr))
    {
        hr = pBodyIndexFrameSource->OpenReader(&m_pBodyIndexFrameReader);
    }
    if (!SUCCEEDED(hr))
        printf("Error opening body index frame source Kinect 2.0 \n");
    else
        printf("Body index frame source opened! \n");

    SafeRelease(pBodyIndexFrameSource);
}

//Initialize FACE reader (all faces, use only the selected one)
void AcquisitionKinect2::InitFaceReader(int id){

    if (m_pFaceFrameReaders[id])
        return;

    IFaceFrameSource *  m_pFaceFrameSources[BODY_COUNT];

    for(int i=0; i<BODY_COUNT;++i)
        m_pFaceFrameSources[i] = nullptr;

    if(!m_bValid) return;

    HRESULT hr;

    for (int i = 0; i < BODY_COUNT; i++){

        if (SUCCEEDED(hr)) {
          // create the face frame source by specifying the required face frame features
          hr = CreateFaceFrameSource(m_pKinectSensor, 0, c_FaceFrameFeatures, &m_pFaceFrameSources[i]);
        }
        if (SUCCEEDED(hr)) {
            // open the corresponding reader
            hr = m_pFaceFrameSources[i]->OpenReader(&m_pFaceFrameReaders[i]);
        }

        if (SUCCEEDED(hr))
            printf("Face Reader opened! \n");

        //to do Controllare se corretto
        SafeRelease(m_pFaceFrameSources[i]);
    }

}

// Check if the Kinect 2 is active and open
void  AcquisitionKinect2::CheckKinect(){
    //
    BOOLEAN bris;
    if (m_pKinectSensor)
    {
        m_pKinectSensor->get_IsOpen(&bris);
        if (!bris)
        {	printf("Kinect close. Opening \n");
            m_pKinectSensor->Open();

            m_pKinectSensor->get_IsAvailable(&bris);
            if (!bris)
            {
                printf("Kinect not available \n");

            }
        }
    }
}

// Converts a body point to screen space (DEPTH)
cv::Point2d AcquisitionKinect2::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height)
{
    // Calculate the body's position on the screen
    DepthSpacePoint depthPoint = {0};
    m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

    float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
    float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;

    return cv::Point2d (screenPointX, screenPointY);
}

// Converts a body point to screen space (RGB)
cv::Point2d AcquisitionKinect2::BodyToScreenRGB(const CameraSpacePoint& bodyPoint, int width, int height)
{
    // Calculate the body's position on the screen
    ColorSpacePoint colorPoint = {0};
    //ICoordinateMapper* ris = GetCoordinateMapper();
    //ris->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);
    m_pCoordinateMapper->MapCameraPointToColorSpace(bodyPoint, &colorPoint);

    float screenPointX = static_cast<float>(colorPoint.X * width) / 1980;
    float screenPointY = static_cast<float>(colorPoint.Y * height) / 1020;

    return cv::Point2d (screenPointX, screenPointY);
}

// Draws one bone of a body
void AcquisitionKinect2::DrawBone(const Joint* pJoints, const Point2d* pJointPoints, JointType joint0, JointType joint1, Mat depth, bool selected)
{

    TrackingState joint0State = pJoints[joint0].TrackingState;
    TrackingState joint1State = pJoints[joint1].TrackingState;

    // If we can't find either of these joints, exit
    if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
    {
        return;
    }

    // Don't draw if both points are inferred
    if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
    {
        return;
    }

    // We assume all drawn bones are inferred unless BOTH joints are tracked
    if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
    {
        if(selected)
            cv::line(depth,pJointPoints[joint0], pJointPoints[joint1], CV_RGB(0, 255, 0), 5);
        else
            cv::line(depth,pJointPoints[joint0], pJointPoints[joint1], CV_RGB(255, 178, 102), 5);
    }
    else
    {
        if(selected)
            cv::line(depth,pJointPoints[joint0], pJointPoints[joint1], CV_RGB(255, 255, 255), 5);
        else
            cv::line(depth,pJointPoints[joint0], pJointPoints[joint1], CV_RGB(255, 178, 102), 5);
    }
}

// Draws a body
void AcquisitionKinect2::DrawBody(const Joint* pJoints, const Point2d* pJointPoints, Mat depth, bool selected)
{
    // Draw the bones

    // Torso
    DrawBone(pJoints, pJointPoints, JointType_Head, JointType_Neck,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft,depth, selected);

    // Right Arm
    DrawBone(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight,depth, selected);

    // Left Arm
    DrawBone(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft,depth, selected);

    // Right Leg
    DrawBone(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight,depth, selected);

    // Left Leg
    DrawBone(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft,depth, selected);
    DrawBone(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft,depth, selected);


    // Draw the joints
    for (int i = 0; i < JointType_Count; ++i)
    {

        //* D2D1_ELLIPSE ellipse = D2D1::Ellipse(pJointPoints[i], c_JointThickness, c_JointThickness);

        if (pJoints[i].TrackingState == TrackingState_Inferred)
        {
            if(selected)
                cv::circle(depth,pJointPoints[i],5,CV_RGB(192, 192, 192),CV_FILLED);
            else
                cv::circle(depth,pJointPoints[i],5,CV_RGB(255, 178, 102),CV_FILLED);
        }
        else if (pJoints[i].TrackingState == TrackingState_Tracked)
        {
            if(selected)
                cv::circle(depth,pJointPoints[i],5,CV_RGB(0, 200, 0),CV_FILLED);
            else
                cv::circle(depth,pJointPoints[i],5,CV_RGB(255, 178, 102),CV_FILLED);
        }


    }

}

//Draw Hand State
void AcquisitionKinect2::DrawHand(HandState handState, Point2d pJointPoints, Mat depth){



    if( (pJointPoints.x-30 > 0) && (pJointPoints.x+30 < depth.cols) && (pJointPoints.y-30 > 0) && (pJointPoints.y+30 < depth.rows)){

        Mat roi = depth(Rect(pJointPoints.x-30, pJointPoints.y-30, 60, 60));

        double alpha = 0.3;


        if(handState == HandState_Closed){
            Mat colorR(roi.size(), CV_8UC3, cv::Scalar(0, 0, 255));
            addWeighted(colorR, alpha, roi, 1.0 - alpha , 0.0, roi);
        }
        else if(handState == HandState_Open){
            Mat colorG(roi.size(), CV_8UC3, cv::Scalar(0, 255, 0));
            addWeighted(colorG, alpha, roi, 1.0 - alpha , 0.0, roi);
        }
        else  if(handState == HandState_Lasso){
            Mat colorW(roi.size(), CV_8UC3, cv::Scalar(255, 255, 255));
            addWeighted(colorW, alpha, roi, 1.0 - alpha , 0.0, roi);
        }
    }
}

// Get next frame from Kinect 2
void AcquisitionKinect2::getFrame(FrameSet &frame){

    //Kinect 2
    frame.kinectVersion = 2;

    // Default value for player idx
    frame.idx = -1;

    // READER COLOR
    IColorFrameReader* pColorFrameReader = GetColorReader();
    if (pColorFrameReader)
    {
        IColorFrame* pColorFrame = NULL;
        HRESULT hr = pColorFrameReader->AcquireLatestFrame(&pColorFrame);
        if (SUCCEEDED(hr))
        {
            INT64 nTime = 0;
            IFrameDescription* pFrameDescription = NULL;
            int nWidth = 0;
            int nHeight = 0;
            ColorImageFormat imageFormat = ColorImageFormat_None;
            UINT nBufferSize = 0;

            hr = pColorFrame->get_FrameDescription(&pFrameDescription);
            if (SUCCEEDED(hr))
            {
                hr = pFrameDescription->get_Width(&nWidth);
            }

            if (SUCCEEDED(hr))
            {
                hr = pFrameDescription->get_Height(&nHeight);
            }

            if (SUCCEEDED(hr))
            {
                hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
            }

            if (SUCCEEDED(hr))
            {


                if (imageFormat == ColorImageFormat_Bgra)
                {
                    hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&m_pColorRGBX));
                }
                else
                {
                    if (m_pColorRGBX.empty())
                    {
                        // allocate
                        m_pColorRGBX = new BYTE [nWidth*nHeight * sizeof(RGBQUAD)];
                    }


                    nBufferSize = nWidth * nHeight * sizeof(RGBQUAD);
                    hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, m_pColorRGBX, ColorImageFormat_Bgra);
                }

            }

            if (SUCCEEDED(hr))
            {
                // conversion to mat
                Mat ris2 = Mat(nHeight,nWidth, CV_8UC4, m_pColorRGBX, Mat::AUTO_STEP).clone();

                if(resolution == 1)
                    //VGA
                    resize(ris2, ris2, Size(), 0.3, 0.3, INTER_AREA);

                cvtColor(ris2,ris2,CV_BGRA2BGR);
                frame.RGB = ris2.clone();

            }
            SafeRelease(pFrameDescription);
        }
        SafeRelease(pColorFrame);
    }


    // READER DEPTH
    IDepthFrameReader * pDepthFrameReader = GetDepthReader();
    if (pDepthFrameReader)
    {
        IDepthFrame * pDepthFrame = NULL;
        HRESULT hr = pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
        if (SUCCEEDED(hr))
        {
            IFrameDescription* pFrameDescription = NULL;
            int nWidth = 0;
            int nHeight = 0;
            UINT nBufferSize = 0;

            hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
            if (SUCCEEDED(hr))
            {
                hr = pFrameDescription->get_Width(&nWidth);
            }

            if (SUCCEEDED(hr))
            {
                hr = pFrameDescription->get_Height(&nHeight);
            }


            if (SUCCEEDED(hr))
            {
                if (m_pDepth.empty())
                {
                    // allocate
                    //m_pDepth = new UINT16 [nWidth*nHeight * sizeof(UINT16)];
                    m_pDepth = new UINT16 [nWidth*nHeight];
                }

                //nBufferSize = nWidth * nHeight * sizeof(UINT16);
                nBufferSize = nWidth * nHeight;

                hr = pDepthFrame->CopyFrameDataToArray(nBufferSize,m_pDepth);

            }


            if (SUCCEEDED(hr))
            {
                // conversion to mat
                Mat ris2 = Mat(nHeight,nWidth, CV_16UC1, m_pDepth, Mat::AUTO_STEP).clone();

                Mat3b risDef (nHeight,nWidth);

                int x,y;
                for (y=0; y<nHeight; y++)
                    for (x=0; x<nWidth ;x++){
                        risDef(y,x)[0]= (ris2.at<UINT16>(y,x)-500)/8;
                        risDef(y,x)[1]= (ris2.at<UINT16>(y,x)-500)/8;
                        risDef(y,x)[2]= (ris2.at<UINT16>(y,x)-500)/8;
                    }

                frame.depth = risDef.clone();
                frame.depthWithoutSkeleton = risDef.clone();
            }
            SafeRelease(pFrameDescription);
        }
        SafeRelease(pDepthFrame);
    }

    //BODY INDEX reader
    IBodyIndexFrame *pBodyIndexFrame = nullptr;
    IBodyIndexFrameReader* pBodyIndexFrameReader;
    HRESULT hrindex;

    pBodyIndexFrameReader = GetBodyIndexReader();
    if (!pBodyIndexFrameReader)
    {   cout << "Body reader not available" << endl;
        frame;
    }

    hrindex = pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);

    if(SUCCEEDED(hrindex)){
        //pBodyIndexSource->get_FrameDescription(&pIndexDescription);
        int irows=424;
        int icols=512;

        Mat1b bodyIndexMat (irows, icols);
        //IBodyIndexFrame* pBodyIndexFrame = nullptr;
        //hr = pBodyIndexReader->AcquireLatestFrame( &pBodyIndexFrame );


        unsigned int bufferSize = 0;
        unsigned char* buffer = nullptr;
        hrindex = pBodyIndexFrame->AccessUnderlyingBuffer( &bufferSize, &buffer );

        if( SUCCEEDED( hrindex ) ){
            for( int y = 0; y < irows; y++ ){
                for( int x = 0; x < icols; x++ ){
                    unsigned int index = y * icols + x;
                    if( buffer[index] != 0xff ){
                        bodyIndexMat ( y, x )=buffer[index]+1;
                    }
                    else{
                        bodyIndexMat ( y, x )=0;
                    }
                }
            }
        }

        SafeRelease( pBodyIndexFrame );
        //debug
        //imshow("prova", bodyIndexMat);
        frame.playerIdx = bodyIndexMat.clone();
    }


    // BODY reader
    IBodyFrame* pBodyFrame = NULL;
    HRESULT hr;
    IBodyFrameReader* pBodyFrameReader;


    pBodyFrameReader = GetBodyReader();
    if (!pBodyFrameReader)
    {   printf("Body reader not available");
        frame;
    }

    //todo: check kinect open
    //CheckKinect();

    hr =  pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);


    IBody* ppBodies[BODY_COUNT] = {0};
    if (SUCCEEDED(hr))
    {

        hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
        //int bestDepthId;

        if (SUCCEEDED(hr))
        {
            int icount=0;

            //Select the closest
            if(modeSkeleton == 1){
                double bestDepth = DBL_MAX;
                _bestDepthId = -1;

                for (int i = 0; i < BODY_COUNT; ++i){
                    IBody* pBody = ppBodies[i];

                    if (pBody)
                    {
                        BOOLEAN bTracked = false;
                        hr = pBody->get_IsTracked(&bTracked);
                        UINT64 id;
                        pBody->get_TrackingId(&id);

                        Joint joints[JointType_Count];

                        if(bTracked){
                            hr = pBody->GetJoints(_countof(joints), joints);
                            double tmpDepth = joints[JointType_SpineMid].Position.Z;
                            if(tmpDepth < bestDepth){
                                bestDepth = tmpDepth;
                                _bestDepthId = i;
                            }
                        }

                    }
                }
            }

             for (int i = 0; i < BODY_COUNT; ++i){
            //if(bestDepthId != -1){

                IBody* pBody = ppBodies[i];
                if (pBody)
                {
                    BOOLEAN bTracked = false;
                    hr = pBody->get_IsTracked(&bTracked);
                    UINT64 id;
                    pBody->get_TrackingId(&id);


                    //Da mettere a posto, per ora inutile
                    if (SUCCEEDED(hr) && bTracked )
                    {
                        // mi salvo il player id
                        frame.idx = _bestDepthId;

                    }


                    // in ogni caso aggiorno i dati del frame
                    Joint joints[JointType_Count];
                    cv::Point2d jointPoints[JointType_Count];
                    cv::Point2d jointPointsRGB[JointType_Count];

                    hr = pBody->GetJoints(_countof(joints), joints);

                    // MAPPING KINECT 2 -> FRAMESET
                    if (SUCCEEDED(hr) && bTracked) //Mapping if only get joints and body is tracked
                    {
                        if(m_pCoordinateMapper &&  !frame.depth.empty() ){

                            int w, h, wRGB, hRGB;
                            w = frame.depth.cols;
                            h = frame.depth.rows;
                            wRGB = frame.RGB.cols;
                            hRGB = frame.RGB.rows;
                            HandState leftHandState = HandState_Unknown;
                            HandState rightHandState = HandState_Unknown;

                            pBody->get_HandLeftState(&leftHandState);
                            pBody->get_HandRightState(&rightHandState);

                            for (int j = 0; j < _countof(joints); ++j){
                                jointPoints[j] = BodyToScreen(joints[j].Position, w,h);
                                jointPointsRGB[j] = BodyToScreenRGB(joints[j].Position, wRGB , hRGB);
                            }

                            //Save coordinates only with selected skeleton
                            if(i == _bestDepthId){
                                storeCoordDepth(frame, jointPoints);
                                storeCoordRGB(frame, jointPointsRGB);
                                storeCoordReal(frame, joints);
                                DrawHand(leftHandState, jointPoints[JointType_HandLeft], frame.depth);
                                DrawHand(rightHandState, jointPoints[JointType_HandRight], frame.depth);
                                DrawBody(joints, jointPoints, frame.depth, true);
                            }
                            else
                                DrawBody(joints, jointPoints, frame.depth, false);

                        }

                    }

                }

            } // for body_count
            //m_pRenderTarget->EndDraw();
        }
    }

    SafeRelease(pBodyFrame);

    for (int iFace = 0; iFace < BODY_COUNT; ++iFace)
    {
        // retrieve the latest face frame from this reader
        IFaceFrame* pFaceFrame = nullptr;
        hr = m_pFaceFrameReaders[iFace]->AcquireLatestFrame(&pFaceFrame);

        BOOLEAN bFaceTracked = false;
        if (SUCCEEDED(hr) && nullptr != pFaceFrame)
        {
            // check if a valid face is tracked in this face frame
            hr = pFaceFrame->get_IsTrackingIdValid(&bFaceTracked);
        }

        if (SUCCEEDED(hr))
        {
            if (bFaceTracked)
            {
                IFaceFrameResult* pFaceFrameResult = nullptr;
                RectI faceBox = {0};
                PointF facePoints[FacePointType::FacePointType_Count];
                Vector4 faceRotation;
                DetectionResult faceProperties[FaceProperty::FaceProperty_Count];
                D2D1_POINT_2F faceTextLayout;

                hr = pFaceFrame->get_FaceFrameResult(&pFaceFrameResult);

                // need to verify if pFaceFrameResult contains data before trying to access it
                if (SUCCEEDED(hr) && pFaceFrameResult != nullptr)
                {
                    hr = pFaceFrameResult->get_FaceBoundingBoxInColorSpace(&faceBox);

                    if (SUCCEEDED(hr))
                    {
                        hr = pFaceFrameResult->get_FaceRotationQuaternion(&faceRotation);
                        ExtractFaceRotationInDegrees(faceRotation);
                        frame.pitch = _pitch;
                        frame.yaw = _yaw;
                        frame.roll = _roll;
                    }

                }

                SafeRelease(pFaceFrameResult);
            }
            else{
                // face tracking is not valid - attempt to fix the issue a valid body is required to perform this step
                if (true)
                {
                    // check if the corresponding body is tracked, if this is true then update the face frame source to track this body
                    IBody* pBody = ppBodies[iFace];
                    if (pBody != nullptr)
                    {
                        BOOLEAN bTracked = false;
                        hr = pBody->get_IsTracked(&bTracked);

                        UINT64 bodyTId;
                        if (SUCCEEDED(hr) && bTracked)
                        {
                            // get the tracking ID of this body
                            hr = pBody->get_TrackingId(&bodyTId);
                            if (SUCCEEDED(hr))
                            {
                                // update the face frame source with the tracking ID
                                m_pFaceFrameSources[iFace]->put_TrackingId(bodyTId);
                            }
                        }
                    }
                }
            }
        }
    }

    }

void AcquisitionKinect2::ExtractFaceRotationInDegrees(Vector4 quaternion){
    double x = quaternion.x;
    double y = quaternion.y;
    double z = quaternion.z;
    double w = quaternion.w;

    // convert face rotation quaternion to Euler angles in degrees
    double dPitch, dYaw, dRoll;
    dPitch = atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z) / M_PI * 180.0;
    dYaw = asin(2 * (w * y - x * z)) / M_PI * 180.0;
    dRoll = atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z) / M_PI * 180.0;

    // clamp rotation values in degrees to a specified range of values to control the refresh rate
    double increment = 5.0;//c_FaceRotationIncrementInDegrees;
    _pitch = static_cast<int>(floor((dPitch + increment/2.0 * (dPitch > 0 ? 1.0 : -1.0)) / increment) * increment);
    _yaw = static_cast<int>(floor((dYaw + increment/2.0 * (dYaw > 0 ? 1.0 : -1.0)) / increment) * increment);
    _roll = static_cast<int>(floor((dRoll + increment/2.0 * (dRoll > 0 ? 1.0 : -1.0)) / increment) * increment);

}

void AcquisitionKinect2::storeCoordRGB(FrameSet& frame, Point2d *jointPointsRGB){
    //Store RGB window coordinates
    frame.bodyJointRGB[FS_handRight].x = jointPointsRGB[JointType_HandRight].x;
    frame.bodyJointRGB[FS_handRight].y = jointPointsRGB[JointType_HandRight].y;

    frame.bodyJointRGB[FS_wristRight].x = jointPointsRGB[JointType_WristRight].x;
    frame.bodyJointRGB[FS_wristRight].y = jointPointsRGB[JointType_WristRight].y;

    frame.bodyJointRGB[FS_elbowRight].x = jointPointsRGB[JointType_ElbowRight].x;
    frame.bodyJointRGB[FS_elbowRight].y = jointPointsRGB[JointType_ElbowRight].y;

    frame.bodyJointRGB[FS_shoulderRight].x = jointPointsRGB[JointType_ShoulderRight].x;
    frame.bodyJointRGB[FS_shoulderRight].y = jointPointsRGB[JointType_ShoulderRight].y;

    frame.bodyJointRGB[FS_handLeft].x = jointPointsRGB[JointType_HandLeft].x;
    frame.bodyJointRGB[FS_handLeft].y = jointPointsRGB[JointType_HandLeft].y;

    frame.bodyJointRGB[FS_wristLeft].x = jointPointsRGB[JointType_WristLeft].x;
    frame.bodyJointRGB[FS_wristLeft].y = jointPointsRGB[JointType_WristLeft].y;

    frame.bodyJointRGB[FS_elbowLeft].x = jointPointsRGB[JointType_ElbowLeft].x;
    frame.bodyJointRGB[FS_elbowLeft].y = jointPointsRGB[JointType_ElbowLeft].y;

    frame.bodyJointRGB[FS_shoulderLeft].x = jointPointsRGB[JointType_ShoulderLeft].x;
    frame.bodyJointRGB[FS_shoulderLeft].y = jointPointsRGB[JointType_ShoulderLeft].y;

    frame.bodyJointRGB[FS_spineShoulder].x = jointPointsRGB[JointType_SpineShoulder].x;
    frame.bodyJointRGB[FS_spineShoulder].y = jointPointsRGB[JointType_SpineShoulder].y;

    frame.bodyJointRGB[FS_spineMid].x = jointPointsRGB[JointType_SpineMid].x;
    frame.bodyJointRGB[FS_spineMid].y = jointPointsRGB[JointType_SpineMid].y;

    frame.bodyJointRGB[FS_spineBase].x = jointPointsRGB[JointType_SpineBase].x;
    frame.bodyJointRGB[FS_spineBase].y = jointPointsRGB[JointType_SpineBase].y;

    frame.bodyJointRGB[FS_hipLeft].x = jointPointsRGB[JointType_HipLeft].x;
    frame.bodyJointRGB[FS_hipLeft].y = jointPointsRGB[JointType_HipLeft].y;

    frame.bodyJointRGB[FS_kneeLeft].x = jointPointsRGB[JointType_KneeLeft].x;
    frame.bodyJointRGB[FS_kneeLeft].y = jointPointsRGB[JointType_KneeLeft].y;

    frame.bodyJointRGB[FS_ankleLeft].x = jointPointsRGB[JointType_AnkleLeft].x;
    frame.bodyJointRGB[FS_ankleLeft].y = jointPointsRGB[JointType_AnkleLeft].y;

    frame.bodyJointRGB[FS_footLeft].x = jointPointsRGB[JointType_FootLeft].x;
    frame.bodyJointRGB[FS_footLeft].y = jointPointsRGB[JointType_FootLeft].y;

    frame.bodyJointRGB[FS_hipRight].x = jointPointsRGB[JointType_HipRight].x;
    frame.bodyJointRGB[FS_hipRight].y = jointPointsRGB[JointType_HipRight].y;

    frame.bodyJointRGB[FS_kneeRight].x = jointPointsRGB[JointType_KneeRight].x;
    frame.bodyJointRGB[FS_kneeRight].y = jointPointsRGB[JointType_KneeRight].y;

    frame.bodyJointRGB[FS_ankleRight].x = jointPointsRGB[JointType_AnkleRight].x;
    frame.bodyJointRGB[FS_ankleRight].y = jointPointsRGB[JointType_AnkleRight].y;

    frame.bodyJointRGB[FS_footRight].x = jointPointsRGB[JointType_FootRight].x;
    frame.bodyJointRGB[FS_footRight].y = jointPointsRGB[JointType_FootRight].y;

    frame.bodyJointRGB[FS_head].x = jointPointsRGB[JointType_Head].x;
    frame.bodyJointRGB[FS_head].y = jointPointsRGB[JointType_Head].y;

    frame.bodyJointRGB[FS_neck].x = jointPointsRGB[JointType_Neck].x;
    frame.bodyJointRGB[FS_neck].y = jointPointsRGB[JointType_Neck].y;
}

void AcquisitionKinect2::storeCoordDepth(FrameSet& frame, Point2d *jointPoints){
    //Store DEPTH window coordinates
    frame.bodyJointDepth[FS_handRight].x = jointPoints[JointType_HandRight].x;
    frame.bodyJointDepth[FS_handRight].y = jointPoints[JointType_HandRight].y;

    frame.bodyJointDepth[FS_wristRight].x = jointPoints[JointType_WristRight].x;
    frame.bodyJointDepth[FS_wristRight].y = jointPoints[JointType_WristRight].y;

    frame.bodyJointDepth[FS_elbowRight].x = jointPoints[JointType_ElbowRight].x;
    frame.bodyJointDepth[FS_elbowRight].y = jointPoints[JointType_ElbowRight].y;

    frame.bodyJointDepth[FS_shoulderRight].x = jointPoints[JointType_ShoulderRight].x;
    frame.bodyJointDepth[FS_shoulderRight].y = jointPoints[JointType_ShoulderRight].y;

    frame.bodyJointDepth[FS_handLeft].x = jointPoints[JointType_HandLeft].x;
    frame.bodyJointDepth[FS_handLeft].y = jointPoints[JointType_HandLeft].y;

    frame.bodyJointDepth[FS_wristLeft].x = jointPoints[JointType_WristLeft].x;
    frame.bodyJointDepth[FS_wristLeft].y = jointPoints[JointType_WristLeft].y;

    frame.bodyJointDepth[FS_elbowLeft].x = jointPoints[JointType_ElbowLeft].x;
    frame.bodyJointDepth[FS_elbowLeft].y = jointPoints[JointType_ElbowLeft].y;

    frame.bodyJointDepth[FS_shoulderLeft].x = jointPoints[JointType_ShoulderLeft].x;
    frame.bodyJointDepth[FS_shoulderLeft].y = jointPoints[JointType_ShoulderLeft].y;

    frame.bodyJointDepth[FS_spineShoulder].x = jointPoints[JointType_SpineShoulder].x;
    frame.bodyJointDepth[FS_spineShoulder].y = jointPoints[JointType_SpineShoulder].y;

    frame.bodyJointDepth[FS_spineMid].x = jointPoints[JointType_SpineMid].x;
    frame.bodyJointDepth[FS_spineMid].y = jointPoints[JointType_SpineMid].y;

    frame.bodyJointDepth[FS_spineBase].x = jointPoints[JointType_SpineBase].x;
    frame.bodyJointDepth[FS_spineBase].y = jointPoints[JointType_SpineBase].y;

    frame.bodyJointDepth[FS_hipLeft].x = jointPoints[JointType_HipLeft].x;
    frame.bodyJointDepth[FS_hipLeft].y = jointPoints[JointType_HipLeft].y;

    frame.bodyJointDepth[FS_kneeLeft].x = jointPoints[JointType_KneeLeft].x;
    frame.bodyJointDepth[FS_kneeLeft].y = jointPoints[JointType_KneeLeft].y;

    frame.bodyJointDepth[FS_ankleLeft].x = jointPoints[JointType_AnkleLeft].x;
    frame.bodyJointDepth[FS_ankleLeft].y = jointPoints[JointType_AnkleLeft].y;

    frame.bodyJointDepth[FS_footLeft].x = jointPoints[JointType_FootLeft].x;
    frame.bodyJointDepth[FS_footLeft].y = jointPoints[JointType_FootLeft].y;

    frame.bodyJointDepth[FS_hipRight].x = jointPoints[JointType_HipRight].x;
    frame.bodyJointDepth[FS_hipRight].y = jointPoints[JointType_HipRight].y;

    frame.bodyJointDepth[FS_kneeRight].x = jointPoints[JointType_KneeRight].x;
    frame.bodyJointDepth[FS_kneeRight].y = jointPoints[JointType_KneeRight].y;

    frame.bodyJointDepth[FS_ankleRight].x = jointPoints[JointType_AnkleRight].x;
    frame.bodyJointDepth[FS_ankleRight].y = jointPoints[JointType_AnkleRight].y;

    frame.bodyJointDepth[FS_footRight].x = jointPoints[JointType_FootRight].x;
    frame.bodyJointDepth[FS_footRight].y = jointPoints[JointType_FootRight].y;

    frame.bodyJointDepth[FS_head].x = jointPoints[JointType_Head].x;
    frame.bodyJointDepth[FS_head].y = jointPoints[JointType_Head].y;

    frame.bodyJointDepth[FS_neck].x = jointPoints[JointType_Neck].x;
    frame.bodyJointDepth[FS_neck].y = jointPoints[JointType_Neck].y;
}

void AcquisitionKinect2::storeCoordReal(FrameSet &frame, Joint *joints){
    //Head
    frame.bodyJoint[FS_head].x = joints[JointType_Head].Position.X;
    frame.bodyJoint[FS_head].y = joints[JointType_Head].Position.Y;
    frame.bodyJoint[FS_head].z = joints[JointType_Head].Position.Z;

    //Neck
    frame.bodyJoint[FS_neck].x = joints[JointType_Neck].Position.X;
    frame.bodyJoint[FS_neck].y = joints[JointType_Neck].Position.Y;
    frame.bodyJoint[FS_neck].z = joints[JointType_Neck].Position.Z;

    //Spine Shoulder
    frame.bodyJoint[FS_spineShoulder].x = joints[JointType_SpineShoulder].Position.X;
    frame.bodyJoint[FS_spineShoulder].y = joints[JointType_SpineShoulder].Position.Y;
    frame.bodyJoint[FS_spineShoulder].z = joints[JointType_SpineShoulder].Position.Z;

    //Shoulder Left
    frame.bodyJoint[FS_shoulderLeft].x = joints[JointType_ShoulderLeft].Position.X;
    frame.bodyJoint[FS_shoulderLeft].y = joints[JointType_ShoulderLeft].Position.Y;
    frame.bodyJoint[FS_shoulderLeft].z = joints[JointType_ShoulderLeft].Position.Z;

    //Elbow Left
    frame.bodyJoint[FS_elbowLeft].x = joints[JointType_ElbowLeft].Position.X;
    frame.bodyJoint[FS_elbowLeft].y = joints[JointType_ElbowLeft].Position.Y;
    frame.bodyJoint[FS_elbowLeft].z = joints[JointType_ElbowLeft].Position.Z;

    //Wrist Left
    frame.bodyJoint[FS_wristLeft].x = joints[JointType_WristLeft].Position.X;
    frame.bodyJoint[FS_wristLeft].y = joints[JointType_WristLeft].Position.Y;
    frame.bodyJoint[FS_wristLeft].z = joints[JointType_WristLeft].Position.Z;

    //Hand Left
    frame.bodyJoint[FS_handLeft].x = joints[JointType_HandLeft].Position.X;
    frame.bodyJoint[FS_handLeft].y = joints[JointType_HandLeft].Position.Y;
    frame.bodyJoint[FS_handLeft].z = joints[JointType_HandLeft].Position.Z;

    //Thumb Left
    frame.bodyJoint[FS_thumbLeft].x = joints[JointType_ThumbLeft].Position.X;
    frame.bodyJoint[FS_thumbLeft].y = joints[JointType_ThumbLeft].Position.Y;
    frame.bodyJoint[FS_thumbLeft].z = joints[JointType_ThumbLeft].Position.Z;

    //Hand Tip Left
    frame.bodyJoint[FS_handTipLeft].x = joints[JointType_HandTipLeft].Position.X;
    frame.bodyJoint[FS_handTipLeft].y = joints[JointType_HandTipLeft].Position.Y;
    frame.bodyJoint[FS_handTipLeft].z = joints[JointType_HandTipLeft].Position.Z;

    //Shoulder Right
    frame.bodyJoint[FS_shoulderRight].x = joints[JointType_ShoulderRight].Position.X;
    frame.bodyJoint[FS_shoulderRight].y = joints[JointType_ShoulderRight].Position.Y;
    frame.bodyJoint[FS_shoulderRight].z = joints[JointType_ShoulderRight].Position.Z;

    //Elbow Right
    frame.bodyJoint[FS_elbowRight].x = joints[JointType_ElbowRight].Position.X;
    frame.bodyJoint[FS_elbowRight].y = joints[JointType_ElbowRight].Position.Y;
    frame.bodyJoint[FS_elbowRight].z = joints[JointType_ElbowRight].Position.Z;

    //Wrist Right
    frame.bodyJoint[FS_wristRight].x = joints[JointType_WristRight].Position.X;
    frame.bodyJoint[FS_wristRight].y = joints[JointType_WristRight].Position.Y;
    frame.bodyJoint[FS_wristRight].z = joints[JointType_WristRight].Position.Z;

    //Hand Right
    frame.bodyJoint[FS_handRight].x = joints[JointType_HandRight].Position.X;
    frame.bodyJoint[FS_handRight].y = joints[JointType_HandRight].Position.Y;
    frame.bodyJoint[FS_handRight].z = joints[JointType_HandRight].Position.Z;

    //Thumb Right
    frame.bodyJoint[FS_thumbRight].x = joints[JointType_ThumbRight].Position.X;
    frame.bodyJoint[FS_thumbRight].y = joints[JointType_ThumbRight].Position.Y;
    frame.bodyJoint[FS_thumbRight].z = joints[JointType_ThumbRight].Position.Z;

    //Hand Tip Right
    frame.bodyJoint[FS_handTipRight].x = joints[JointType_HandTipRight].Position.X;
    frame.bodyJoint[FS_handTipRight].y = joints[JointType_HandTipRight].Position.Y;
    frame.bodyJoint[FS_handTipRight].z = joints[JointType_HandTipRight].Position.Z;

    //Spine Mid
    frame.bodyJoint[FS_spineMid].x = joints[JointType_SpineMid].Position.X;
    frame.bodyJoint[FS_spineMid].y = joints[JointType_SpineMid].Position.Y;
    frame.bodyJoint[FS_spineMid].z = joints[JointType_SpineMid].Position.Z;

    //Spine Base
    frame.bodyJoint[FS_spineBase].x = joints[JointType_SpineBase].Position.X;
    frame.bodyJoint[FS_spineBase].y = joints[JointType_SpineBase].Position.Y;
    frame.bodyJoint[FS_spineBase].z = joints[JointType_SpineBase].Position.Z;

    //Hip Left
    frame.bodyJoint[FS_hipLeft].x = joints[JointType_HipLeft].Position.X;
    frame.bodyJoint[FS_hipLeft].y = joints[JointType_HipLeft].Position.Y;
    frame.bodyJoint[FS_hipLeft].z = joints[JointType_HipLeft].Position.Z;

    //Knee Left
    frame.bodyJoint[FS_kneeLeft].x = joints[JointType_KneeLeft].Position.X;
    frame.bodyJoint[FS_kneeLeft].y = joints[JointType_KneeLeft].Position.Y;
    frame.bodyJoint[FS_kneeLeft].z = joints[JointType_KneeLeft].Position.Z;

    //Ankle Left
    frame.bodyJoint[FS_ankleLeft].x = joints[JointType_AnkleLeft].Position.X;
    frame.bodyJoint[FS_ankleLeft].y = joints[JointType_AnkleLeft].Position.Y;
    frame.bodyJoint[FS_ankleLeft].z = joints[JointType_AnkleLeft].Position.Z;

    //Foot Left
    frame.bodyJoint[FS_footLeft].x = joints[JointType_FootLeft].Position.X;
    frame.bodyJoint[FS_footLeft].y = joints[JointType_FootLeft].Position.Y;
    frame.bodyJoint[FS_footLeft].z = joints[JointType_FootLeft].Position.Z;

    //Hip Right
    frame.bodyJoint[FS_hipRight].x = joints[JointType_HipRight].Position.X;
    frame.bodyJoint[FS_hipRight].y = joints[JointType_HipRight].Position.Y;
    frame.bodyJoint[FS_hipRight].z = joints[JointType_HipRight].Position.Z;

    //Knee Right
    frame.bodyJoint[FS_kneeRight].x = joints[JointType_KneeRight].Position.X;
    frame.bodyJoint[FS_kneeRight].y = joints[JointType_KneeRight].Position.Y;
    frame.bodyJoint[FS_kneeRight].z = joints[JointType_KneeRight].Position.Z;

    //Ankle Right
    frame.bodyJoint[FS_ankleRight].x = joints[JointType_AnkleRight].Position.X;
    frame.bodyJoint[FS_ankleRight].y = joints[JointType_AnkleRight].Position.Y;
    frame.bodyJoint[FS_ankleRight].z = joints[JointType_AnkleRight].Position.Z;

    //Foot Right
    frame.bodyJoint[FS_footRight].x = joints[JointType_FootRight].Position.X;
    frame.bodyJoint[FS_footRight].y = joints[JointType_FootRight].Position.Y;
    frame.bodyJoint[FS_footRight].z = joints[JointType_FootRight].Position.Z;
}
