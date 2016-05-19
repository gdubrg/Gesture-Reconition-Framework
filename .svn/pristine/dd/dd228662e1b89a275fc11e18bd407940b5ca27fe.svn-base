
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <locale>
#include <ctime>
#include <sstream>

#include "gesturerecognitiondtw.h"
#include "frameset.h"

using namespace std;
using namespace cv;

static const int windowSizeOnline = 33;
static const int minlength = 30;

//DTW matrix
//Mat1d tab(windowSize,windowSize);
//Mat1d slopeI(windowSize,windowSize);
//Mat1d slopeJ(windowSize,windowSize);

enum DTWJoint{
    DTW_handLeft,
    DTW_wristLeft,
    DTW_elbowLeft,
    DTW_elbowRight,
    DTW_wristRight,
    DTW_handRight,
    DTW_shoulderLeft,
    DTW_shoulderRight
};

//Constructor
GestureRecognitionDTW::GestureRecognitionDTW(){

    //Load gestures from file
    //loadFile("dtw/_defaultGestureFile2.txt");
    loadFile("dtw/rotateandpoint.txt");
    //loadFile("dtw/4gesture.txt");

    //Initialize vector scores (dim = num of labels)
    allScores.resize(allLabels.size(), 0.0);

    //mhiImage = nullptr;
  //  if(mhiImage == nullptr)
        mhiImage = Mat::zeros(424, 512, CV_32FC1);
}

GestureRecognitionDTW::~GestureRecognitionDTW() {
    //delete mhiImage;
}

//TEST MHI
void GestureRecognitionDTW::mhi(FrameSet &frame, int nFrame){

    //Mat1b mask(frame.depth.rows, frame.depth.cols, uchar(0));

   // Mat1b tmpMask1(frame.depth.rows, frame.depth.cols, uchar(0));
    //tmpMask1 = frame.depth.clone();


//    for(int r=0;r<frame.depth.rows;++r)
//        for(int c=0;c<frame.depth.cols;++c){
//            if(frame.depth.at<ushort>(r,c) > threshDepth-1 && frame.depth.at<ushort>(r,c) < threshDepth+1)
//                mask(r,c) = 255;

//        }

//    if(frame.RGB.rows > 1){
//         Mat1b mask(frame.RGB.rows, frame.RGB.cols, uchar(0));
//         int threshDepth = frame.RGB.at<ushort>((int)frame.RGB.rows/2, (int)frame.RGB.cols/2);
//    for(int r=0;r<frame.RGB.rows;++r)
//        for(int c=0;c<frame.RGB.cols;++c){
//            if(frame.RGB.at<ushort>(r,c) > threshDepth-1 && frame.RGB.at<ushort>(r,c) < threshDepth+1)
//                mask(r,c) = 255;

//        }
//    resize(mask, mask, Size(1080, 1920), INTER_NEAREST);


    Mat1b mask(frame.playerIdx.rows, frame.playerIdx.cols, uchar(0));
    circle(mask, Point(frame.bodyJointDepth[FS_handRight].x, frame.bodyJointDepth[FS_handRight].y), 10, Scalar(255), CV_FILLED);
    circle(mask, Point(frame.bodyJointDepth[FS_handLeft].x, frame.bodyJointDepth[FS_handLeft].y), 10, Scalar(255), CV_FILLED);

    for(int r=0;r<frame.playerIdx.rows;++r)
        for(int c=0;c<frame.playerIdx.cols;++c)
            if(frame.playerIdx(r,c) != 0)
                mask(r,c) = 255;
    else
                mask(r,c) = 0;

    if(mask.rows > 0)
    imshow("ieuie", mask);


    if(mask.rows > 0){
    updateMotionHistory(mask, mhiImage, nFrame, 300);
    Mat1b tmpMat; //tmpMat = mhiImage.clone();
    mhiImage.convertTo(tmpMat, CV_8UC1, 255);
    imshow("jhiuh", tmpMat);
    }

    //}

}

//Main
void GestureRecognitionDTW::recognize(FrameSet &frame){


    //Acquisisco sequenza (window di tot frame)
    getWindowSequence(frame);

    Mat1b tmpMat;
    //mhi(frame, nFrame);

    nFrame++;

    //circle(frame.RGB, Point(frame.bodyJointRGB[FS_handRight].x, frame.bodyJointRGB[FS_handRight].y), 10, Scalar(0, 0, 255), -1);
    //circle(frame.RGB, Point(frame.bodyJointRGB[FS_handLeft].x, frame.bodyJointRGB[FS_handLeft].y), 10, Scalar(0, 0, 255), -1);
    //circle(frame.depth, Point(frame.bodyJointDepth[FS_head].x, frame.bodyJointDepth[FS_head].y), 10, Scalar(0, 0, 255), -1);


    emit WindowSizeSignal();

    if(allFrames.size() == windowSizeOnline){

       // nFrame = 0;

        //Compute DTW
        int best;
        double resDTW;
        bestDTW = DBL_MAX;
        for(int i=0;i<allLabels.size();++i){

            if(eucDistJoints(allGestures[i][windowSizeOnline-1], allFrames[windowSizeOnline-1]) < thresholdLastFrame){
                resDTW = dtw(allGestures[i], allFrames);
                resDTW /= allFrames.size();
                allScores[i] = resDTW;

                if(resDTW < bestDTW){
                    best = i; //save index for label
                    bestDTW = resDTW;

                }
            }
        }

        //Classifico (prendo costo minore)
        classificationString = (bestDTW<thresholdDTW ? allLabels[best] : "Unknown");


        emit (classificationDone());

        cout << classificationString << ": " << bestDTW << endl;

        if(bestDTW<thresholdDTW){

            qDebug()<<allFrames.size();
            allFrames.clear();
            qDebug()<<allFrames.size();

            // debug
            for (int i1=0; i1<windowSizeOnline; ++i1){
                for (int i2=0; i2<windowSizeOnline; ++i2){
                    //cout <<tab(i1,i2) << "\t";
                }
                cout<< "\n";
            }
            cout<< "\n";

        }
        else{
            allFrames.erase(allFrames.begin());
            //allFrames.clear();
        }
    }
}

//Get a sequence of windowsize frames
void GestureRecognitionDTW::getWindowSequence(FrameSet &frame){

    tripletPoint p;

    //Hand Left
    p.x = frame.bodyJoint[FS_handLeft].x;
    p.y = frame.bodyJoint[FS_handLeft].y;
    p.z = frame.bodyJoint[FS_handLeft].z;

    allJoint.push_back(p);

    //Wrist Left
    p.x = frame.bodyJoint[FS_wristLeft].x;
    p.y = frame.bodyJoint[FS_wristLeft].y;
    p.z = frame.bodyJoint[FS_wristLeft].z;

    allJoint.push_back(p);

    //Elbow Left
    p.x = frame.bodyJoint[FS_elbowLeft].x;
    p.y = frame.bodyJoint[FS_elbowLeft].y;
    p.z = frame.bodyJoint[FS_elbowLeft].z;

    allJoint.push_back(p);

    //Elbow Right
    p.x = frame.bodyJoint[FS_elbowRight].x;
    p.y = frame.bodyJoint[FS_elbowRight].y;
    p.z = frame.bodyJoint[FS_elbowRight].z;

    allJoint.push_back(p);

    //Wrist Right
    p.x = frame.bodyJoint[FS_wristRight].x;
    p.y = frame.bodyJoint[FS_wristRight].y;
    p.z = frame.bodyJoint[FS_wristRight].z;

    allJoint.push_back(p);

    //Hand Right
    p.x = frame.bodyJoint[FS_handRight].x;
    p.y = frame.bodyJoint[FS_handRight].y;
    p.z = frame.bodyJoint[FS_handRight].z;

    allJoint.push_back(p);

    //Shoulder Right
    p.x = frame.bodyJoint[FS_shoulderRight].x;
    p.y = frame.bodyJoint[FS_shoulderRight].y;
    p.z = frame.bodyJoint[FS_shoulderRight].z;

    allJoint.push_back(p);

    //Shoulder Left
    p.x = frame.bodyJoint[FS_shoulderLeft].x;
    p.y = frame.bodyJoint[FS_shoulderLeft].y;
    p.z = frame.bodyJoint[FS_shoulderLeft].z;

    allJoint.push_back(p);

    //Centre the data
    centreNormData(allJoint);

    // Collect all joint in 1 frame
    allFrames.push_back(allJoint);

    //Clear joint vector
    allJoint.clear();
}

//Centre and Normalize Data
void GestureRecognitionDTW::centreNormData(vector<tripletPoint>& allJoint){

    double centerX = (allJoint[DTW_shoulderRight].x + allJoint[DTW_shoulderLeft].x) / 2;
    double centerY = (allJoint[DTW_shoulderRight].y + allJoint[DTW_shoulderLeft].y) / 2;

    //Centre the data
    for(int i=0; i<6;++i){
        allJoint[i].x -= centerX;
        allJoint[i].y -= centerY;

    }

    //Normalization of the coordinates
    double shouldDist = sqrt(pow(allJoint[DTW_shoulderRight].x - allJoint[DTW_shoulderLeft].x, 2)+pow(allJoint[DTW_shoulderRight].y - allJoint[DTW_shoulderLeft].y, 2));

    for(int i=0;i<6;++i){
        allJoint[i].x /= shouldDist;
        allJoint[i].y /= shouldDist;

        //cout << allJoint[i].x << " " << allJoint[i].y << endl;
    }

}

//Compute Euclidean Distance
double GestureRecognitionDTW::eucDist(tripletPoint a, tripletPoint b){

    double d;
    d = pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2);
    return sqrt(d);

}

//Compute Euclidean Distance between set of joints
double GestureRecognitionDTW::eucDistJoints(vector<tripletPoint> set1, vector<tripletPoint> set2){

    double d = 0.0;
    for(int i=0;i<set1.size();++i){
        double tmp = 0.0;
        tmp = pow((set1[i].x - set2[i].x), 2) + pow((set1[i].y - set2[i].y), 2);
        d += sqrt(tmp);
    }

    return d;
}

//Get window size
int GestureRecognitionDTW::getWindowSize(){
    return allFrames.size();
}

//Load gestures from file
void GestureRecognitionDTW::loadFile(string filename){

    //Open file
    ifstream in(filename);//, std::ios_base::in);
    if(!in.is_open()){
        cout << "Error opening gesture file..." << endl;
        return;
    }
    else
        cout << "Gesture file opened..." << endl;

    //To read double with comma instead dot
    locale myLocale("");
    in.imbue(myLocale);

    while(!in.eof()){

        //Read gesture name
       char nameLabel[50];
       in.getline(nameLabel, 50);
       string tmpName = nameLabel;
       allLabels.push_back(tmpName);

       //cout << "Nome gesture: " << tmpName << endl;

       tripletPoint tmpP;
       vector<tripletPoint> tmpSingleFrame;
       vector<vector<tripletPoint>> tmpSingleGest;

       //For all frames
       for(int i=0;i<33;++i){

           for(int k=0;k<6;++k){

               double x = 0.0, y= 0.0;
               in >> x; in.ignore();
               in >> y; in.ignore();
               tmpP.x = x;// cout << tmpP.x << endl;
               tmpP.y = y; //cout << tmpP.y << endl;

               tmpSingleFrame.push_back(tmpP);

           }

           in.getline(nameLabel, 50); //Leggo separatore dei frame (TOLTO)
           //cout << nameLabel << endl;
           tmpSingleGest.push_back(tmpSingleFrame);
           tmpSingleFrame.clear();
       }

       //in.getline(nameLabel, 50); // vado a capo e leggo spazio bianco
       //cout << nameLabel << endl;
       in.getline(nameLabel, 50); //leggo ----
       //cout << nameLabel << endl;
       //cout << "Aggiungo gesture" << endl;
       allGestures.push_back(tmpSingleGest);
       tmpSingleGest.clear();
    }

    cout << "Loaded " << allGestures.size() << " gestures from file" << endl;

    in.close();
}

//DTW method (return best match)
double GestureRecognitionDTW::dtw(vector<vector<tripletPoint>> seq1, vector<vector<tripletPoint>> seq2){


    //reverse(seq1.begin(),seq1.end());
    //reverse(seq2.begin(),seq2.end());

    int maxSlope = 2;
    double bestMatch = DBL_MAX;

    //Check sequence length
//    if((seq1.size() != windowSize) && (seq2.size() != windowSize) ){
//        cout << "Error in sequence length!" << endl;
//        return -1;
//    }
    int windowSize = 0;
    if(seq1.size() < seq2.size())
        windowSize = seq1.size();
    else
        windowSize = seq2.size();

    Mat1d tab(windowSize,windowSize);
    Mat1d slopeI(windowSize,windowSize);
    Mat1d slopeJ(windowSize,windowSize);

    //Initialize matrix
    for(int r=0;r<tab.rows;++r)
        for(int c=0;c<tab.cols;++c){
            tab(r, c) = DBL_MAX;
            slopeI(r ,c) = 0;
            slopeJ(r ,c) = 0;
        }
    tab(0,0) = 0.0;

    //Dynamic computation
    for(int i=1;i<tab.rows;++i)
        for(int j=1;j<tab.cols;++j){

            if( (tab(i, j-1) < tab(i-1, j-1)) && (tab(i, j-1) < tab(i-1, j)) && (slopeI(i, j-1) < maxSlope)){
                tab(i, j) = eucDistJoints(seq1[i], seq2[i]) + tab(i, j-1);
                slopeI(i, j) = slopeI(i, j-1) + 1;
                slopeJ(i, j) = 0;
            }

            else if( (tab(i-1, j) < tab(i-1, j-1)) && (tab(i-1, j) < tab(i, j-1)) && (slopeJ(i-1, j) < maxSlope)){
                    tab(i, j) = eucDistJoints(seq1[i], seq2[i]) + tab(i-1, j);
                    slopeI(i, j) = 0;
                    slopeJ(i, j) = slopeJ(i-1, j) + 1;
            }
            else{
                tab(i, j) = eucDistJoints(seq1[i], seq2[i]) + tab(i-1, j-1);
                slopeI(i, j) = 0;
                slopeJ(i, j) = 0;
            }
        }

    //Find best match inside last column
    for(int i=minlength; i<(windowSize-1); ++i)
        if(tab(i, (windowSize-1)) < bestMatch)
            bestMatch = tab(i, (windowSize-1));

    //Find best match inside last row
    for(int i=minlength; i<(windowSize-1); ++i)
        if(tab((windowSize-1), 1) < bestMatch)
            bestMatch = tab((windowSize-1), 1);

    return bestMatch;

}

void getDistFeatures(FrameSet frame, vector<tripletPoint>& tmpSingleFrame){

    tripletPoint tmpP;

    tmpP.x = frame.bodyJoint[FS_handLeft].x - frame.bodyJoint[FS_handRight].x;
    tmpP.y = frame.bodyJoint[FS_handLeft].y - frame.bodyJoint[FS_handRight].y;
    tmpP.z = frame.bodyJoint[FS_handLeft].z - frame.bodyJoint[FS_handRight].z;
    tmpSingleFrame.push_back(tmpP);

    tmpP.x = frame.bodyJoint[FS_handLeft].x - frame.bodyJoint[FS_hipCenter].x;
    tmpP.y = frame.bodyJoint[FS_handLeft].y - frame.bodyJoint[FS_hipCenter].y;
    tmpP.z = frame.bodyJoint[FS_handLeft].z - frame.bodyJoint[FS_hipCenter].z;
    tmpSingleFrame.push_back(tmpP);

    tmpP.x = frame.bodyJoint[FS_handRight].x - frame.bodyJoint[FS_hipCenter].x;
    tmpP.y = frame.bodyJoint[FS_handRight].y - frame.bodyJoint[FS_hipCenter].y;
    tmpP.z = frame.bodyJoint[FS_handRight].z - frame.bodyJoint[FS_hipCenter].z;
    tmpSingleFrame.push_back(tmpP);

    tmpP.x = frame.bodyJoint[FS_footLeft].x - frame.bodyJoint[FS_hipCenter].x;
    tmpP.y = frame.bodyJoint[FS_footLeft].y - frame.bodyJoint[FS_hipCenter].y;
    tmpP.z = frame.bodyJoint[FS_footLeft].z - frame.bodyJoint[FS_hipCenter].z;
    tmpSingleFrame.push_back(tmpP);

    tmpP.x = frame.bodyJoint[FS_footRight].x - frame.bodyJoint[FS_hipCenter].x;
    tmpP.y = frame.bodyJoint[FS_footRight].y - frame.bodyJoint[FS_hipCenter].y;
    tmpP.z = frame.bodyJoint[FS_footRight].z - frame.bodyJoint[FS_hipCenter].z;
    tmpSingleFrame.push_back(tmpP);

}

void getCoordFeatures(FrameSet frame, vector<tripletPoint>& tmpSingleFrame){

    tripletPoint tmpP;

    //For each joint
    for(auto i=0; i<28; ++i){
        if(frame.bodyJoint[i].x != FLT_MAX){
            tmpP.x = frame.bodyJoint[i].x;
            tmpP.y = frame.bodyJoint[i].y;
            tmpP.z = frame.bodyJoint[i].z;
            tmpSingleFrame.push_back(tmpP);
        }
    }
}

//DTW TRAIN
void GestureRecognitionDTW::Train(const TrainingSet &tr){

    // clear di tutto, se chiamo il train cancello, altrimenti procede come sempre
    allGestures.clear();
    allScores.clear();
    allLabels.clear();

    // creo la struttura per i gesti

    vector<tripletPoint> tmpSingleFrame;
    vector<vector<tripletPoint>> tmpSingleGest;

    // per ogni sequenza del training set

    // iterazione per ogni video
    for (auto it = tr.begin(); it != tr.end(); ++it){

        int idGesto;
        idGesto = (*it).GetGestureId();

        //Memorizzo l'id per ogni video
        allLabels.push_back(to_string(idGesto));
        allLabelsMSR.push_back(idGesto);

        //iterazione per ogni frame
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            if(tr.getTypeFeatures() == "coord")
                //FEATURES: semplici coordinate di tutti i giunti
                getCoordFeatures((*itframeset), tmpSingleFrame);

            if(tr.getTypeFeatures() == "dist")
                //FEATURES: distanza coppie giunti
                getDistFeatures((*itframeset), tmpSingleFrame);

            //To do: normalizzazione rispetto spalle e centro

            //Per ogni singolo frame, accumulo e pulisco
            tmpSingleGest.push_back(tmpSingleFrame);
            tmpSingleFrame.clear();
        }

        //Per ogni singolo video, accumulo e pulisco
        allGestures.push_back(tmpSingleGest);
        tmpSingleGest.clear();

    }

    cout << "DTW TRAIN: Loaded " << allGestures.size() << " gestures" << endl;
    cout << "DTW TRAIN: Stored " << allLabels.size() << " labels" << endl;

}

//Get current date (day, month, year, hour, min, sec)
string getNameDate(){

    time_t currentTime;
    struct tm *localTime;
    int day, month, year, hour, min, sec;
    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time

    day = localTime->tm_mday;
    month = localTime->tm_mon + 1;
    year = localTime->tm_year + 1900;
    hour = localTime->tm_hour;
    min = localTime->tm_min;
    sec = localTime->tm_sec;

    stringstream nameDate;
    nameDate << to_string(day) << "-" << to_string(month) << "-" << to_string(year) << "_" << to_string(hour) << "_" << to_string(min) << "_" << to_string(sec);

    return nameDate.str();
}

//DTW TEST
void GestureRecognitionDTW::Test(const TestingSet &ts){

    allScores.resize(allLabels.size(), 0.0);
    int idActionTest;

    //Per ogni azione presente nel testingset
    for (auto it = ts.begin(); it != ts.end(); ++it){

        //Estraggo label azione
        idActionTest = (*it).GetGestureId();

        //Estraggo vector<tripletJoint>, per ogni frame
        tripletPoint tmpP;
        vector<tripletPoint> testFrame;
        vector<vector<tripletPoint>> testAction;
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            if(ts.getTypeFeatures() == "coord")
                //FEATURES: semplici coordinate di tutti i giunti
                getCoordFeatures((*itframeset), testFrame);

            if(ts.getTypeFeatures() == "dist")
                //FEATURES: distanza coppie giunti
                getDistFeatures((*itframeset), testFrame);

            testAction.push_back(testFrame);
            testFrame.clear();
        }


        //Compute DTW
        int best;
        double resDTW;
        bestDTW = DBL_MAX;

        //Per ogni azione di train
        for(int i=0;i<allLabels.size();++i){

            //TO DO: controllo iniziale distanza ultimo frame
            //if(eucDistJoints(allGestures[i][windowSize-1], allFrames[windowSize-1]) < thresholdLastFrame){

            resDTW = dtw(allGestures[i], testAction);
            //resDTW /= allFrames.size();
            allScores[i] = resDTW;

            if(resDTW < bestDTW){
                best = i; //save index for label
                bestDTW = resDTW;

            }
            // }
        }

        //Classifico (prendo costo minore)
        //classificationString = (bestDTW<500 ? allLabels[best] : "Unknown");
        classificationString = allLabels[best];

        emit (classificationDone());

        cout << "Classification: " << classificationString << ": " << bestDTW << "\t";

        //EVALUATION
        totTest++;
        if(allLabelsMSR[best] ==idActionTest){
            totRightClassified++;
            cout << "TRUE" << endl;
        }
        else{
            totWrongClassified++;
            cout << "FALSE" << endl;
        }


        //to do: gestione della sliding window, se necessaria
        if(bestDTW<thresholdDTW){

            //qDebug()<<allFrames.size();
            //allFrames.clear();
            //qDebug()<<allFrames.size();

//            // debug
//            for (int i1=0; i1<windowSize; ++i1){
//                for (int i2=0; i2<windowSize; ++i2){
//                    cout <<tab(i1,i2) << "\t";
//                }
//                cout<< "\n";
//            }
//            cout<< "\n";

        }
        else{

           // allFrames.erase(allFrames.begin());
            //allFrames.clear();
        }
    }//fine classificazione

    //Print output
    string fileResult = "DTWMSR/" + getNameDate() + "_" + ts.parameter.substr(7);
    ofstream outResults(fileResult);

    outResults << getNameDate() << endl;
    vector<string> outputAnnotations = ts.getAnnotations();
    for(auto i=0; i<outputAnnotations.size(); ++i)
        outResults << outputAnnotations[i] << endl;
    outResults << endl;
    outResults << "Total action for train: " << allGestures.size() << endl;
    outResults << "Total action tested: " << totTest << endl;
    outResults << "Right Classified: " << totRightClassified << endl;
    outResults << "Wrong Classified: " << totWrongClassified << endl;
    outResults << "Classification: " << ((float)totRightClassified/(float)totTest) << "%" << endl;

    outResults.close();
}
