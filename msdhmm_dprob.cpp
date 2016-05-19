#include "msdhmm_dprob.h"
#include <assert.h>
#include <math.h>

#include <iostream>

// costruttore
Cmsdhmm_dprob::Cmsdhmm_dprob(int iSize,bool bUseLog)
{
    m_bNormalized=false;
    m_bLog=false;
    Resize(iSize);
    UseLog(bUseLog);

}


void Cmsdhmm_dprob::Resize(int iNewSize){

    // mi salvo la vecchia modalità
    bool bUseLogOld;
    bUseLogOld = m_bLog;

    // resize e init dei valori con distribuzione uniforme
    resize(iNewSize,1.0/iNewSize);
    m_bLog=false;
    m_bNormalized=true;

    // riattivo la vecchia modalità
    UseLog(bUseLogOld);
}


double Cmsdhmm_dprob::GetLogLikelihood(int value){

    assert (value<size());



    if (m_bLog)
        return at(value);
    return log(at(value));
}

double Cmsdhmm_dprob::GetLikelihood(int value){

    assert (value<size());

    if (m_bLog){

        return exp(at(value));
    }

    return at(value);
}


void Cmsdhmm_dprob::Reset(){
    fill(begin(),end(),0);
    m_bNormalized=false;
}


void Cmsdhmm_dprob::Accumulate(unsigned int ival, double dVal){
    at(ival)+=dVal;
    m_bNormalized=false;
}


// porto gli elementi nulli ad EPSILONDPROB
// normalizzazione: termini che sommano ad 1
void Cmsdhmm_dprob::Normalize(){

    // normalizzazione: prima calcolo la somma
    double dSum=0;

    // todo: controllare validità
    if (m_bLog){
        for (auto it =begin(); it != end(); ++it){
           *it = log(exp(*it) + EPSILONDPROB);
        }
     }
    else{
        for (auto it =begin(); it != end(); ++it){
           *it += EPSILONDPROB;
        }
     }



    if (m_bLog){

        // calcolo la somma

        for (auto it =begin(); it != end(); ++it){
           dSum+=exp(*it);
        }

        assert(dSum);
        // divido ogni termine per la somma [coi log, sottraggo il log della somma]
        dSum = log(dSum);

       for (auto it =begin(); it != end(); ++it)
           *it -= dSum;
    }
    else // non log
    {

        // calcolo la somma
       for (auto it =begin(); it != end(); ++it)
           dSum += (*it);

       assert(dSum);

        // divido ogni termine per la somma
       for (auto it =begin(); it != end(); ++it)
           *it /= dSum;

    }




    // todo: controllare validità
    if (size()>1){
        // creo copia temp
        vector<double> tmp (size());
        std::copy(begin(),end(),tmp.begin());

        if (m_bLog){

            int i, dim;

            // primo elemento
            at(0)=log(exp(tmp.at(0))*0.9 + exp(tmp.at(1))*0.1); // esiste uno in quanto size > 1

            // ultimo elemento
            at(size()-1)=log(exp(tmp.at(size()-1))*0.9 + exp(tmp.at(size()-2))*0.1); // esiste in quanto size > 1

            // al centro
            for (int i=1; i<size()-2; ++i)
                at(i)=log(exp(tmp.at(i))*0.8 + exp(tmp.at(i-1))*0.1 + exp(tmp.at(i+1))*0.1);

        }
        else{
            int i, dim;

            // primo elemento
            at(0)=tmp.at(0)*0.9 + tmp.at(1)*0.1; // esiste uno in quanto size > 1

            // ultimo elemento
            at(size()-1)=tmp.at(size()-1)*0.9 + tmp.at(size()-2)*0.1; // esiste in quanto size > 1

            // al centro
            for (int i=1; i<size()-2; ++i)
                at(i)=tmp.at(i)*0.8 + tmp.at(i-1)*0.1 + tmp.at(i+1)*0.1;

         }
    }




}

// per passare da modalità log a normale e viceversa
void Cmsdhmm_dprob::UseLog(bool bLog){
    // controllo tipologia esistente
    if (m_bLog == bLog)
        return;

    unsigned int i;
    if (bLog){  // devo convertire tutto in log
        for (i=0; i<size(); i++)
           at(i) = log (at(i));
    }
    else{  // devo convertire tutto in valori normali
        for (i=0; i<size(); i++)
           at(i) = exp (at(i));
    }

    m_bLog=bLog;
}



bool Cmsdhmm_dprob::SaveToFile(FILE *f){

    // numeri vari
    unsigned int iSize = size();
    fwrite(&iSize,sizeof(unsigned int),1,f);
    unsigned  int i;
    for (i=0; i<iSize; i++)
        fwrite(&at(i),sizeof(double),1,f);

    fwrite(&m_bLog,sizeof(bool),1,f);
    fwrite(&m_bNormalized,sizeof(bool),1,f);

    return true;
}


bool Cmsdhmm_dprob::LoadFromFile(FILE *f){

    // numeri vari
    unsigned int iSize;
    fread(&iSize,sizeof(unsigned int),1,f);
    unsigned  int i;
    // resize
    Resize(iSize);

    // lettura valori
    for (i=0; i<iSize; i++)
        fread(&at(i),sizeof(double),1,f);

    fread(&m_bLog,sizeof(bool),1,f);
    fread(&m_bNormalized,sizeof(bool),1,f);

    return true;
}



// -------------------------------------------------


//costruttore con vettore di dimensioni
Cmsdhmm_MultiStreamProb::Cmsdhmm_MultiStreamProb (const vector<unsigned int> &sizes){

    // in teoria, il vettore è inizialmente vuoto

    //inizializzo
    Init(sizes);
}

// costruttore di default
Cmsdhmm_MultiStreamProb::Cmsdhmm_MultiStreamProb (){
    // non faccio niente: il vettore dovrebbe essere vuoto

}



void Cmsdhmm_MultiStreamProb::Init(const vector<unsigned int> &sizes){

    // vuoto il vettore, nel caso ci siano elementi
    clear();

    // aggiungo elementi

    // numero di distribuzioni da inserire
    int iM;
    iM = sizes.size();

    int i;
    for (i=0; i<iM; i++){
        // creo e aggiungo al vettore una distribuzione
        Cmsdhmm_dprob distrib(sizes[i]);
        push_back(distrib);
    }

    SetUniformWeights();

}



//TODO: Devono sommare ad uno!?!?!?!??
void Cmsdhmm_MultiStreamProb::SetUniformWeights(){
    // ridimensiono i pesi con la stessa dimensione del vettore e metto tutti i pesi ad 1
    _weights.resize(size(), 1.0);
}


// calcolo likelihood
double Cmsdhmm_MultiStreamProb::GetLogLikelihood(const vector<unsigned int> featureVector){

    double dRis=0;
    unsigned int i;
    for (i=0; i<size(); i++){
        dRis += at(i).GetLogLikelihood (featureVector[i]) * _weights[i];
    }

    return dRis;

}

double Cmsdhmm_MultiStreamProb::GetLikelihood(const vector<unsigned int> featureVector){
    double dRis=1;
    unsigned int i;
    for (i=0; i<size(); i++){
        dRis *= pow(at(i).GetLikelihood(featureVector[i]),_weights[i]);
       // cout << _weights[i] << endl;
    }

    return dRis;
}

void Cmsdhmm_MultiStreamProb::Reset(){
    unsigned int i;
    for (i=0; i<size(); i++){
        at(i).Reset();
    }
}

void Cmsdhmm_MultiStreamProb::Normalize(){
    unsigned int i;
    for (i=0; i<size(); i++){
        at(i).Normalize();
    }
}

void Cmsdhmm_MultiStreamProb::Accumulate(vector<unsigned int> &data, double dVal){
    unsigned int i;
    for (i=0; i<size(); i++){
        at(i).Accumulate(data[i],dVal);
    }
}

bool Cmsdhmm_MultiStreamProb::LoadFromFile(FILE *f){

    //quantlevel
    int tmp;
    fread(&tmp, sizeof(int), 1, f);

    resize(tmp);

    unsigned int i;
    for(i=0; i<tmp; ++i){
        at(i).LoadFromFile(f);
    }

    _weights.resize(tmp);

    for(i=0; i<tmp; ++i){

        fread(&_weights[i], sizeof(double), 1, f);
    }

    return true;
}

bool Cmsdhmm_MultiStreamProb::SaveToFile(FILE *f){

    //quantlevel
    int tmp = size();
    fwrite(&tmp, sizeof(int), 1, f);

    unsigned int i;
    for(i=0; i<tmp; ++i){
        at(i).SaveToFile(f);
    }


    for(i=0; i<tmp; ++i){
        fwrite(&_weights[i], sizeof(double), 1, f);
    }

    return true;
}
