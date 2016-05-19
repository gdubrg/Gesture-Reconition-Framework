#ifndef FRAMESET
#define FRAMESET

#include <opencv2/opencv.hpp>
#include <vector>

//Body skeleton joints names
enum jointNames{
    FS_head,
    FS_neck,
    FS_shoulderCenter,
    FS_spineShoulder,
    FS_shoulderRight,
    FS_elbowRight,
    FS_wristRight,
    FS_handRight,
    FS_thumbRight,
    FS_shoulderLeft,
    FS_elbowLeft,
    FS_wristLeft,
    FS_handLeft,
    FS_thumbLeft,
    FS_spineMid,
    FS_spine,
    FS_spineBase,
    FS_hipCenter,
    FS_hipRight,
    FS_kneeRight,
    FS_ankleRight,
    FS_footRight,
    FS_hipLeft,
    FS_kneeLeft,
    FS_ankleLeft,
    FS_footLeft,
    FS_handTipRight,
    FS_handTipLeft
};

struct tripletPoint{
    float x = FLT_MAX;
    float y = FLT_MAX;
    float z = FLT_MAX;
};


/// Framset class
///  contiene tutti i dati di un frame acquisito di una singola persona.
/// Fondamentale la presenza dello scheletro ( std::vector<tripletPoint>)
class FrameSet{

public:

    //RGB Frame
    cv::Mat RGB;

    //Depth Frame
    cv::Mat depth;

    //Frame with players
    cv::Mat1b playerIdx;

    //Player idx (only 1 player)
    int idx;

    //Skeleton joints
    std::vector<tripletPoint> bodyJoint;

    //Skeleton joints in depth window coordinates (512x424)
    std::vector<tripletPoint> bodyJointDepth;

    //Skeleton joints in depth window coordinates (1920x1080)
    std::vector<tripletPoint> bodyJointRGB;

    //Head Rotation
    int pitch;
    int yaw;
    int roll;

   //Kinect Version.
    // TODO: più che kinect version, possiamo fare un altro enum con tipi di sorgenti diverse
    // es, se in futuro non usassimo un kinect ma un altro dispositivo...
   int kinectVersion;

   //Constructor
   FrameSet(){
       // init dei vari dati
       kinectVersion = 0;
       idx=-1;
       pitch=0; yaw=0; roll=0;

       bodyJoint.resize(28, tripletPoint());
       bodyJointDepth.resize(28, tripletPoint());
       bodyJointRGB.resize(28, tripletPoint());
   }


private:

};



#endif // FRAMESET

