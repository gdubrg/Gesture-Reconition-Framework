#ifndef MSDHMM_DPROB_H
#define MSDHMM_DPROB_H

#include <stdlib.h>
#include <vector>

using namespace std;

#define EPSILONDPROB 0.001

// classe per singola distribuzione di probabilità - discreta - singolo stream
class Cmsdhmm_dprob : public vector<double>
{
public:
    Cmsdhmm_dprob(int iSize=1, bool bUseLog=false);

    // accesso ai dati
    bool IsNormalized(){return m_bNormalized;}
    bool IsLogDistribution(){return m_bLog;}

    void Resize(int iNewSize);

    // calcolo likelihood
    double GetLogLikelihood(int value);
    double GetLikelihood(int value);

    void Reset();
    void Normalize();
    void Accumulate(unsigned int ival, double dVal=1);

    void UseLog(bool bLog);


    // un po' di funzioni di utility, tipo  salvataggio e lettura su file
    bool SaveToFile(FILE *f);
    bool LoadFromFile(FILE *f);

protected:

    // flag interni
    bool m_bNormalized; // distribuzione normalizzata
    bool m_bLog; // true: i valori sono già in logaritmi


};


// ------------------------------------------------------
// classe per insieme di probabilità (multi stream)
class Cmsdhmm_MultiStreamProb : public vector<Cmsdhmm_dprob>
{
public:
    //costruttore di default
    Cmsdhmm_MultiStreamProb ();

    //costruttore con vettore di dimensioni
    Cmsdhmm_MultiStreamProb (const vector<unsigned int> &sizes);

    void Init(const vector<unsigned int> &sizes);

    void SetUniformWeights();

    void SetWeights(vector<double> weights){
        _weights = weights;
    }


    void Reset();
    void Normalize();
    void Accumulate( vector<unsigned int> &data, double dVal=1);


    // calcolo likelihood
    double GetLogLikelihood(const vector<unsigned int> featureVector);
    double GetLikelihood(const vector<unsigned int> featureVector);


    // un po' di funzioni di utility, tipo  salvataggio e lettura su file
    bool SaveToFile(FILE *f);
    bool LoadFromFile(FILE *f);

protected:
    vector<double> _weights;


};

#endif // MSDHMM_DPROB_H
