#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <list>

#include "gesturerecognition.h"
#include "gesturerecognitionhmmoffline.h"
#include "frameset.h"
#include "dirent.h"

using namespace std;
using namespace cv;
using namespace gmmstd;

GestureRecognitionHMMOffline::GestureRecognitionHMMOffline(){

}

void GestureRecognitionHMMOffline::recognize(FrameSet &frame){

}

void createHMM(vector<vector<vector<double>>> allGesturesToTrain, int idAction, TrainingSet tr){

    //HMM parameters
    int numStates = 8;
    int dimFV = allGesturesToTrain[0][0].size();
    int numGauss = 3;

    typedef vector<vector<vector<double>>>::iterator iter_vm;
    const iter_vm vm_init = allGesturesToTrain.begin();
    const iter_vm vm_final = allGesturesToTrain.end();

    //Create new HMM
    cout << "New HMM created, label: " << idAction << endl;
    CHMM_GMM *phmm = new CHMM_GMM(numStates, dimFV, numGauss);

    //Initialize HMM
    cout << "HMM initialized." << endl;
    phmm->m_bLeftRight = true;
    phmm->Init_NotRandom2(vm_init->begin(), vm_init->end()); //to do: non trova not random

    //Train HMM
    cout << "Training..." << endl;
    int pniter;
    double plogprobinit, plogprobfinal;
    phmm->BaumWelch_Multiple(vm_init, vm_final, &pniter, &plogprobinit, &plogprobfinal);

    //Check probability values
    if(plogprobinit!=plogprobinit || plogprobfinal!=plogprobfinal){
        cout << "Problems with likelihood!" << endl;
    }

    //Store HMM
    cout << "Saving..." << endl << endl;
    string prefix = "./hmmOffline/" + tr.getTypeTest() + "/";
    string name = "hmm_";
    name.append(to_string(idAction));
    prefix.append(name);
    char *cstr = new char[prefix.length()+1];
    strcpy_s(cstr, prefix.length()+1, prefix.c_str());
    phmm->SaveToFile(cstr);

}

//Insert a value, return the avg value of 5 elements (used in getAVLMeanFeatures)
double InsertAndMean(list<double>& l, double val){

    int maxSize = 5;

    //Inserisco elemento mantenendo costante il numero
    if(l.size() < maxSize)
        l.push_back(val);
    else{
        l.pop_front();
        l.push_back(val);
    }

    //Calcolo la somma degli elementi
    list<double>::iterator it = l.begin();
    double sum = 0;
    for(int i=0;i<l.size();++i){
        advance(it, i);
        sum += *it;
    }
    return (sum/l.size());
}

//Features: offset with hip center
vector<double> getFeatures(FrameSet frame){

    vector<double> vFrameFeatures; //features calcolate per il singolo frame
    double dist;

    //Distanza centro spalle - mano destra
    dist = sqrt(pow(( frame.bodyJoint[FS_handRight].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_handRight].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_handRight].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - mano sinistra
    dist = sqrt(pow(( frame.bodyJoint[FS_handLeft].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_handLeft].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_handLeft].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - gomito destro
    dist = sqrt(pow(( frame.bodyJoint[FS_elbowRight].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_elbowRight].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_elbowRight].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - gomito sinistro
    dist = sqrt(pow(( frame.bodyJoint[FS_elbowLeft].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_elbowLeft].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_elbowLeft].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - spalla destra
    dist = sqrt(pow(( frame.bodyJoint[FS_shoulderRight].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_shoulderRight].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_shoulderRight].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - spalla sinistra
    dist = sqrt(pow(( frame.bodyJoint[FS_shoulderLeft].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_shoulderLeft].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_shoulderLeft].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - polso destro
    dist = sqrt(pow(( frame.bodyJoint[FS_wristRight].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_wristRight].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_wristRight].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza centro spalle - polso sinistro
    dist = sqrt(pow(( frame.bodyJoint[FS_wristLeft].x - frame.bodyJoint[FS_shoulderCenter].x),2) +
        pow(( frame.bodyJoint[FS_wristLeft].y - frame.bodyJoint[FS_shoulderCenter].y),2) +
        pow(( frame.bodyJoint[FS_wristLeft].z - frame.bodyJoint[FS_shoulderCenter].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza mano destra - mano sinistra
    dist = sqrt(pow(( frame.bodyJoint[FS_handRight].x - frame.bodyJoint[FS_handLeft].x),2) +
        pow(( frame.bodyJoint[FS_handRight].y - frame.bodyJoint[FS_handLeft].y),2) +
        pow(( frame.bodyJoint[FS_handRight].z - frame.bodyJoint[FS_handLeft].z),2));
    vFrameFeatures.push_back(dist);

    //Distanza gomito destro - gomito sinistro
    dist = sqrt(pow(( frame.bodyJoint[FS_elbowRight].x - frame.bodyJoint[FS_elbowLeft].x),2) +
        pow(( frame.bodyJoint[FS_elbowRight].y - frame.bodyJoint[FS_elbowLeft].y),2) +
        pow(( frame.bodyJoint[FS_elbowRight].z - frame.bodyJoint[FS_elbowLeft].z),2));
    vFrameFeatures.push_back(dist);

    //Normalizzo valori tra 0 e 1
    normalize(vFrameFeatures, vFrameFeatures, 1.0, -1.0, NORM_L1);

    return vFrameFeatures;
}

//Features: Acceleration, Velocity, Location of joints
vector<double> getAVLFeatures(FrameSet old2, FrameSet old1, FrameSet curr){

    vector<double> vFrameFeatures;
    double val;

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x) - (old1.bodyJoint[FS_handRight].x - old2.bodyJoint[FS_handRight].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y) - (old1.bodyJoint[FS_handRight].y - old2.bodyJoint[FS_handRight].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z) - (old1.bodyJoint[FS_handRight].z - old2.bodyJoint[FS_handRight].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_handRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);

    //-------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x) - (old1.bodyJoint[FS_handLeft].x - old2.bodyJoint[FS_handLeft].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y) - (old1.bodyJoint[FS_handLeft].y - old2.bodyJoint[FS_handLeft].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z) - (old1.bodyJoint[FS_handLeft].z - old2.bodyJoint[FS_handLeft].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_handLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_handLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //---------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x) - (old1.bodyJoint[FS_footRight].x - old2.bodyJoint[FS_footRight].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y) - (old1.bodyJoint[FS_footRight].y - old2.bodyJoint[FS_footRight].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z) - (old1.bodyJoint[FS_footRight].z - old2.bodyJoint[FS_footRight].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_footRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //-----------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x) - (old1.bodyJoint[FS_footLeft].x - old2.bodyJoint[FS_footLeft].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y) - (old1.bodyJoint[FS_footLeft].y - old2.bodyJoint[FS_footLeft].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z) - (old1.bodyJoint[FS_footLeft].z - old2.bodyJoint[FS_footLeft].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_footLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_footLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);

    //-----------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x) - (old1.bodyJoint[FS_head].x - old2.bodyJoint[FS_head].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y) - (old1.bodyJoint[FS_head].y - old2.bodyJoint[FS_head].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z) - (old1.bodyJoint[FS_head].z - old2.bodyJoint[FS_head].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_head].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].z - curr.bodyJoint[FS_hipCenter].z;

    normalize(vFrameFeatures, vFrameFeatures, 1.0, -1.0, NORM_L1);

    return vFrameFeatures;
}

//Features: Acceleration, Velocity, Location (means) of joints
vector<double> getAVLMeanFeatures(FrameSet old2, FrameSet old1, FrameSet curr, vector<list<double>>& vMean){

    vector<double> vFrameFeatures;
    double val;

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x) - (old1.bodyJoint[FS_handRight].x - old2.bodyJoint[FS_handRight].x);
    vFrameFeatures.push_back(InsertAndMean(vMean[0], val));
    val = (curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y) - (old1.bodyJoint[FS_handRight].y - old2.bodyJoint[FS_handRight].y);
    vFrameFeatures.push_back(InsertAndMean(vMean[1], val));
    val = (curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z) - (old1.bodyJoint[FS_handRight].z - old2.bodyJoint[FS_handRight].z);
    vFrameFeatures.push_back(InsertAndMean(vMean[2], val));

    //VELOCITA
    val = curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[3], val));
    val = curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[4], val));
    val = curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[5], val));

    //OFFSET
    val = curr.bodyJoint[FS_handRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[6], val));
    val = curr.bodyJoint[FS_handRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[7], val));
    val = curr.bodyJoint[FS_handRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[8], val));

    //-------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x) - (old1.bodyJoint[FS_handLeft].x - old2.bodyJoint[FS_handLeft].x);
    vFrameFeatures.push_back(InsertAndMean(vMean[9], val));
    val = (curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y) - (old1.bodyJoint[FS_handLeft].y - old2.bodyJoint[FS_handLeft].y);
    vFrameFeatures.push_back(InsertAndMean(vMean[10], val));
    val = (curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z) - (old1.bodyJoint[FS_handLeft].z - old2.bodyJoint[FS_handLeft].z);
    vFrameFeatures.push_back(InsertAndMean(vMean[11], val));

    //VELOCITA
    val = curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[12], val));
    val = curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[13], val));
    val = curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[14], val));

    //OFFSET
    val = curr.bodyJoint[FS_handLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[15], val));
    val = curr.bodyJoint[FS_handLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[16], val));
    val = curr.bodyJoint[FS_handLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[17], val));


    //---------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x) - (old1.bodyJoint[FS_footRight].x - old2.bodyJoint[FS_footRight].x);
    vFrameFeatures.push_back(InsertAndMean(vMean[18], val));
    val = (curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y) - (old1.bodyJoint[FS_footRight].y - old2.bodyJoint[FS_footRight].y);
    vFrameFeatures.push_back(InsertAndMean(vMean[19], val));
    val = (curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z) - (old1.bodyJoint[FS_footRight].z - old2.bodyJoint[FS_footRight].z);
    vFrameFeatures.push_back(InsertAndMean(vMean[20], val));

    //VELOCITA
    val = curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[21], val));
    val = curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[22], val));
    val = curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[23], val));

    //OFFSET
    val = curr.bodyJoint[FS_footRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[24], val));
    val = curr.bodyJoint[FS_footRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[25], val));
    val = curr.bodyJoint[FS_footRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[26], val));


    //-----------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x) - (old1.bodyJoint[FS_footLeft].x - old2.bodyJoint[FS_footLeft].x);
    vFrameFeatures.push_back(InsertAndMean(vMean[27], val));
    val = (curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y) - (old1.bodyJoint[FS_footLeft].y - old2.bodyJoint[FS_footLeft].y);
    vFrameFeatures.push_back(InsertAndMean(vMean[28], val));
    val = (curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z) - (old1.bodyJoint[FS_footLeft].z - old2.bodyJoint[FS_footLeft].z);
    vFrameFeatures.push_back(InsertAndMean(vMean[29], val));

    //VELOCITA
    val = curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[30], val));
    val = curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[31], val));
    val = curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[32], val));

    //OFFSET
    val = curr.bodyJoint[FS_footLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMean(vMean[33], val));
    val = curr.bodyJoint[FS_footLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMean(vMean[34], val));
    val = curr.bodyJoint[FS_footLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMean(vMean[35], val));

    //-----------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x) - (old1.bodyJoint[FS_head].x - old2.bodyJoint[FS_head].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y) - (old1.bodyJoint[FS_head].y - old2.bodyJoint[FS_head].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z) - (old1.bodyJoint[FS_head].z - old2.bodyJoint[FS_head].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_head].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].z - curr.bodyJoint[FS_hipCenter].z;

    normalize(vFrameFeatures, vFrameFeatures, 1.0, -1.0, NORM_L1);

    return vFrameFeatures;
}

//HMM TRAIN
void GestureRecognitionHMMOffline::Train(const TrainingSet &tr){

    int id;
    vector<vector<double>> singleGest; //accumulo i frame con features
    vector<vector<vector<double>>> allGesturesToTrain; //Accumulo azioni stessa classe per train
    int countNullFrame = 0;
    bool actionNull = false;

    vector<list<double>> vMean;
    vMean.resize(36);

    //For each action
    for(auto it = tr.begin(); it != tr.end(); ++it){

        //Store current id
        id = (*it).GetGestureId();

        //For each frame
        int k = 0;
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset){


            //Get features
            vector<double> tmpSingleFrame;
            //tmpSingleFrame = getFeatures((*itframeset));
            ++k;
            if(k > 3){
                tmpSingleFrame = getAVLFeatures(*(itframeset-2), *(itframeset-1), (*itframeset));
                //tmpSingleFrame = getAVLMeanFeatures(*(itframeset-2), *(itframeset-1), (*itframeset), vMean);



            //Check null features
            for(auto i=0;i<tmpSingleFrame.size();++i)
                if(tmpSingleFrame[i] == 0)
                    countNullFrame++;

                if(countNullFrame == tmpSingleFrame.size())
                    actionNull = true;
                //Store single frame features
                singleGest.push_back(tmpSingleFrame);
            }

        }

        //Store action
        if(!actionNull){
            allGesturesToTrain.push_back(singleGest);
        }
        else
            cout << "Features NULL, action discarded" << endl;

        actionNull = false;
        countNullFrame = 0;
        singleGest.clear();

        //Se prossima label diversa creo hmm, altrimenti accumulo
        if( (*(it+1)).GetGestureId() != id ){

            //Create new HMM
            createHMM(allGesturesToTrain, id, tr);

            //Clear vector with actions
            allGesturesToTrain.clear();

        }
    }
}

//Load hmm trained
vector<CHMM_GMM> loadHMM(string path, vector<string>& vTypeHMM, TestingSet ts){

    vector<CHMM_GMM> vHMM;
    DIR* dir;
    dirent* f;

    dir = opendir(path.c_str());
    if(!dir){
        cout  << "Error opening HMM folder!" << endl;
        exit(0);
    }

    while(f = readdir(dir)){
        string filename = f->d_name;
        if(filename.compare(".") != 0 && filename.compare("..") != 0){

            //Store HMM type /get only the number of label)
            vTypeHMM.push_back(filename.substr(4, filename.size() - filename.find_first_of("_")));

            CHMM_GMM* hmm = new CHMM_GMM(1, 1, 1);
            string prefix = "hmmOffline/" + ts.getTypeTest() + "/" + filename;
            char * hmmName = new char [prefix.length()+1];
            strcpy_s(hmmName, prefix.length()+1, prefix.c_str());

            //Load HMM
            if(!(hmm->LoadFromFile(hmmName)))
                cout << "Error loading HMM HMM\n" << endl;

            //Store HMM
            vHMM.push_back(*hmm);

            //Delete last HMM
            hmm->~CHMM_GMM();
        }
    }

    return vHMM;
}

//Create a name with the current data
string getNameDateS(){

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

//HMM TEST
void GestureRecognitionHMMOffline::Test(const TestingSet &ts){

    vector<vector<double>> singleGestToTest;
    double maxLogLk = DBL_MIN;
    int position = 0;
    int rightId;
    int countNullFrame = 0;
    bool actionNull = false;

    vector<list<double>> vMean;
    vMean.resize(36);

    //Performance
    int totTheoric = ts.size();
    int totTest = 0;
    int totRightClassified = 0;
    int totWrongClassified = 0;

    //Load all HMM trained
    vector<CHMM_GMM> vHMM;
    vector<string> vTypeHMM;
    vHMM = loadHMM("hmmOffline/"+ts.getTypeTest()+"/", vTypeHMM, ts);
    cout << "HMM loaded: " << vHMM.size() << endl;

    //For each action
    for(auto it = ts.begin(); it != ts.end(); ++it){

        rightId = (*it).GetGestureId();

        //For each frame
        int k=0;
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            vector<double> tmpSingleFrame;
            //tmpSingleFrame = getFeatures((*itframeset));
            k++;
            if(k > 3){
                tmpSingleFrame = getAVLFeatures(*(itframeset-2), *(itframeset-1), (*itframeset));
                //tmpSingleFrame = getAVLMeanFeatures(*(itframeset-2), *(itframeset-1), (*itframeset), vMean);

            //Check null features
            for(auto i=0;i<tmpSingleFrame.size();++i)
                if(tmpSingleFrame[i] == 0)
                    countNullFrame++;
            if(countNullFrame == tmpSingleFrame.size())
                actionNull = true;

            //Store single frame features
            singleGestToTest.push_back(tmpSingleFrame);
            }

        }

        //Classification only if the action is not null
        if(!actionNull){

            totTest++;

            //For each HMM
            for(auto i = 0; i<vHMM.size(); ++i){

                //Transition matrix
                Mat_<double> A;
                A = vHMM[i].m_A;

                typedef vector<vector<double>>::iterator iter_vf;
                const iter_vf ivf_init = singleGestToTest.begin();
                const iter_vf ivf_final = singleGestToTest.end();
                double loglikelihood = vHMM[i].LogLikelihood(ivf_init, ivf_final, &A);

                //Find best
                if(loglikelihood > maxLogLk){
                    position = i;
                    maxLogLk = loglikelihood;
                }
            }

            //Check classification
            cout << "GT: " << rightId << "\t" << "OUR: " << vTypeHMM[position] << "\t";
            if(vTypeHMM[position].compare(to_string(rightId)) == 0){
                cout << "TRUE" << endl;
                totRightClassified++;
            }
            else{
                cout << "FALSE" << endl;
                totWrongClassified++;
            }

            //Clear and reset all
            singleGestToTest.clear();
            maxLogLk = DBL_MIN;
            position = 0;

        }
        actionNull = false;
    }

    //Print output
    string fileResult = "DTWMSR/" + getNameDateS() + "_" + ts.parameter.substr(7);
    ofstream outResults(fileResult);

    outResults << getNameDateS() << endl;
    //outResults << "Mode: " << ts.parameter << endl;
    vector<string> outputAnnotations;
    outputAnnotations = ts.getAnnotations();
    for(auto i=0; i<outputAnnotations.size(); ++i)
        outResults << outputAnnotations[i] << endl;
    outResults << endl;
    outResults << "Total label for train: " << vHMM.size() << endl;
    outResults << "Theorical total action " << totTheoric << endl;
    outResults << "Total action tested: " << totTest << endl;
    outResults << "Right Classified: " << totRightClassified << endl;
    outResults << "Wrong Classified: " << totWrongClassified << endl;
    outResults << "Classification: " << ((float)totRightClassified/(float)totTest) << "%" << endl;

    outResults.close();
}


