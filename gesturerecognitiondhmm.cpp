#include "gesturerecognitiondhmm.h"
#include "dirent.h"
#include <time.h>
#include <fstream>

using namespace std;
using namespace cv;

static const unsigned int centroids = 10;

GestureRecognitionDHMM::GestureRecognitionDHMM()
{

}

void GestureRecognitionDHMM::recognize(FrameSet &frame){

}

double InsertAndMeanQ(list<double>& l, double val){

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


//double QuantMin[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
//double QuantMax[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

double QuantMin[]={-0.312031566,-0.363824223,-0.889426672,-0.288222033,-0.342135926,-0.656274878,-1.183286833,-1.517413062,-0.881622777,-0.224240801,-0.232495957,-0.894582439,-0.188003958,-0.193458053,-0.651920157,-0.875685286,-0.475690571,-0.673816942,-0.344159564,-0.52591729,-0.914772979,-0.384559059,-0.539828803,-0.712920561,-0.952179876,-1.847611106,-1.469784356,-0.213656487,-0.266083921,-0.866729378,-0.196634556,-0.251486135,-0.645934921,-0.394145009,-0.626064239,-1.020111177,-0.232602944,-0.419684834,-0.89714414,-0.170539303,-0.298820795,-0.649786589,-0.896209592,-1.848131506,-0.694324135,-0.215081752,-0.43307787,-0.955194945,-0.166430212,-0.316972923,-0.693930416,-0.850700833,-1.894958573,-0.828122436,-0.121410358,-0.193855728,-0.597449425,-0.158256756,-0.245164682,-0.825324054,-0.121410358,-0.193855728,-0.597449425};
 double QuantMax[]={0.311901122,0.363411823,0.890743853,0.288101423,0.342481791,0.655980698,0.818263179,1.259289309,0.492710069,0.224072837,0.232441034,0.895812112,0.187968302,0.193618388,0.651684599,0.428919487,0.713696493,0.462237474,0.34389921,0.525043287,0.916726595,0.384749978,0.540291046,0.712411026,1.359712193,1.989625952,0.912409476,0.213750359,0.265954976,0.867948182,0.19658127,0.251650372,0.645569304,0.834606826,0.992255483,0.715656604,0.232375041,0.419355153,0.898030499,0.170532933,0.298877927,0.64959107,0.720073932,-0.058137494,0.412237722,0.215032289,0.432759925,0.955894929,0.166442534,0.317138441,0.693711986,1.013986591,0.021292983,0.543481814,0.121378384,0.193985997,0.597402403,0.158314946,0.24539234,0.826249853,0.121378384,0.193985997,0.597402403};
 double QuantLev[]={30, 30, 30,
                   30, 30, 30,
                   10, 10, 10,

                   //ER
                   10, 10, 10,
                   10, 10, 10,
                   10, 10, 10,

                   //HL
                   30, 30, 30,
                   30, 30, 30,
                   10, 10, 10,

                   //EL
                   10, 10, 10,
                   10, 10, 10,
                   10, 10, 10,

                   //FR
                   10, 10, 10,
                   10, 10, 10,
                   10, 10, 10,

                   //FL
                   10, 10, 10,
                   10, 10, 10,
                   10, 10, 10,

                   //Head
                   10, 10, 10,
                   10, 10, 10,
                   10, 10, 10};

//double QuantMin[]={-0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5};
//double QuantMax[]={0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
//double QuantLev[]={10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};


vector<vector<unsigned int>> featureQuant2(vector<vector<double>> action){

    vector<unsigned int> quantSingleFrame;
    vector<vector<unsigned int>> quantSingleAction;

    //For each frame
    for (auto i=0; i<action.size(); ++i){

        //for each double value (36)
        for(auto j=0; j<action[0].size(); ++j){
            int iVal;
            double dVal = action[i][j];

            iVal = int(((dVal - QuantMin[j]) / (QuantMax[j] - QuantMin[j])) * QuantLev[j]);

            // soglia
            if (iVal<0) iVal=0;
            if (iVal >= QuantLev[j]) iVal= QuantLev[j]-1;

            quantSingleFrame.push_back(iVal);
        }


        //memorizzo frame
        quantSingleAction.push_back(quantSingleFrame);
        quantSingleFrame.clear();

    }//end frame

    //debug
    //        ofstream out("quant.txt");
    //        for(auto i=0;i<quantSingleAction.size();++i)
    //            out << quantSingleAction[i][0] << "\n";
    //        out.close();

    //getchar();
    return quantSingleAction;
}

int quant(float curVal, int k, float v){

    int q;

    for(auto i=-(k/2); i<(k/2); ++i){
        float down = v*i;
        float top = v*(i+1);
        if( (curVal >= down) && (curVal < top) ){
            q = i + (k/2);
            return q;
        }
    }
}

template <typename _T>
vector<vector<unsigned int>> featureQuant(vector<vector<double>> action, _T ts){

    vector<unsigned int> quantSingleFrame;
    vector<vector<unsigned int>> quantSingleAction;
    //vector<vector<vector<unsigned int>>> quantActions;


    //For each frame
    for (auto i=0; i<action.size(); ++i){

        //Lunghezza da quantizzare
        float l = (ts._l)*2;

        //Valore singola parte
        float v = l / (float)ts._k;//centroids;

        //Valore quanto
        int q;

        //for each double value (36)
        for(auto j=0; j<action[0].size(); ++j)
            quantSingleFrame.push_back(quant(action[i][j], ts._k, v));
        //cout << quant(action[i][8], ts._k, v) << "\t" << action[i][7] << endl;

        //memorizzo frame
        quantSingleAction.push_back(quantSingleFrame);
        quantSingleFrame.clear();

    }//end frame

    //debug
    //        ofstream out("quant.txt");
    //        for(auto i=0;i<quantSingleAction.size();++i)
    //            out << quantSingleAction[i][0] << "\n";
    //        out.close();

    //memorizzo azione
    //quantActions.push_back(quantSingleAction);
    //quantSingleAction.clear();


    //getchar();
    return quantSingleAction;
}

vector<double> getAVLFeaturesNoNorm(const FrameSet &old2, const FrameSet &old1, const FrameSet &curr){
    vector<double> vFrameFeatures;
    double val;

    //HAND RIGHT------------------------------------------------

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

//    //AREA TRIANGOLO
//    tripletPoint AB;
//    AB.x = curr.bodyJoint[FS_shoulderRight].x - curr.bodyJoint[FS_elbowRight].x;
//    AB.y = curr.bodyJoint[FS_shoulderRight].y - curr.bodyJoint[FS_elbowRight].y;
//    AB.z = curr.bodyJoint[FS_shoulderRight].z - curr.bodyJoint[FS_elbowRight].z;

//    tripletPoint AC;
//    AC.x = curr.bodyJoint[FS_handRight].x - curr.bodyJoint[FS_elbowRight].x;
//    AC.y = curr.bodyJoint[FS_handRight].y - curr.bodyJoint[FS_elbowRight].y;
//    AC.z = curr.bodyJoint[FS_handRight].z - curr.bodyJoint[FS_elbowRight].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);

//    double cos = (AB.x*AC.x + AB.y*AC.y) / (sqrt( (pow(AB.x, 2) + pow(AB.y, 2))*(pow(AC.x, 2) + pow(AC.y, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.z*AC.z) / (sqrt( (pow(AB.x, 2) + pow(AB.z, 2))*(pow(AC.x, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.y*AC.y + AB.z*AC.z) / (sqrt( (pow(AB.y, 2) + pow(AB.z, 2))*(pow(AC.y, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);





    //ELBOW RIGHT----------------------------------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_elbowRight].x - old1.bodyJoint[FS_elbowRight].x) - (old1.bodyJoint[FS_elbowRight].x - old2.bodyJoint[FS_elbowRight].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowRight].y - old1.bodyJoint[FS_elbowRight].y) - (old1.bodyJoint[FS_elbowRight].y - old2.bodyJoint[FS_elbowRight].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowRight].z - old1.bodyJoint[FS_elbowRight].z) - (old1.bodyJoint[FS_elbowRight].z - old2.bodyJoint[FS_elbowRight].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_elbowRight].x - old1.bodyJoint[FS_elbowRight].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].y - old1.bodyJoint[FS_elbowRight].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].z - old1.bodyJoint[FS_elbowRight].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_elbowRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //HAND LEFT--------------------------------------------------

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

    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_shoulderLeft].x - curr.bodyJoint[FS_elbowLeft].x;
//    AB.y = curr.bodyJoint[FS_shoulderLeft].y - curr.bodyJoint[FS_elbowLeft].y;
//    AB.z = curr.bodyJoint[FS_shoulderLeft].z - curr.bodyJoint[FS_elbowLeft].z;

//    AC.x = curr.bodyJoint[FS_handLeft].x - curr.bodyJoint[FS_elbowLeft].x;
//    AC.y = curr.bodyJoint[FS_handLeft].y - curr.bodyJoint[FS_elbowLeft].y;
//    AC.z = curr.bodyJoint[FS_handLeft].z - curr.bodyJoint[FS_elbowLeft].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.y*AC.y) / (sqrt( (pow(AB.x, 2) + pow(AB.y, 2))*(pow(AC.x, 2) + pow(AC.y, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.z*AC.z) / (sqrt( (pow(AB.x, 2) + pow(AB.z, 2))*(pow(AC.x, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.y*AC.y + AB.z*AC.z) / (sqrt( (pow(AB.y, 2) + pow(AB.z, 2))*(pow(AC.y, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);


    //ELBOW LEFT---------------------------------------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_elbowLeft].x - old1.bodyJoint[FS_elbowLeft].x) - (old1.bodyJoint[FS_elbowLeft].x - old2.bodyJoint[FS_elbowLeft].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowLeft].y - old1.bodyJoint[FS_elbowLeft].y) - (old1.bodyJoint[FS_elbowLeft].y - old2.bodyJoint[FS_elbowLeft].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowLeft].z - old1.bodyJoint[FS_elbowLeft].z) - (old1.bodyJoint[FS_elbowLeft].z - old2.bodyJoint[FS_elbowLeft].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_elbowLeft].x - old1.bodyJoint[FS_elbowLeft].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].y - old1.bodyJoint[FS_elbowLeft].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].z - old1.bodyJoint[FS_elbowLeft].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_elbowLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //FOOT RIGHT---------------------------------------------

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

//    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_hipRight].x - curr.bodyJoint[FS_kneeRight].x;
//    AB.y = curr.bodyJoint[FS_hipRight].y - curr.bodyJoint[FS_kneeRight].y;
//    AB.z = curr.bodyJoint[FS_hipRight].z - curr.bodyJoint[FS_kneeRight].z;

//    AC.x = curr.bodyJoint[FS_footRight].x - curr.bodyJoint[FS_kneeRight].x;
//    AC.y = curr.bodyJoint[FS_footRight].y - curr.bodyJoint[FS_kneeRight].y;
//    AC.z = curr.bodyJoint[FS_footRight].z - curr.bodyJoint[FS_kneeRight].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);


    //KNEE RIGHT---------------------------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_kneeRight].x - old1.bodyJoint[FS_kneeRight].x) - (old1.bodyJoint[FS_kneeRight].x - old2.bodyJoint[FS_kneeRight].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeRight].y - old1.bodyJoint[FS_kneeRight].y) - (old1.bodyJoint[FS_kneeRight].y - old2.bodyJoint[FS_kneeRight].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeRight].z - old1.bodyJoint[FS_kneeRight].z) - (old1.bodyJoint[FS_kneeRight].z - old2.bodyJoint[FS_kneeRight].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_kneeRight].x - old1.bodyJoint[FS_kneeRight].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].y - old1.bodyJoint[FS_kneeRight].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].z - old1.bodyJoint[FS_kneeRight].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_kneeRight].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].z - curr.bodyJoint[FS_hipCenter].z;
//    vFrameFeatures.push_back(val);


    //FOOT LEFT------------------------------------

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

//    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_hipLeft].x - curr.bodyJoint[FS_kneeLeft].x;
//    AB.y = curr.bodyJoint[FS_hipLeft].y - curr.bodyJoint[FS_kneeLeft].y;
//    AB.z = curr.bodyJoint[FS_hipLeft].z - curr.bodyJoint[FS_kneeLeft].z;

//    AC.x = curr.bodyJoint[FS_footLeft].x - curr.bodyJoint[FS_kneeLeft].x;
//    AC.y = curr.bodyJoint[FS_footLeft].y - curr.bodyJoint[FS_kneeLeft].y;
//    AC.z = curr.bodyJoint[FS_footLeft].z - curr.bodyJoint[FS_kneeLeft].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);


    //KNEE LEFT------------------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_kneeLeft].x - old1.bodyJoint[FS_kneeLeft].x) - (old1.bodyJoint[FS_kneeLeft].x - old2.bodyJoint[FS_kneeLeft].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeLeft].y - old1.bodyJoint[FS_kneeLeft].y) - (old1.bodyJoint[FS_kneeLeft].y - old2.bodyJoint[FS_kneeLeft].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeLeft].z - old1.bodyJoint[FS_kneeLeft].z) - (old1.bodyJoint[FS_kneeLeft].z - old2.bodyJoint[FS_kneeLeft].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_kneeLeft].x - old1.bodyJoint[FS_kneeLeft].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].y - old1.bodyJoint[FS_kneeLeft].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].z - old1.bodyJoint[FS_kneeLeft].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_kneeLeft].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].z - curr.bodyJoint[FS_hipCenter].z;
//    vFrameFeatures.push_back(val);


    //HEAD------------------------------------------

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x) - (old1.bodyJoint[FS_head].x - old2.bodyJoint[FS_head].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y) - (old1.bodyJoint[FS_head].y - old2.bodyJoint[FS_head].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z) - (old1.bodyJoint[FS_head].z - old2.bodyJoint[FS_head].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);

//        //OFFSET
//        val = curr.bodyJoint[FS_head].x - curr.bodyJoint[FS_hipCenter].x;
//        vFrameFeatures.push_back(val);
//        val = curr.bodyJoint[FS_head].y - curr.bodyJoint[FS_hipCenter].y;
//        vFrameFeatures.push_back(val);
//        val = curr.bodyJoint[FS_head].z - curr.bodyJoint[FS_hipCenter].z;


    return vFrameFeatures;
}

vector<double> getAVLFeaturesQ(const FrameSet &old2, const FrameSet &old1, const FrameSet &curr){
    vector<double> vFrameFeatures;
    double val;

    //HAND RIGHT------------------------------------------------

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

//    //AREA TRIANGOLO
//    tripletPoint AB;
//    AB.x = curr.bodyJoint[FS_shoulderRight].x - curr.bodyJoint[FS_elbowRight].x;
//    AB.y = curr.bodyJoint[FS_shoulderRight].y - curr.bodyJoint[FS_elbowRight].y;
//    AB.z = curr.bodyJoint[FS_shoulderRight].z - curr.bodyJoint[FS_elbowRight].z;

//    tripletPoint AC;
//    AC.x = curr.bodyJoint[FS_handRight].x - curr.bodyJoint[FS_elbowRight].x;
//    AC.y = curr.bodyJoint[FS_handRight].y - curr.bodyJoint[FS_elbowRight].y;
//    AC.z = curr.bodyJoint[FS_handRight].z - curr.bodyJoint[FS_elbowRight].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);

//    double cos = (AB.x*AC.x + AB.y*AC.y) / (sqrt( (pow(AB.x, 2) + pow(AB.y, 2))*(pow(AC.x, 2) + pow(AC.y, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.z*AC.z) / (sqrt( (pow(AB.x, 2) + pow(AB.z, 2))*(pow(AC.x, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.y*AC.y + AB.z*AC.z) / (sqrt( (pow(AB.y, 2) + pow(AB.z, 2))*(pow(AC.y, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);





    //ELBOW RIGHT----------------------------------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_elbowRight].x - old1.bodyJoint[FS_elbowRight].x) - (old1.bodyJoint[FS_elbowRight].x - old2.bodyJoint[FS_elbowRight].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowRight].y - old1.bodyJoint[FS_elbowRight].y) - (old1.bodyJoint[FS_elbowRight].y - old2.bodyJoint[FS_elbowRight].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowRight].z - old1.bodyJoint[FS_elbowRight].z) - (old1.bodyJoint[FS_elbowRight].z - old2.bodyJoint[FS_elbowRight].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_elbowRight].x - old1.bodyJoint[FS_elbowRight].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].y - old1.bodyJoint[FS_elbowRight].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].z - old1.bodyJoint[FS_elbowRight].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_elbowRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //HAND LEFT--------------------------------------------------

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

    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_shoulderLeft].x - curr.bodyJoint[FS_elbowLeft].x;
//    AB.y = curr.bodyJoint[FS_shoulderLeft].y - curr.bodyJoint[FS_elbowLeft].y;
//    AB.z = curr.bodyJoint[FS_shoulderLeft].z - curr.bodyJoint[FS_elbowLeft].z;

//    AC.x = curr.bodyJoint[FS_handLeft].x - curr.bodyJoint[FS_elbowLeft].x;
//    AC.y = curr.bodyJoint[FS_handLeft].y - curr.bodyJoint[FS_elbowLeft].y;
//    AC.z = curr.bodyJoint[FS_handLeft].z - curr.bodyJoint[FS_elbowLeft].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.y*AC.y) / (sqrt( (pow(AB.x, 2) + pow(AB.y, 2))*(pow(AC.x, 2) + pow(AC.y, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.x*AC.x + AB.z*AC.z) / (sqrt( (pow(AB.x, 2) + pow(AB.z, 2))*(pow(AC.x, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);

//    cos = (AB.y*AC.y + AB.z*AC.z) / (sqrt( (pow(AB.y, 2) + pow(AB.z, 2))*(pow(AC.y, 2) + pow(AC.z, 2)) ));
//    val = acos(cos);
//    vFrameFeatures.push_back(val);


    //ELBOW LEFT---------------------------------------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_elbowLeft].x - old1.bodyJoint[FS_elbowLeft].x) - (old1.bodyJoint[FS_elbowLeft].x - old2.bodyJoint[FS_elbowLeft].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowLeft].y - old1.bodyJoint[FS_elbowLeft].y) - (old1.bodyJoint[FS_elbowLeft].y - old2.bodyJoint[FS_elbowLeft].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_elbowLeft].z - old1.bodyJoint[FS_elbowLeft].z) - (old1.bodyJoint[FS_elbowLeft].z - old2.bodyJoint[FS_elbowLeft].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_elbowLeft].x - old1.bodyJoint[FS_elbowLeft].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].y - old1.bodyJoint[FS_elbowLeft].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].z - old1.bodyJoint[FS_elbowLeft].z;
    vFrameFeatures.push_back(val);

    //OFFSET
    val = curr.bodyJoint[FS_elbowLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_elbowLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(val);


    //FOOT RIGHT---------------------------------------------


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

//    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_hipRight].x - curr.bodyJoint[FS_kneeRight].x;
//    AB.y = curr.bodyJoint[FS_hipRight].y - curr.bodyJoint[FS_kneeRight].y;
//    AB.z = curr.bodyJoint[FS_hipRight].z - curr.bodyJoint[FS_kneeRight].z;

//    AC.x = curr.bodyJoint[FS_footRight].x - curr.bodyJoint[FS_kneeRight].x;
//    AC.y = curr.bodyJoint[FS_footRight].y - curr.bodyJoint[FS_kneeRight].y;
//    AC.z = curr.bodyJoint[FS_footRight].z - curr.bodyJoint[FS_kneeRight].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);


    //KNEE RIGHT---------------------------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_kneeRight].x - old1.bodyJoint[FS_kneeRight].x) - (old1.bodyJoint[FS_kneeRight].x - old2.bodyJoint[FS_kneeRight].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeRight].y - old1.bodyJoint[FS_kneeRight].y) - (old1.bodyJoint[FS_kneeRight].y - old2.bodyJoint[FS_kneeRight].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeRight].z - old1.bodyJoint[FS_kneeRight].z) - (old1.bodyJoint[FS_kneeRight].z - old2.bodyJoint[FS_kneeRight].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_kneeRight].x - old1.bodyJoint[FS_kneeRight].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].y - old1.bodyJoint[FS_kneeRight].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].z - old1.bodyJoint[FS_kneeRight].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_kneeRight].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeRight].z - curr.bodyJoint[FS_hipCenter].z;
//    vFrameFeatures.push_back(val);


    //FOOT LEFT------------------------------------

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

//    //AREA TRIANGOLO
//    AB.x = curr.bodyJoint[FS_hipLeft].x - curr.bodyJoint[FS_kneeLeft].x;
//    AB.y = curr.bodyJoint[FS_hipLeft].y - curr.bodyJoint[FS_kneeLeft].y;
//    AB.z = curr.bodyJoint[FS_hipLeft].z - curr.bodyJoint[FS_kneeLeft].z;

//    AC.x = curr.bodyJoint[FS_footLeft].x - curr.bodyJoint[FS_kneeLeft].x;
//    AC.y = curr.bodyJoint[FS_footLeft].y - curr.bodyJoint[FS_kneeLeft].y;
//    AC.z = curr.bodyJoint[FS_footLeft].z - curr.bodyJoint[FS_kneeLeft].z;

//    val = 0.5 * (AB.y*AC.z - AB.z*AC.y - AB.x*AC.z + AB.z*AC.x + AB.x*AC.y - AB.y*AC.x);
//    vFrameFeatures.push_back(val);


    //KNEE LEFT------------------------------------

//    //ACCELERAZIONE
//    val = (curr.bodyJoint[FS_kneeLeft].x - old1.bodyJoint[FS_kneeLeft].x) - (old1.bodyJoint[FS_kneeLeft].x - old2.bodyJoint[FS_kneeLeft].x);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeLeft].y - old1.bodyJoint[FS_kneeLeft].y) - (old1.bodyJoint[FS_kneeLeft].y - old2.bodyJoint[FS_kneeLeft].y);
//    vFrameFeatures.push_back(val);
//    val = (curr.bodyJoint[FS_kneeLeft].z - old1.bodyJoint[FS_kneeLeft].z) - (old1.bodyJoint[FS_kneeLeft].z - old2.bodyJoint[FS_kneeLeft].z);
//    vFrameFeatures.push_back(val);

//    //VELOCITA
//    val = curr.bodyJoint[FS_kneeLeft].x - old1.bodyJoint[FS_kneeLeft].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].y - old1.bodyJoint[FS_kneeLeft].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].z - old1.bodyJoint[FS_kneeLeft].z;
//    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_kneeLeft].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_kneeLeft].z - curr.bodyJoint[FS_hipCenter].z;
//    vFrameFeatures.push_back(val);


    //HEAD------------------------------------------

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x) - (old1.bodyJoint[FS_head].x - old2.bodyJoint[FS_head].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y) - (old1.bodyJoint[FS_head].y - old2.bodyJoint[FS_head].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z) - (old1.bodyJoint[FS_head].z - old2.bodyJoint[FS_head].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);


    /*
        //OFFSET
        val = curr.bodyJoint[FS_head].x - curr.bodyJoint[FS_hipCenter].x;
        vFrameFeatures.push_back(val);
        val = curr.bodyJoint[FS_head].y - curr.bodyJoint[FS_hipCenter].y;
        vFrameFeatures.push_back(val);
       val = curr.bodyJoint[FS_head].z - curr.bodyJoint[FS_hipCenter].z;
*/
    normalize(vFrameFeatures, vFrameFeatures, 1.0, -1.0, NORM_L1);

    return vFrameFeatures;
}

vector<double> getAVLMeanFeaturesQ(FrameSet old2, FrameSet old1, FrameSet curr, vector<list<double>>& vMean){

    vector<double> vFrameFeatures;
    double val;

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x) - (old1.bodyJoint[FS_handRight].x - old2.bodyJoint[FS_handRight].x);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[0], val));
    val = (curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y) - (old1.bodyJoint[FS_handRight].y - old2.bodyJoint[FS_handRight].y);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[1], val));
    val = (curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z) - (old1.bodyJoint[FS_handRight].z - old2.bodyJoint[FS_handRight].z);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[2], val));

    //VELOCITA
    val = curr.bodyJoint[FS_handRight].x - old1.bodyJoint[FS_handRight].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[3], val));
    val = curr.bodyJoint[FS_handRight].y - old1.bodyJoint[FS_handRight].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[4], val));
    val = curr.bodyJoint[FS_handRight].z - old1.bodyJoint[FS_handRight].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[5], val));

    //OFFSET
    val = curr.bodyJoint[FS_handRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[6], val));
    val = curr.bodyJoint[FS_handRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[7], val));
    val = curr.bodyJoint[FS_handRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[8], val));

    //-------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x) - (old1.bodyJoint[FS_handLeft].x - old2.bodyJoint[FS_handLeft].x);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[9], val));
    val = (curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y) - (old1.bodyJoint[FS_handLeft].y - old2.bodyJoint[FS_handLeft].y);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[10], val));
    val = (curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z) - (old1.bodyJoint[FS_handLeft].z - old2.bodyJoint[FS_handLeft].z);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[11], val));

    //VELOCITA
    val = curr.bodyJoint[FS_handLeft].x - old1.bodyJoint[FS_handLeft].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[12], val));
    val = curr.bodyJoint[FS_handLeft].y - old1.bodyJoint[FS_handLeft].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[13], val));
    val = curr.bodyJoint[FS_handLeft].z - old1.bodyJoint[FS_handLeft].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[14], val));

    //OFFSET
    val = curr.bodyJoint[FS_handLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[15], val));
    val = curr.bodyJoint[FS_handLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[16], val));
    val = curr.bodyJoint[FS_handLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[17], val));


    //---------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x) - (old1.bodyJoint[FS_footRight].x - old2.bodyJoint[FS_footRight].x);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[18], val));
    val = (curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y) - (old1.bodyJoint[FS_footRight].y - old2.bodyJoint[FS_footRight].y);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[19], val));
    val = (curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z) - (old1.bodyJoint[FS_footRight].z - old2.bodyJoint[FS_footRight].z);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[20], val));

    //VELOCITA
    val = curr.bodyJoint[FS_footRight].x - old1.bodyJoint[FS_footRight].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[21], val));
    val = curr.bodyJoint[FS_footRight].y - old1.bodyJoint[FS_footRight].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[22], val));
    val = curr.bodyJoint[FS_footRight].z - old1.bodyJoint[FS_footRight].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[23], val));

    //OFFSET
    val = curr.bodyJoint[FS_footRight].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[24], val));
    val = curr.bodyJoint[FS_footRight].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[25], val));
    val = curr.bodyJoint[FS_footRight].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[26], val));


    //-----------------------

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x) - (old1.bodyJoint[FS_footLeft].x - old2.bodyJoint[FS_footLeft].x);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[27], val));
    val = (curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y) - (old1.bodyJoint[FS_footLeft].y - old2.bodyJoint[FS_footLeft].y);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[28], val));
    val = (curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z) - (old1.bodyJoint[FS_footLeft].z - old2.bodyJoint[FS_footLeft].z);
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[29], val));

    //VELOCITA
    val = curr.bodyJoint[FS_footLeft].x - old1.bodyJoint[FS_footLeft].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[30], val));
    val = curr.bodyJoint[FS_footLeft].y - old1.bodyJoint[FS_footLeft].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[31], val));
    val = curr.bodyJoint[FS_footLeft].z - old1.bodyJoint[FS_footLeft].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[32], val));

    //OFFSET
    val = curr.bodyJoint[FS_footLeft].x - curr.bodyJoint[FS_hipCenter].x;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[33], val));
    val = curr.bodyJoint[FS_footLeft].y - curr.bodyJoint[FS_hipCenter].y;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[34], val));
    val = curr.bodyJoint[FS_footLeft].z - curr.bodyJoint[FS_hipCenter].z;
    vFrameFeatures.push_back(InsertAndMeanQ(vMean[35], val));

    //-----------------------------

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);

    //ACCELERAZIONE
    val = (curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x) - (old1.bodyJoint[FS_head].x - old2.bodyJoint[FS_head].x);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y) - (old1.bodyJoint[FS_head].y - old2.bodyJoint[FS_head].y);
    vFrameFeatures.push_back(val);
    val = (curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z) - (old1.bodyJoint[FS_head].z - old2.bodyJoint[FS_head].z);
    vFrameFeatures.push_back(val);

    //VELOCITA
    val = curr.bodyJoint[FS_head].x - old1.bodyJoint[FS_head].x;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].y - old1.bodyJoint[FS_head].y;
    vFrameFeatures.push_back(val);
    val = curr.bodyJoint[FS_head].z - old1.bodyJoint[FS_head].z;
    vFrameFeatures.push_back(val);

//    //OFFSET
//    val = curr.bodyJoint[FS_head].x - curr.bodyJoint[FS_hipCenter].x;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].y - curr.bodyJoint[FS_hipCenter].y;
//    vFrameFeatures.push_back(val);
//    val = curr.bodyJoint[FS_head].z - curr.bodyJoint[FS_hipCenter].z;


    normalize(vFrameFeatures, vFrameFeatures, 1.0, -1.0, NORM_L1);

    return vFrameFeatures;
}

vector<vector<unsigned int>> featureQuantKMeans(vector<vector<double>> action){

    Mat labels;
    vector<unsigned int> labelFrame;
    vector<vector<unsigned int>> labelAction;
    int k = centroids;

    //per ogni frame
    for(auto i=0; i<action.size(); ++i){

        Mat matAction(action[0].size(), 1, CV_32F);

        for(auto j=0; j<action[0].size(); ++j)
            matAction.at<float>(j, 0) = action[i][j];

       kmeans(matAction, k, labels, TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0), 3, KMEANS_PP_CENTERS);

        //TO DO: controllo valori negativi
        for(int r=0; r<labels.rows; ++r)
            for(int c=0; c<labels.cols; ++c)
                qDebug() << labels.at<int>(r,c) << " ";

        //TO DO: controllo conversione unsigned int
        for(auto j=0; j<labels.rows; ++j)
            labelFrame.push_back(labels.at<int>(j, 0));

        labelAction.push_back(labelFrame);
        labelFrame.clear();
    }

    return labelAction;

}

void createHMM(vector<vector<vector<unsigned int>>> allGesturesToTrain, int idAction, unsigned int levels, string subfolder, vector<double> vWeigths){

    //HMM parameters
    int numStates = 8;
    vector<unsigned int> quantlevels;//={levels, levels, levels, levels};
    for(auto i=0; i<allGesturesToTrain[0][0].size(); ++i)//allGesturesToTrain[0][0].size()
    // for(auto i=0; i<30; ++i)//allGesturesToTrain[0][0].size()
        //quantlevels.push_back(QuantLev[i]);
        quantlevels.push_back(levels);

    typedef vector<vector<vector<unsigned int>>>::iterator iter_vm;
    const iter_vm init = allGesturesToTrain.begin();
    const iter_vm final = allGesturesToTrain.end();

    //Create new HMM
    Cmsdhmm_hmm dhmm(numStates, quantlevels, true, 2);

    cout << "New HMM created, label: " << idAction << endl;

    //Inizializzazione deterministica
    dhmm.m_bLeftRight = true;
    dhmm.Init_Deterministic(init->begin(), init->end()); //to do: non trova not random
    cout << "HMM initialized." << endl;

    // set dei pesi (DOPO L'init...)
    dhmm.SetWeights(vWeigths);

    //Train HMM
    cout << "Training..." << endl;
    int pniter;
    double plogprobinit, plogprobfinal;
    dhmm.BaumWelch_Multiple(init, final, &pniter, &plogprobinit, &plogprobfinal);

    //Check probability values
    if(plogprobinit!=plogprobinit || plogprobfinal!=plogprobfinal){
        cout << "Problems with likelihood!" << endl;
    }

    //Store HMM (typetest settato durante load dei file con azioni)
    cout << "Saving..." << endl << endl;
    string prefix = "./dhmmOffline/" + subfolder + "/";
    string name = "hmm_";
    char strIdAction[10];
    sprintf(strIdAction,"%02d",idAction);
    //name.append(to_string(idAction));
    name.append(strIdAction);
    prefix.append(name);

    char *cstr = new char[prefix.length()+1];
    strcpy_s(cstr, prefix.length()+1, prefix.c_str());
    dhmm.SaveToFile(cstr);

    //Memorizzo i pesi
    ofstream wOut("weights/"+name+".txt");
    for(auto i=0;i<vWeigths.size();++i)
        wOut << vWeigths[i] << endl;
}

int GetFeatureSetSize(const vector<vector<vector<unsigned int>>> &allGesturesToTrainQuant) {
    return ((allGesturesToTrainQuant[0])[0]).size();
}

vector<double> ComputeWeights3(const vector<vector<vector<double>>> &allGesturesToTrain, int id){

    vector<double> vWeights;
    int featureSize = allGesturesToTrain[0][0].size();
    vWeights.resize(featureSize, 1);

    if(id == 2){
        cout << "primo" << endl;
        for(auto i=0;i<18;++i)
            vWeights[i] = 0.5;

        for(auto i=18;i<36;++i)
            vWeights[i] = 0.1;

        for(auto i=36;i<45;++i)
            vWeights[i] = 0.9;

        for(auto i=45;i<54;++i)
            vWeights[i] = 0.9;

        for(auto i=54;i<63;++i)
            vWeights[i] = 0.9;
    }

    else{

        for(auto i=0;i<18;++i)
            vWeights[i] = 1-0.82;

        for(auto i=18;i<36;++i)
            vWeights[i] = 1-0.91;

        for(auto i=36;i<45;++i)
            vWeights[i] = 1-0.72;

        for(auto i=45;i<54;++i)
            vWeights[i] = 1-0.65;

        for(auto i=54;i<63;++i)
            vWeights[i] = 1-0.78;

    }

    return vWeights;

}

vector<double> ComputeWeights2(const vector<vector<vector<double>>> &allGesturesToTrain){

    //TODO: lavoro con le feat NON normalizzate?

    vector<double> vWeights;
    int featureSize = allGesturesToTrain[0][0].size();
    vWeights.resize(featureSize, 1); //di base lascio invariata la prob di emissione

    vector<int> votes;
    votes.resize(featureSize, 0);

    //TODO: pensare a come calcolare la soglia
    double VHTh = 0.3; //soglia velocità mani
    double VFTh = 0.3; //soglia velocità piedi
    double wUP = 0.45;
    double VHR = 0.0, VHL = 0.0, LHR = 0.0, LHL = 0.0, VFR = 0.0, VFL = 0.0, LFR = 0.0, LFL = 0.0;

    //per ogni azione
    for(auto i=0; i<allGesturesToTrain.size();++i){

        //per ogni frame
        for(auto j=0; j<allGesturesToTrain[i].size();++j){

            //VEL HANDS
            VHR += allGesturesToTrain[i][j][3] + allGesturesToTrain[i][j][4] + allGesturesToTrain[i][j][5];
            VHL += allGesturesToTrain[i][j][21] + allGesturesToTrain[i][j][22] + allGesturesToTrain[i][j][23];

            //VEL FOOTS
            VFR += allGesturesToTrain[i][j][39] + allGesturesToTrain[i][j][40] + allGesturesToTrain[i][j][41];
            VFL += allGesturesToTrain[i][j][48] + allGesturesToTrain[i][j][49] + allGesturesToTrain[i][j][50];

        }//frame

        //Normalizzo per numero di frame
        //VHR /= (float)allGesturesToTrain[i].size();
        //VHL /= (float)allGesturesToTrain[i].size();
        //VFR /= (float)allGesturesToTrain[i].size();
        //VFL /= (float)allGesturesToTrain[i].size();

        //VEL HANDS
        if(VHR > VHL){
            if((VHR-VHL) > VHTh){
                votes[3]++;
                votes[4]++;
                votes[5]++;
            }
        }
        else if(VHR < VHL){
            if((VHL-VHR) > VHTh){
                votes[21]++;
                votes[22]++;
                votes[23]++;
            }
        }


        //VEL FOOTS
        if(VFR > VFL){
            if((VFR-VFL) > VFTh){
                votes[39]++;
                votes[40]++;
                votes[41]++;
            }
        }
        else if(VFR < VFL){
            if((VFL-VFR) > VFTh){
                votes[48]++;
                votes[49]++;
                votes[50]++;
            }
        }

    }//azione

    for(auto i=0;i<featureSize;++i){
        //qDebug() << votes[i] << "\t" << (allGesturesToTrain.size()/2) << endl;
        if(votes[i] > (allGesturesToTrain.size()/2))
            vWeights[i] = wUP;
    }


    return vWeights;

}

vector<double> ComputeWeights(const vector<vector<vector<double>>> &allGesturesToTrain, int type){

    // calcolo di pesi in base al movimento?!?!
    vector<double> vWeights;
    int featureSize = allGesturesToTrain[0][0].size();//GetFeatureSetSize(allGesturesToTrain);
    vWeights.resize(featureSize,1);  // default pesi ad 1

    // soluzione 1

    vector<double> vAccFinal;
    vAccFinal.resize(featureSize, 0);

    // k: 0-63
    // j: per ogni frame
    // i: per ogni video

    //per ogni azione
    for(auto i=0; i<allGesturesToTrain.size();++i){

        vector<double> vAccFeat;
        vAccFeat.resize(featureSize, 0);

        //per ogni frame
        for(auto j=0; j<allGesturesToTrain[i].size();++j){

            //per ogni feature
            for(auto k=0;k<allGesturesToTrain[i][j].size();++k){
                vAccFeat[k] += abs(allGesturesToTrain[i][j][k]);
            }
        }

        //ofstream out("weights/actions/"+to_string(type)+to_string(i+1)+".txt");
//        ofstream out("weights/actions/valori.txt", std::ostream::app);
//            out << type << "\t"  << i+1 <<  "\t";

//        for(auto z=0;z<vAccFeat.size();++z)
//            out << vAccFeat[z] << "\t";

//        out << endl;
//        out.close();

        //Memorizzo i normalizzati (divido per il numero di frame)
        for(auto k=0; k<vAccFinal.size();++k)
            vAccFinal[k] += (vAccFeat[k] / (float)allGesturesToTrain[i].size());
    }

    //Faccio la media tra tutte le azioni
    for(auto k=0; k<vAccFinal.size();++k){
        vAccFinal[k] /= (float)allGesturesToTrain.size();
        vAccFinal[k] = 1 - vAccFinal[k];
    }

    //normalize(vAccFinal, vAccFinal, 1.0, 0.0, NORM_L1);

    return vAccFinal;
}

void GestureRecognitionDHMM::Train(const TrainingSet &tr){

    int id;
    vector<vector<double>> singleGest; //accumulo i frame con features
    vector<vector<vector<double>>> allGesturesToTrain; //Accumulo azioni stessa classe per train

    vector<vector<double>> singleGestNN;
    vector<vector<vector<double>>> allGesturesToTrainNN;

    int countNullFrame = 0;
    bool actionNull = false;

    vector<list<double>> vMean;
    vMean.resize(36);


    //For each action
    for(auto it = tr.begin(); it != tr.end(); ++it){

        // -----------------------------------------------------------
        // FASE 1 - Calcolo delle features
        // -----------------------------------------------------------

        //Store current id
        id = (*it).GetGestureId();

        // sequence is null by default
        actionNull = true;

        //For each frame
        int k = 0;
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset){


            //Get features
            vector<double> tmpSingleFrame;
            vector<double> tmpSingleFrameNN;
            //tmpSingleFrame = getFeatures((*itframeset));
            ++k;
            if(k > 3){
                tmpSingleFrame = getAVLFeaturesQ(*(itframeset-2), *(itframeset-1), (*itframeset));
                tmpSingleFrameNN = getAVLFeaturesNoNorm(*(itframeset-2), *(itframeset-1), (*itframeset));
                //tmpSingleFrame = getAVLMeanFeaturesQ(*(itframeset-2), *(itframeset-1), (*itframeset), vMean);



                //Check null features
                for(auto i=0;i<tmpSingleFrame.size();++i)
                    if(tmpSingleFrame[i] == 0)
                        countNullFrame++;

                if(countNullFrame <= (tmpSingleFrame.size()/2))
                    actionNull = false;

                //Store single frame features
                singleGest.push_back(tmpSingleFrame);
                singleGestNN.push_back(tmpSingleFrameNN);
            }

        }

        //Store action
        if(!actionNull){
            allGesturesToTrain.push_back(singleGest);
            allGesturesToTrainNN.push_back(singleGestNN);
        }
        else
            cout << "Features NULL, action discarded" << endl;

        actionNull = false;
        countNullFrame = 0;
        singleGest.clear();
        singleGestNN.clear();

        //Se prossima label diversa creo hmm, altrimenti accumulo

        if ((((it+1) != tr.end()) &&  (*(it+1)).GetGestureId() != id ) ||
                (it+1) == tr.end())
        {

            // ---------------------------------------------------------
            // QUI HO LA COLLEZIONE DELLE FEATURES PER TUTTE LE ISTANZE DEL GESTO/LABEL CORRENTE
            // FASE 2: training
            // ---------------------------------------------------------



            // FASE 2.1:  QUANTIZZAZIONE (double -> unsigned int)
            vector<vector<vector<unsigned int>>> allGesturesToTrainQuant;
            vector<vector<unsigned int>> singleGestureToTrainQuant;
            for(auto i=0; i<allGesturesToTrain.size(); ++i){
                // quantizzazione
                singleGestureToTrainQuant = featureQuant(allGesturesToTrain[i], tr);
                //singleGestureToTrainQuant = featureQuant2(allGesturesToTrain[i]);
                // aggiungo a collezione
                allGesturesToTrainQuant.push_back(singleGestureToTrainQuant);
                singleGestureToTrainQuant.clear();
            }
            //in modo da passare un vector<vector<uint>> e fare un unico metodo anche per il test
            unsigned int levels; //se sono uguali tutti i livelli


            int featuresize = GetFeatureSetSize(allGesturesToTrainQuant);

            // FASE 2.2:  CALCOLO PESI
            vector<double> vWeights;

            // todo: a scelta
            vWeights.resize(featuresize, 1);  // default pesi ad 1
            //vWeights = ComputeWeights2(allGesturesToTrain);
            //vWeights = ComputeWeights2(allGesturesToTrainNN);
            //vWeights = ComputeWeights(allGesturesToTrainNN, id);
            //vWeights = ComputeWeights(allGesturesToTrain, id);
            //vWeights = ComputeWeights3(allGesturesToTrain, id);


            // FASE 2.3:  CREAZIONE HMM
            // versione originale
            //createHMM(allGesturesToTrainQuant, id, tr._k, tr);

            createHMM(allGesturesToTrainQuant, id, tr._k,  tr.getTypeTest(), vWeights);



            //Clear vector with actions
            allGesturesToTrain.clear();
            allGesturesToTrainNN.clear();
            allGesturesToTrainQuant.clear();

        }
    }


}

//Load hmm trained
vector<Cmsdhmm_hmm> loadHMM(string path, vector<string>& vTypeHMM, TestingSet ts){

    //per ora il path serve solo per caricare i nomi
    vector<Cmsdhmm_hmm> vHMM;
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
            cout << "Carico: " << filename << endl;
            vTypeHMM.push_back(filename.substr(4, filename.size() - filename.find_first_of("_")));

           // vector<unsigned int> quantlevels={levels, levels, levels, levels};
            vector<unsigned int> quantlevels;//={levels, levels, levels, levels};
            for(auto i=0; i<1; ++i)//non importa valore, caricato da salvataggio
                quantlevels.push_back(ts._k);
            Cmsdhmm_hmm* hmm = new Cmsdhmm_hmm(8, quantlevels, true, 2);
            string prefix = "dhmmOffline/" + ts.getTypeTest() + "/" + filename;
            char * hmmName = new char [prefix.length()+1];
            strcpy_s(hmmName, prefix.length()+1, prefix.c_str());

            //Load HMM
            if(!(hmm->LoadFromFile(hmmName)))
                cout << "Error loading HMM \n" << endl;

            //Store HMM
            vHMM.push_back(*hmm);


            //Delete last HMM
            hmm->~Cmsdhmm_hmm();
        }
    }


    return vHMM;
}

//Create a name with the current data
string getNameDateQ(){

    time_t currentTime;
    struct tm *localTime;
    int day, month, year, hour, min, sec;
    time( &currentTime );                   // Get the current time
    localTime = localtime(&currentTime);  // Convert the current time to the local time

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

void GestureRecognitionDHMM::Test(const TestingSet &ts){

    vector<vector<double>> singleGestToTest;
    double maxLogLk = -DBL_MAX;
    int position = 0;
    int rightId;
    int countNullFrame = 0;
    bool actionNull = false;

    //Per le features con la media
    vector<list<double>> vMean;
    vMean.resize(36);

    //Performance
    int totTheoric = ts.size();
    int totTest = 0;
    int totRightClassified = 0;
    int totWrongClassified = 0;

    //Load all HMM trained
    vector<Cmsdhmm_hmm> vHMM;
    vector<string> vTypeHMM;
    vector<Mat> vFinal;
    vHMM = loadHMM("dhmmOffline/"+ ts.getTypeTest()+ "/", vTypeHMM, ts);
    cout << "HMMs loaded: " << vHMM.size() << endl;

    //Matrice di confusione
    Mat1b confusionMat(21, 21);
    for(auto r=0; r<confusionMat.rows;++r)
        for(auto c=0; c<confusionMat.cols; ++c)
            confusionMat(r,c) = 0;
    for(auto i=0; i<21; ++i){
        confusionMat(i, 0) = i;
        confusionMat(0, i) = i;
    }

    //For each action
    for(auto it = ts.begin(); it != ts.end(); ++it){

        rightId = (*it).GetGestureId();

        string nameF = it->GetDescription().substr(43, 26);
        //ofstream outFeat("features/"+nameF);
        //DEBUG ofstream outFeat("features/tutte.txt",std::ifstream::app);

        //For each frame
        int k=0;
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            vector<double> tmpSingleFrame;
            //tmpSingleFrame = getFeatures((*itframeset));
            k++;
            if(k > 2){
                tmpSingleFrame = getAVLFeaturesQ(*(itframeset-2), *(itframeset-1), (*itframeset)); 
                //tmpSingleFrame = getAVLMeanFeaturesQ(*(itframeset-2), *(itframeset-1), (*itframeset), vMean);

                //DEBUGfor(auto i=0;i<tmpSingleFrame.size();++i)
                //DEBUG    outFeat << tmpSingleFrame[i] << "\t";
                //DEBUGoutFeat << endl;

                //Check null features
                for(auto i=0;i<tmpSingleFrame.size();++i)
                    if(tmpSingleFrame[i] == 0)
                        countNullFrame++;
                if(countNullFrame > (tmpSingleFrame.size()/2))
                    actionNull = true;

                //Store single frame features
                singleGestToTest.push_back(tmpSingleFrame);
            }

        }

        //DEBUGoutFeat.close();

        //Classification only if the action is not null
        if(!actionNull){

            totTest++;

            //Vector of scores
            vector<double> vScores;

            //For each HMM
            for(auto i = 0; i<vHMM.size(); ++i){

                //Transition matrix
                //Mat_<double> A;
                //A = vHMM[i].m_A;

                //QUANTIZZAZIONE!! (double -> unsigned int)
                vector<vector<unsigned int>> singleGestToTestQuant;
                singleGestToTestQuant = featureQuant(singleGestToTest, ts);
                //singleGestToTestQuant = featureQuant2(singleGestToTest);

                typedef vector<vector<unsigned int>>::iterator iter_vf;
                const iter_vf init = singleGestToTestQuant.begin();
                const iter_vf final = singleGestToTestQuant.end();

                //double loglikelihood = vHMM[i].LogLikelihood(init, final, &A);
                double loglikelihood = vHMM[i].LogLikelihood(init, final, 0);

                //cout << vHMM[i].m_final << endl;
                vFinal.push_back(vHMM[i].m_final);

                //Store the score
                vScores.push_back(loglikelihood);

                //Find best
                if(loglikelihood > maxLogLk){
                    position = i;
                    maxLogLk = loglikelihood;
                }
            }

            //Check classification
            //cout << "Sequence: " << it->GetDescription() <<endl;
            cout << (it->GetDescription()).substr(43, 26)  << "\tGT: " << rightId << "\t" << "OUR: " << vTypeHMM[position] << "\t" << maxLogLk << "\t";
            char strrightId[10];
            sprintf(strrightId,"%02d",rightId);
            if(vTypeHMM[position].compare(strrightId) == 0){
                cout << "TRUE" << endl;
                totRightClassified++;
                confusionMat[rightId][stoi(vTypeHMM[position])]++;
            }
            else{
                cout << "FALSE" << endl;
                totWrongClassified++;
                confusionMat[rightId][stoi(vTypeHMM[position])]++;
            }

            //Ad ogni fine azione stampo il vettore di scores
//            ofstream outScores(_pathScoresTrain + (it->GetDescription()).substr(43, 26));
//            //cout << (it->GetDescription()).substr(43, 26) << endl;
//            outScores << it->GetDescription() << endl;
//            outScores << rightId << endl;
//            if(vTypeHMM[position].compare(strrightId) == 0)
//                outScores << "TRUE" << endl;
//            else
//                outScores << "FALSE\t" << endl; //vTypeHMM[position] << endl;
//            for(auto i=0; i<vScores.size();++i)
//                outScores << vScores[i] << endl;

//            outScores << endl << (it->end() - it->begin())-3 << endl;
//            outScores << vHMM.size() << endl;


//            for(auto i=0; i<vFinal.size() ;++i){
//                for(auto c=0; c<vFinal[i].cols ;++c){
//                    for (auto r=0; r<vFinal[i].rows; ++r)
//                        outScores << vFinal[i].at<double>(r,c) << "\t";
//                    outScores << endl;
//                }
//                outScores << endl << endl;
//            }

//            outScores.close();

            //Clear and reset all
            singleGestToTest.clear();
            maxLogLk = -DBL_MAX;
            position = 0;
            vScores.clear();
            vFinal.clear();

        }
        else{
            cout << (it->GetDescription()).substr(43, 26) << "\tNull action, not tested!" << endl;

        }

        actionNull = false;
        countNullFrame = 0;
    }

    //Print output
    string fileResult = "DTWMSR/" + getNameDateQ() + "_" + ts.parameter.substr(7);
    ofstream outResults(fileResult);

    outResults << getNameDateQ() << endl;
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
    cout << "Classification: " << ((float)totRightClassified/(float)totTest) << "%" << endl;
    outResults.close();

    //Accumulazione risultati esperimenti BATCH
    ofstream accResults("accResults.txt", std::fstream::app);
    float resClass = ((float)totRightClassified/(float)totTest);
    accResults << resClass << " " << fileResult.substr(0, fileResult.find_first_of(".")) << endl;
    accResults.close();

    //Salvo matrice confusione
    ofstream outConfusion(fileResult.substr(0, fileResult.find_first_of(".")) + "_confusion.txt");
    for(auto r=0; r<confusionMat.rows; ++r){
        for(auto c=0; c< confusionMat.cols; ++c)
            outConfusion << (int)confusionMat(r,c) << "\t";
        outConfusion << endl;
    }
    outConfusion.close();

}

