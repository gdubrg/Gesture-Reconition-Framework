#ifndef COORDINATESUTILITY
#define COORDINATESUTILITY

#include <math.h>
#include <fstream>
#include <list>

#include "trainingset.h"
#include "testingset.h"

//Di seguito i metodi che elaborano le coordinate dei testing o training set

//Trasformazione coordinate in sistema di coordinate centrate su utente
template<typename _T>
void humanCenteredCoord(_T& ts){

    //For each action
    for(auto it = ts.begin(); it != ts.end(); ++it){

        //For each frame
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            //Find versors
            tripletPoint p7 = (*itframeset).bodyJoint[FS_shoulderRight];
            tripletPoint p4 = (*itframeset).bodyJoint[FS_shoulderLeft];
            tripletPoint p13 = (*itframeset).bodyJoint[FS_hipCenter];

            //N1 versor
            double normaN1 = sqrt( pow(p7.x-p4.x, 2) + pow(p7.y-p4.y, 2) + pow(p7.x-p4.x, 2));
            tripletPoint N1;
            N1.x = ( (p7.x-p4.x)/normaN1 );
            N1.y = ( (p7.y-p4.y)/normaN1 );
            N1.z = ( (p7.z-p4.z)/normaN1 );

            //Tmp versor
            double normaU = sqrt( pow(p7.x-p13.x, 2) + pow(p7.y-p13.y, 2) + pow(p7.x-p13.x, 2));
            tripletPoint U;
            U.x = ( (p7.x-p13.x)/normaU );
            U.y = ( (p7.y-p13.y)/normaU );
            U.z = ( (p7.z-p13.z)/normaU );

            //N3 versor
            tripletPoint N3;
            N3.x = N1.y*U.z - N1.z*U.y;
            N3.y = N1.z*U.x - N1.x*U.z;
            N3.z = N1.x*U.y - N1.y*U.x;
            double normaN3 = sqrt( pow(N3.x,2) + pow(N3.y,2) + pow(N3.z,2) );
            N3.x = N3.x / normaN3;
            N3.y = N3.y / normaN3;
            N3.z = N3.z / normaN3;

            //N2 versor
            tripletPoint N2;
            N2.x = N3.y*N1.z - N3.z*N1.y;
            N2.y = N3.z*N1.x - N3.x*N1.z;
            N2.z = N3.x*N1.y - N3.y*N1.x;

            //Change coordinates
            tripletPoint pCenter = (*itframeset).bodyJoint[FS_hipCenter];

            for(int i=0;i<28;++i){
                if( ((*itframeset).bodyJoint[i].x != FLT_MAX) && (i != FS_hipCenter) ){

                    double R = sqrt( pow((*itframeset).bodyJoint[i].x-pCenter.x, 2) + pow((*itframeset).bodyJoint[i].y-pCenter.y, 2) + pow((*itframeset).bodyJoint[i].x-pCenter.x, 2));

                    tripletPoint p;
                    p.x = (*itframeset).bodyJoint[i].x - pCenter.x;
                    p.y = (*itframeset).bodyJoint[i].y - pCenter.y;
                    p.z = (*itframeset).bodyJoint[i].z - pCenter.z;

                    //New Coordinates
                    (*itframeset).bodyJoint[i].x = p.x*N1.x + p.y*N1.y + p.z*N1.z;
                    (*itframeset).bodyJoint[i].y = p.x*N2.x + p.y*N2.y + p.z*N2.z;
                    (*itframeset).bodyJoint[i].z = p.x*N3.x + p.y*N3.y + p.z*N3.z;

                }
            }
        }
    }
}

//value to quantize, k parts, v value single part
//int quant(float curVal, int k, float v){

//    int q;

//    for(auto i=-(k/2); i<(k/2); ++i){
//        float down = v*i;
//        float top = v*(i+1);
//        if( (curVal >= down) && (curVal < top) ){
//            q = i;
//            return q;
//        }
//    }
//}

//template<typename _T>
//std::vector<std::vector<std::vector<int>>> quantizationCoord(_T& ts, int k){

//    std::vector<int> quantSingleFrame;
//    std::vector<std::vector<int>> quantSingleAction;
//    std::vector<std::vector<std::vector<int>>> quantActions;

//    //For each action
//    for(auto it = ts.begin(); it != ts.end(); ++it){

//        //For each frame
//        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

//            //Lunghezza da quantizzare
//            float l = 1*2;

//            //Valore singola parte
//            float v = l / (float)k;

//            //Valore quanto
//            int q;

//            quantSingleFrame.push_back(quant((*itframeset).bodyJoint[FS_handRight].x, k, v));

//            quantSingleFrame.push_back(quant((*itframeset).bodyJoint[FS_handRight].y, k, v));

//            quantSingleFrame.push_back(quant((*itframeset).bodyJoint[FS_handLeft].x, k, v));

//            quantSingleFrame.push_back(quant((*itframeset).bodyJoint[FS_handLeft].y, k, v));

//            //memorizzo frame
//            quantSingleAction.push_back(quantSingleFrame);
//            quantSingleFrame.clear();

//        }//end frame

//        //debug
//        ofstream out("quant.txt");
//        for(auto i=0;i<quantSingleAction.size();++i)
//            out << quantSingleAction[i][0] << "\n";
//        out.close();

//        //memorizzo azione
//        quantActions.push_back(quantSingleAction);
//        quantSingleAction.clear();



//    }//end action

//    return quantActions;
//}

//Spherical Coordinate transformation
template<typename _T>
void sphericalCoord(_T& ts){

    //For each action
    for(auto it = ts.begin(); it != ts.end(); ++it){

        //For each frame
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            //For each joint
            for(auto i=0;i<28;++i)

                //Only if it is a valid joint
                if((*itframeset).bodyJoint[i].x != FLT_MAX){

                    float R = sqrt(pow((*itframeset).bodyJoint[i].x, 2) + pow((*itframeset).bodyJoint[i].y, 2) + pow((*itframeset).bodyJoint[i].z, 2));
                    float THETA = acos(((*itframeset).bodyJoint[i].z)/R);
                    float PHI = atan2( (*itframeset).bodyJoint[i].y, (*itframeset).bodyJoint[i].x );

                    (*itframeset).bodyJoint[i].x = R;
                    (*itframeset).bodyJoint[i].y = THETA;
                    (*itframeset).bodyJoint[i].z = PHI;

                }

        }
    }
}

//Centre and normalize the data
template<typename _T>
void centreAndNormalize(_T& ts){

    //For each action
    for(auto it = ts.begin(); it != ts.end(); ++it){

        //For each frame
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

            double centerX = ((*itframeset).bodyJoint[FS_shoulderRight].x + (*itframeset).bodyJoint[FS_shoulderLeft].x)/2;
            double centerY = ((*itframeset).bodyJoint[FS_shoulderRight].y + (*itframeset).bodyJoint[FS_shoulderLeft].y)/2;

            //Centre the data
            for(auto i=0; i<28; ++i)
                if((*itframeset).bodyJoint[i].x != FLT_MAX){
                    (*itframeset).bodyJoint[i].x -= centerX;
                    (*itframeset).bodyJoint[i].y -= centerY;
                }

            //Normalization
            double shouldDist = sqrt(pow((*itframeset).bodyJoint[FS_shoulderRight].x - (*itframeset).bodyJoint[FS_shoulderLeft].x, 2)+pow((*itframeset).bodyJoint[FS_shoulderRight].y - (*itframeset).bodyJoint[FS_shoulderLeft].y, 2));

            for(auto i=0; i<28; ++i)
                if((*itframeset).bodyJoint[i].x != FLT_MAX){
                    (*itframeset).bodyJoint[i].x /= shouldDist;
                    (*itframeset).bodyJoint[i].y /= shouldDist;
                }
        }
    }
}

#endif // COORDINATESUTILITY

