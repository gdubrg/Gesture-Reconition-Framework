#pragma comment(lib, "user32.lib")

#include <iostream>
#include <string>
#include <vector>
#include <QThread>
#include <QTcpSocket>

#include <QLabel>

#include "dirent.h"
#include "gesturerecognitionhmm.h"

using namespace std;
using namespace cv;
using namespace gmmstd;

GestureRecognitionHMM::GestureRecognitionHMM(){

    //Inizializzazione variabili
    contLabel = 0;
    position = 0;
    tmpMaxLk = -DBL_MIN;

    //Vettore dei joint presi dal frame
    vJoint = vector<tripletPoint>(20);

    //Carico tutti gli HMM
    outputBuffer.append( "Finding HMM...");
    cout << "Finding HMM..." << endl;
    ChargeHmm("hmm/");
    outputBuffer.append("ok! \n");
    cout << "Ok!" << endl;
    ok = false;
    //outputBuffer = "";

    //Finestra preclassify
    //preClass = Mat::zeros(70, 405, CV_8UC3);
    preClass = Mat(70, 671, CV_8UC3, Scalar(240, 240, 240));
    for(int i=0;i<10;++i)
        ColorCircleSmall(preClass, i, Scalar(0, 0, 255));

    //Finestra classificazione
    hmmState = Mat(70, 635, CV_8UC3, Scalar(240, 240, 240));
    for(int i=0;i<8;++i)
        ColorCircle(hmmState, i, Scalar(0, 0, 255));
    for(int i=0;i<7;++i)
        ColorLine(i, Scalar(0, 0, 255));

    //Finestra ok gesture
    okGesture = Mat(70, 80, CV_8UC3, Scalar(240, 240, 240));

    //Finestra istogramma
    histoLk = Mat(200, 681, CV_8UC3, Scalar(0, 0, 0));

    //Finestra sicurezza
    rob = Mat(70, 80, CV_8UC3, Scalar(255, 255, 255));

    //Finestre istogrammi HMM
    histoZoom = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoBack = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoSr= Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoSl= Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoPoint = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));

    histoRot = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoSU = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoSD = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoZoomOut = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));
    histoExit = Mat(200, 840, CV_8UC3, Scalar(0, 0, 0));

    histoImages[0]=histoBack;
    histoImages[2]=histoPoint;
    histoImages[6]=histoSl;
    histoImages[7]=histoSr;
    histoImages[8]=histoZoom;
    histoImages[9]=histoZoomOut;
    histoImages[5]=histoSU;
    histoImages[4]=histoSD;
    histoImages[1]=histoExit;
    histoImages[3]=histoRot;

}

void GestureRecognitionHMM::recognize(FrameSet &frame){

    //Per visualizzare etichetta solo un certo periodo
    contLabel++;

    cout << frame.idx << endl;

    //Se individuata una persona
    if(frame.idx >= 0){

        //Memorizzo i joint nel vettore vJoint
        //GetDataSkeleton(frame);

        //Calcolo le features e le memorizzo in vFrameFeatures
        GetFeatures(frame);

        //Accumulo le features in vFeatures
        vFeatures.push_back(vFrameFeatures);

        //Controllo che sia iniziata una gesture
        if(CheckGesture(ok) || ok){
            ok = true;

            //Segnalo attivazione classificazione
            circle(frame.RGB, Point(40, 35), 30, Scalar(0, 255, 0), -1, 8, 0);

            if(vFeatures.size()%1 == 0){

                for(int i=0;i<vHMM.size();++i){
                    //Ottengo matrice di transizione
                    Mat_<double> A;
                    A = vHMM[i].m_A;

                    //Valuto l'HMM
                    typedef vector<vector<double>>::iterator iter_vf;
                    const iter_vf ivf_init = vFeatures.begin();
                    const iter_vf ivf_final = vFeatures.end();
                    double lk = vHMM[i].LogLikelihood(ivf_init, ivf_final, &A);

                    if(lk > tmpMaxLk){
                        position = i;
                        tmpMaxLk = lk;
                    }
                }

                //cout << position << endl;
                UpdateHistoProb();
            }

            //Classifico una finestra di tot frame
            if(CheckEnd() || vFeatures.size() == 45){ // || vHMM

                cout << tmpMaxLk << endl;
                typeAction = Classify();
                if(typeAction.compare("No Action") != 0){
                    outputBuffer.append("Classification done!\n"); //<< typeAction << endl;
                    cout << "Classification done!: " << typeAction << endl;
                    sendCommand(typeAction);
                }

                contLabel = 0;

                //Dopo classificazione, elimino intera finestra
                vFeatures.erase(vFeatures.begin(), vFeatures.end());
                ok = false;
                position = 0;
                tmpMaxLk = -DBL_MIN;

            }
        }

        else{
            //Segnalo disattivazione classificazione
            circle(frame.RGB, Point(40, 35), 30, Scalar(0, 0, 255), -1, 8, 0);
        }
    }


    //Decido quando far vedere label classificazione
    if(contLabel >= 30){
        typeAction = "";
        contLabel = 0;

        //AllRed(preClass);
        for(int i=0;i<10; ++i)
            ColorCircleSmall(preClass, i, Scalar(0, 0, 255));
        for(int i=0;i<8;++i)
            ColorCircle(hmmState, i, Scalar(0, 0, 255));
        for(int i=0;i<7;++i)
            ColorLine(i, Scalar(0, 0, 255));
        circle(rob, Point(40, 35), 30, Scalar(0, 0, 0), -1, 8, 0);
        ResetHistoProb();
        histoLk = Mat(200, 681, CV_8UC3, Scalar(0, 0, 0));
        robustness = 0;
    }

    //Pulisco strutture dati utilizzate
    vFrameFeatures.clear();

    //Scrivo classificazione
    InsertText();

    // emetto il segnale ogni fine riconoscimento
    emit FrameProcessed();

}

//NOT USED
int GestureRecognitionHMM::IdSkeletonCheck(Mat1b framePlayer){
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

void GestureRecognitionHMM::ChargeHmm(string path){

    DIR* dir;
    dirent* f;

    dir = opendir(path.c_str());
    if(!dir){
        outputBuffer.append("Errore apertura cartella HMM!\n");
        cout  << "Errore apertura cartella HMM!" << endl;
        system("pause");
        EXIT_FAILURE;
    }

    while(f = readdir(dir)){
        string filename = f->d_name;
        if(filename.compare(".") != 0 && filename.compare("..") != 0){

            //Memorizzo tipo HMM
            vTypeHMM.push_back(filename);

            CHMM_GMM* hmm = new CHMM_GMM(1, 1, 1);
            string prefix = "hmm/"+filename;
            char * hmmName = new char [prefix.length()+1];
            strcpy_s(hmmName, prefix.length()+1, prefix.c_str());

            //Carico HMM e controllo esito
            if(!(hmm->LoadFromFile(hmmName)))
                outputBuffer.append("Errore caricamento HMM\n");

            //Accumulo HMM
            vHMM.push_back(*hmm);

            //Distruggo l'HMM caricato
            hmm->~CHMM_GMM();
        }
    }

}

void GestureRecognitionHMM::GetFeatures(FrameSet frame){

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
    normalize(vFrameFeatures, vFrameFeatures, 1.0, 0.0);

}

string GestureRecognitionHMM::Classify(){

    double maxLogLk = -DBL_MIN;
    double maxlogLkOther;
    //double robustness;
    double logLk;
    double sumL = 0;
    int poll = 0;
    int position = 0;
    vector<double> resultLogLk;
    string classification = "nullo";

    //Scorro e testo tutti gli HMM
    for(int i=0; i<vHMM.size(); ++i){

        //Ottengo matrice di transizione
        Mat_<double> A;
        A = vHMM[i].m_A;

        //Valuto l'HMM
        typedef vector<vector<double>>::iterator iter_vf;
        const iter_vf ivf_init = vFeatures.begin();
        const iter_vf ivf_final = vFeatures.end();
        logLk = vHMM[i].LogLikelihood(ivf_init, ivf_final, &A);

        //Memorizzo valore
        resultLogLk.push_back(logLk);

        //Mantengo solo valore massimo
        if((abs(logLk) > maxLogLk) && logLk==logLk){
            maxLogLk = logLk;
            classification = vTypeHMM[i].substr(vTypeHMM[i].find_first_of("_")+1, vTypeHMM[i].size() - vTypeHMM[i].find_first_of("_"));
            position = i;
        }

        //DEBUG Stampa distribuzione probabilit stati finale
        /*for(int k=0;k<8;++k)
            cout << vHMM[i].inst[k] << "\t";
        cout << endl;*/
        /*cout << vTypeHMM[i].substr(vTypeHMM[i].find_first_of("_")+1, vTypeHMM[i].size() - vTypeHMM[i].find_first_of("_")) << " ";
        for(int k=0;k<8;++k)
            cout << vHMM[i].m_final(vHMM[i].m_final.rows-1, k) << " ";
        cout << endl;*/

        //Controllo probabilit stato finale
        sumL = vHMM[i].m_final(vHMM[i].m_final.rows-1, vHMM[i].m_final.cols-1);// + vHMM[i].m_final(vHMM[i].m_final.rows-1, vHMM[i].m_final.cols-2);

        //Votano gli HMM che hanno finito l'azione
        if(sumL>0.98)
            poll++;

    }//fine for

    //UpdateHisto(i, logLk, Scalar(255, 255, 255));

    //Coloro di rosso valore maggiore
    //UpdateHisto(position, maxLogLk, Scalar(0, 0, 255));
    UpdateHisto(resultLogLk);

    /*for(int i=0; i<resultLogLk.size(); ++i)
            cout << resultLogLk[i] << endl;*/

    //Ordino valori
    sort(resultLogLk.begin(), resultLogLk.end());

    //Memorizzo secondo valore (per rapporto sicurezza)
    maxlogLkOther = resultLogLk[resultLogLk.size()-9];

    //Calcolo robustezza classificazione
    robustness = abs(maxLogLk/maxlogLkOther)*100;
    if(robustness > 125)
        circle(rob, Point(40, 35), 30, Scalar(0, 255, 0), -1, 8, 0);
    else if(robustness > 100)
        circle(rob, Point(40, 35), 30, Scalar(0, 255, 255), -1, 8, 0);
    else
        circle(rob, Point(40, 35), 30, Scalar(0, 0, 255), -1, 8, 0);

    //DEBUG Stampe
    //cout << "Numero voti: " << poll << endl;
    //cout << "LogLk: " << maxLogLk << "\t" << "Sicurezza: " << robustness << "\t";

    //Se pi della met hanno finito l'azione classifico
    /*if( poll<=(vHMM.size()/2)  || !CheckRobustness(robustness, classification) ){
        if(!CheckRobustness(robustness, classification) && !(poll<=(vHMM.size()/2)))
            cout << "ROBUSTEZZA NON SUFFICIENTE" << "\t";
        else
            cout << "AZIONE NON FINITA" << "\t";

        return "No Action";
    }
    else
        return classification;*/

    if( !CheckInstant(position, vHMM)){// || maxLogLk<450){//  || poll<=(vHMM.size()/2)){
        //if( (poll<=(vHMM.size()/2)-2))
           // outputBuffer.append("No Classification.\n GESTURE NOT FINISHED? \t\n\n");
        //else
            outputBuffer.append("No Classification. \n GESTURE UNKNOWN!\n");
            cout << "No Classification. \n GESTURE UNKNOWN!" << endl;
        return "No Action";
    }

    else
        return classification;
}

string GestureRecognitionHMM::PreClassify(){

    double maxLogLk = -DBL_MIN;
    double logLk;
    double sum = 0;
    int poll = 0;
    string classification = "nullo";

    //Resetto stato preclassificazione
    AllRed(preClass);

    //Scorro e testo tutti gli HMM
    for(int i=0; i<vHMM.size(); ++i){

        //Ottengo matrice di transizione
        Mat_<double> A;
        A = vHMM[i].m_A;

        //Valuto l'HMM
        typedef vector<vector<double>>::iterator iter_vf;
        const iter_vf ivf_init = vFeatures.begin();
        const iter_vf ivf_final = vFeatures.end();
        logLk = vHMM[i].LogLikelihood(ivf_init, ivf_final, &A);
        //cout << logLk << endl;
        //UpdateHisto(i, logLk, Scalar(255, 255, 255));

        //Mantengo solo valore massimo
        if((abs(logLk) > maxLogLk) && logLk==logLk){
            maxLogLk = logLk;
            classification = vTypeHMM[i].substr(vTypeHMM[i].find_first_of("_")+1, vTypeHMM[i].size() - vTypeHMM[i].find_first_of("_"));
        }

        //Analizzo probabilit finale degli stati
        sum = vHMM[i].m_final(vHMM[i].m_final.rows-1, 0);// + vHMM[i].m_final(vHMM[i].m_final.rows-1, 1);
        if(sum>0.98)
            poll++;
        else
            ColorCircleSmall(preClass, i, Scalar(0, 255, 0));

        //DEBUG Stampa distribuzione probabilit stati finale
        /*for(int k=0;k<8;++k)
            cout << vHMM[i].m_final(vHMM[i].m_final.rows-1, k) << " ";
        cout << endl;*/

    }//fine for

    //Se pi della met danno esito negativo ritorno azione nulla
    if((poll>(vHMM.size()/2)+2))
        return "No Action";
    else
        return classification;

}

void GestureRecognitionHMM::InsertText(){

    string text;
    if(typeAction.length() > 0)
        text = ConvertText(typeAction);

    int fontFace = CV_FONT_HERSHEY_SIMPLEX;
    double fontScale = 3.5;
    int thickness = 7;

    //black = Mat(250, 650, CV_8UC3, Scalar(224, 224, 224));
    black = Mat::zeros(250, 650, CV_8UC3);

    int baseline=0;
    Size textSize = getTextSize(text, fontFace,
        fontScale, thickness, &baseline);
    baseline += thickness;

    // center the text
    Point textOrg((black.cols - textSize.width)/2,
        (black.rows + textSize.height)/2);

    // then put the text itself
    putText(black, text, textOrg, fontFace, fontScale, Scalar(255, 255, 255, 0), thickness, 8, false);

}

string GestureRecognitionHMM::ConvertText(string text){

    if(text.compare("zoom") == 0)
        return "Zoom";
    else if(text.compare("sl") == 0)
        return "Slide Left";
    else if(text.compare("sr") == 0)
        return "Slide Right";
    else if(text.compare("point") == 0)
        return "Point";
    else if(text.compare("back") == 0)
        return "Go Back";
    else if(text.compare("exit") == 0)
        return "Exit";
    else if(text.compare("zoomout") == 0)
        return "Zoom Out";
    else if(text.compare("scrolldown") == 0)
        return "Scroll Down";
    else if(text.compare("scrollup") == 0)
        return "Scroll Up";
    else if(text.compare("rotate") == 0)
        return "Rotate";
    else
        return ""; //No action
}

bool GestureRecognitionHMM::CheckGesture(bool ok){

    if(!ok){
        if(vFeatures.size()%10 == 0){
            string result;
            result = PreClassify();

            //cout << "Pre-Classificazione: " << result << endl;

            if(result.compare("No Action") == 0 && !ok){
                vFeatures.erase(vFeatures.begin(), vFeatures.begin()+5);
                return false;
            }
            else
                return true;
        }
        else
            return false;
    }
    else
        return false;

}

bool GestureRecognitionHMM::CheckRobustness(double rob, string clas){

    if(clas.compare("zoom") == 0)
        if(rob > 300)
            return true;
        else
            return false;

    else if(clas.compare("sl") == 0)
        if(rob > 140)
            return true;
        else
            return false;

    else if(clas.compare("sr") == 0)
        if(rob > 102)
            return true;
        else
            return false;

    else if(clas.compare("point") == 0)
        if(rob > 106)
            return true;
        else
            return false;

    else if(clas.compare("back") == 0)
        if(rob > 110)
            return true;
        else
            return false;

    return false;
}

bool GestureRecognitionHMM::CheckInstant(int position, vector<CHMM_GMM> vHMM){

    /*if(vHMM[position].inst[7] < 40){
        cout << "Ultimo stato raggiunto troppo presto!" << "\t";
        return false;
    }*/

    /*int ok = 0;
    for(int i=0;i<8;++i)
        if( vHMM[position].inst[i] != 0)
            ok++;
    if( ok<2){
        cout << "Troppi pochi stati visitati" << "\t";
        return false;
    }*/

    //cout << "Potrebbe essere: " << vTypeHMM[position].substr(vTypeHMM[position].find_first_of("_")+1, vTypeHMM[position].size() - vTypeHMM[position].find_first_of("_")) << endl;

    //Coloro immagine stati
    for(int i=0;i<vHMM[position].m_final.cols;++i){
        if(vHMM[position].inst[i] > 0){
            ColorCircle(hmmState, i, Scalar(0, 255, 0));
            if(i!=7)
                ColorLine(i, Scalar(0, 255, 0));
        }
    }


    //Controllo che l'ultimo stato sia stato raggiunto
    bool lastState = false;
    if(vHMM[position].inst[7]>0)
        lastState = true;

    //Controllo che il terzultimo stato sia stato raggiunto
    bool penState = false;
    if(vHMM[position].inst[6]>0)
        penState = true;

    //Controllo che almeno la met degli stati sia stata visitata
    int poll = 0;
    for(int i=0;i<8;++i)
        if(vHMM[position].inst[i]>0)
            poll++;
    if(poll>4 && lastState && penState)
        return true;

    return false;
}

void GestureRecognitionHMM::AllRed(Mat img){
//    circle(img, Point(45, 35), 25, Scalar(0, 0, 255), -1, 8, 0);
//    circle(img, Point(110, 35), 25, Scalar(0, 0, 255), -1, 8, 0);
//    circle(img, Point(175, 35), 25, Scalar(0, 0, 255), -1, 8, 0);
//    circle(img, Point(240, 35), 25, Scalar(0, 0, 255), -1, 8, 0);
//    circle(img, Point(305, 35), 25, Scalar(0, 0, 255), -1, 8, 0);
    for(int i=0;i<10; ++i)
        ColorCircleSmall(preClass, i, Scalar(0, 0, 255));
}

void GestureRecognitionHMM::ColorCircle(Mat img, int position, Scalar color){
    int shift = position*75;
    circle(img, Point(55+shift, 35), 30, color, -1, 8, 0);
}

void GestureRecognitionHMM::ColorCircleSmall(Mat img, int position, Scalar color){
    int shift = position*65;
    circle(img, Point(45+shift, 35), 25, color, -1, 8, 0);
}

void GestureRecognitionHMM::ColorLine(int position, Scalar color){
    int shift = position*75;
    line(hmmState, Point(85+shift, 35), Point(100+shift, 35), color, 3, 8, 0);
}

void GestureRecognitionHMM::UpdateHisto(int position, double value, Scalar color){

    //Normalizzo
    int y = value / 25;

    //Disegno rettangolo
    int shift = position*100;

    //Elimino vecchio bin
    rectangle(histoLk, Point(shift, 200), Point(shift+60, 0), Scalar(0, 0, 0), -1, 8, 0);

    //Disegno nuovo bin
    rectangle(histoLk, Point(shift, y), Point(shift+60, 0), color, -1, 8, 0);

}

void GestureRecognitionHMM::UpdateHisto(vector<double> values){

    //Normalizzo valori
    normalize(values, values, 200);

    //Ottengo massimo valore
    vector<double>::iterator it = max_element(values.begin(), values.end());
    int max = it - values.begin();

    int offset = 70;

    for(int i=0;i<values.size();++i){
        int shift = i*65;
        //Elimino vecchio bin
        rectangle(histoLk, Point(30+shift, 200), Point(shift+offset, 0), Scalar(0, 0, 0), -1, 8, 0);
        //Disegno nuovo bin
        if(i != max)
            rectangle(histoLk, Point((30+shift), 200-values[i]), Point(shift+offset, 200), Scalar(255, 255, 255), -1, 8, 0);
        else if(i == max)
            rectangle(histoLk, Point(30+shift, 200-values[i]), Point(shift+offset, 200), Scalar(0, 0, 255), -1, 8, 0);
    }

}

void GestureRecognitionHMM::UpdateHistoProb(){

    int iIndex;
    for (iIndex=0; iIndex<NCLASSES; ++iIndex){

        for(int i=0;i<vHMM[0].m_iN;++i){
            //Ottengo valore
            double val = vHMM[iIndex].m_final(vHMM[iIndex].m_final.rows-1, i);
            val *= 150;
            if (!isnan(val))
            {
            //Elimino vecchio bin
            rectangle(histoImages[iIndex], Point(40+i*100, 200), Point(i*100+100, 0), Scalar(0, 0, 0), -1, 8, 0);
            //Disegno nuovo bin
            if(position == iIndex)
                rectangle(histoImages[iIndex], Point(40+i*100, 200-val), Point(i*100+100, 200), Scalar(0, 255, 0), -1, 8, 0);
            if(i==7)
                if((val/150)>0.90)
                    rectangle(histoImages[iIndex], Point(40+i*100, 200-val), Point(i*100+100, 200), Scalar(0, 0, 255), -1, 8, 0);
                else
                    ;
            else if(position != iIndex)
                rectangle(histoImages[iIndex], Point(40+i*100, 200-val), Point(i*100+100, 200), Scalar(255, 255, 255), -1, 8, 0);
            }
        }
    }
}

void GestureRecognitionHMM::ResetHistoProb(){
   for (int iClass=0; iClass<NCLASSES; iClass++){
    for(int i=0;i<vHMM[iClass].m_iN;++i)
        rectangle(histoImages[iClass], Point(40+i*100, 200), Point(i*100+100, 0), Scalar(0, 0, 0), -1, 8, 0);
   }
}

//NOT USED
Mat& GestureRecognitionHMM::getFrameRGB(){
    //return _frame.RGB;
    return Mat();
}

//NOT USED
Mat& GestureRecognitionHMM::getFrameDepth(){
    // return frame.depth;
    return Mat();
}

Mat& GestureRecognitionHMM::getDetection(){
    return preClass;
}

Mat& GestureRecognitionHMM::getStates(){
    return hmmState;
}

Mat& GestureRecognitionHMM::getGestureStarted(){
    return okGesture;
}

Mat& GestureRecognitionHMM::getRobustness(){
    return rob;
}

Mat& GestureRecognitionHMM::getHisto(){
    return histoLk;
}

Mat& GestureRecognitionHMM::getGesture(){
    return black;
}

QString GestureRecognitionHMM::getOutputBuffer(){
    return QString::fromStdString(outputBuffer);
}

void GestureRecognitionHMM::resetOutputBuffer(){
    outputBuffer.clear();
}

double GestureRecognitionHMM::getLevelRobustness(){
    return robustness;
}

Mat& GestureRecognitionHMM::getHistoBack(){
    return histoBack;
}

Mat& GestureRecognitionHMM::getHistoPoint(){
    return histoPoint;
}

Mat& GestureRecognitionHMM::getHistoSl(){
    return histoSl;
}

Mat& GestureRecognitionHMM::getHistoSr(){
    return histoSr;
}

Mat& GestureRecognitionHMM::getHistoZoomOut(){
    return histoZoomOut;
}

Mat& GestureRecognitionHMM::getHistoSU(){
    return histoSU;
}

Mat& GestureRecognitionHMM::getHistoSD(){
    return histoSD;
}

Mat& GestureRecognitionHMM::getHistoExit(){
    return histoExit;
}

Mat& GestureRecognitionHMM::getHistoRot(){
    return histoRot;
}

Mat& GestureRecognitionHMM::getHistoZoom(){
    return histoZoom;
}

bool GestureRecognitionHMM::CheckEnd(){

    if(vHMM[position].inst[7]>0)
        return true;
    else
        return false;
}

bool GestureRecognitionHMM::sendCommand(string command){

    CommandSender *sender = new CommandSender(command.data());
    QThread* pworkerThread = new QThread();
    sender->moveToThread(pworkerThread);
    QObject::connect(pworkerThread, SIGNAL(finished()), sender, SLOT(deleteLater()));
    pworkerThread->start();
    QObject::connect(pworkerThread, SIGNAL(started()),sender, SLOT(doWork()));
	

    return true;
}

void CommandSender::doWork() {

    QTcpSocket socket;
    QString result;

    socket.connectToHost("localhost",8000);
    cout << "Socket created..." << endl;

    if (socket.waitForConnected(-1))
    {

        cout << "Socket connected..." << endl;
        // socket collegata
        QString strValue;

        if(_command.startsWith("sl"))
            strValue = "Right";
        if(_command.startsWith("sr"))
            strValue = "Left";
        if(_command.startsWith("exit"))
            strValue = "Alt+F5"; // culo
        if(_command.startsWith("rotate"))
            strValue = "Rotate";
        if(_command.startsWith("point"))
            strValue = "Return";
        if(_command.startsWith("zoom"))
            strValue = "Ctrl+plus";
        if(_command.startsWith("zoomout"))
            strValue = "Ctrl+minus";
        if(_command.startsWith("goback"))
            strValue = "Ctrl+z";

        int nsend = socket.write(strValue.toLatin1());
        cout << "send: " << nsend << endl;

         result = "invio comando effettuato: ";
         result.append( strValue);
         cout << "Invio comando effettuato: " << strValue.toStdString() << endl;
    }
    else{
        result= "errore socket";
        cout << socket.errorString().toStdString() << endl;
    }

    socket.close();

    emit resultReady(result);
    return;

}

