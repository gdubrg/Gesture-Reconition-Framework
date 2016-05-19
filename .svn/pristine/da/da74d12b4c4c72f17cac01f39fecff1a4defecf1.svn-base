#ifndef MSDHMM_HMM_H
#define MSDHMM_HMM_H

#include "msdhmm_dprob.h"
#include <opencv2/opencv.hpp>
#include <QDebug>

using namespace cv;


#define DELTA 0.001

template <class ForwardIterator>
int SequenceLength (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd){
    int T=0;
    for (ForwardIterator it = ObservationsBegin; it!=ObservationsEnd; ++it) T++;
    return T;
}


class Cmsdhmm_hmm
{
public:


    // costruttore
    // N: numero di stati
    // vettore di quantizzazione

    Cmsdhmm_hmm (unsigned int N, vector<unsigned int> quantlevels, bool bLeftRight=true, int iDeltaSteps=1);

    // operazioni
public:




    void ResetB();
    // TODO: mettere un epsilon per "sporcare" le distribuzioni

    // init completamente random senza vettore di osservazioni
    bool Init_Random();

    // init random con statistiche prese dalle osservazioni
    template <class ForwardIterator>
    bool Init_Random (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

    // init deterministico, prendendo le osservazioni e dividendo in fettine
    template <class ForwardIterator>
    bool Init_Deterministic (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);


    bool SetLeftRight (bool bSet=true, int iDeltaSteps=1){
        m_bLeftRight= bSet;
        m_iDeltaSteps=iDeltaSteps;
        return bSet;
    }



    // Log Likelihood calcolata con la forward (senza parametri) - iteratori std
    template <class ForwardIterator>
    double LogLikelihood (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd, Mat_<double>* alphaT=0);

    // Log Likelihood calcolata con la forward (senza parametri) - iteratori std
    template <class ForwardIterator>
    double Test (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd, Mat_<double>* alphaT=0);


    // forward
    template <class ForwardIterator>
    double Forward(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

    template <class ForwardIterator1, class ForwardIterator2>
    double ForwardWithScale(ForwardIterator1 ObservationsBegin, ForwardIterator1 ObservationsEnd, Mat_<double> &alpha, ForwardIterator2 ScaleBegin);

    // backward
    template <class ForwardIterator>
    double Backward(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    double BackwardWithScale(BidirectionalIterator1 ObservationsBegin, BidirectionalIterator1 ObservationsEnd, Mat_<double> &beta,  BidirectionalIterator2 ScaleBegin);

    template <class BidirectionalIterator>
    void BaumWelch (BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation,
        Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma, int *pniter, double *plogprobinit, double *plogprobfinal);

    template <class BidirectionalIterator>
        void BaumWelch(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int *pniter, double *plogprobinit, double *plogprobfinal);


   template <class BidirectionalIterator>
        vector<unsigned int> Viterbi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation);


    // -------------------------
    // TODO
    /*
    template <class BidirectionalIterator>
    bool Init_Random_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence);

    */

    template <class BidirectionalIterator>
    void BaumWelch_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence, int *pniter, double *plogprobinit, double *plogprobfinal);



    // Funzioni di utilitā

    void ComputeGamma(Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma);

    template <class BidirectionalIterator>
        void ComputeXi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &xi);

    /*
    template <class BidirectionalIterator>
        void ComputeXi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &xi);

    template <class  BidirectionalIterator>
        void UpdateSigmaNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_sigmail, double &dDen_sigmail, int e, int i, int r, int s, int k);

        template <class  BidirectionalIterator>
    void  UpdateMuilNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_muil, double &dDen_muil, int e, int r, int k);

    template <class  BidirectionalIterator>
    void UpdateGammail(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int e, int i);
    */


    // leggo e scrivo su file
    bool LoadFromFile (FILE *f);
    bool SaveToFile (FILE *f);

    bool LoadFromFile (const char* strFileName);
    bool SaveToFile (const char* strFileName);

    void PrintA(){
        int i,j;
        for (i=0; i<m_iN;i++)
            for (j=0; j<m_iN;j++)
                qDebug() << i << "->" << j << m_A(i,j) << "\n";

    }

    // gestione dei pesi
    void SetWeights(vector<double> dweigths){
            _dweigths = dweigths;

            // set pesi di tutte le componenti
            for (auto &it :m_B)
                it.SetWeights(_dweigths);

    }


    // per ora lascio tutto public
public:

    unsigned int m_iN;		/* number of states;  Q={1,2,...,N} */
    vector<unsigned int> m_quantlevels; // livelli di quantizzazione (indica anche la dimensione del feature vector)


    Mat_<double> m_A;	/* A[1..N][1..N]. a[i][j] is the transition prob of going from state i at time t to state j at time t+1 */
    vector<Cmsdhmm_MultiStreamProb> m_B;	/* m_B(j) e' la distribuzione che definisce lo stato j */

    Mat_<double> m_pi;	/* pi[1..N] pi[i] is the initial state distribution. */
    Mat_<double> m_final;	/* final[1..N] final[i] is the final state distribution. */

    bool m_bLeftRight;
    int m_iDeltaSteps;

protected:

    vector<Mat_<double>*> Vp_xi;
    vector<Mat_<double>*> Vp_gammail;
    vector<Mat_<double>*> Vp_gamma;

    vector<double> _dweigths;
};





















// ------------------------------------
// DEFINIZIONE FUNZIONI TEMPLATE
// ------------------------------------


// init deterministico, prendendo le osservazioni e dividendo in fettine
template <class ForwardIterator>
bool Cmsdhmm_hmm::Init_Deterministic (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd){

    //Probabilita iniziali
    int i,j;

    int iSizeF;
    iSizeF=ObservationsBegin->size();

    vector<double> weigths;
    weigths.resize(iSizeF,1);
    SetWeights(weigths);

    // calcolo lunghezza sequenza
    unsigned int T= ObservationsEnd - ObservationsBegin;
    double dSizechunk= ((double) T) / m_iN;

    if(m_bLeftRight){

        //Probabilitā iniziale degli stati (1, 0, ..., 0)
        fill(m_pi.begin(), m_pi.end(), 0);
        m_pi(0, 0) = 1.0;

        //Matrice A
        fill(m_A.begin(),m_A.end(),0.);

        for (i=0; i<m_iN-1; i++){

            m_A(i,i)= (dSizechunk -1) / dSizechunk;
            //qDebug() << "i: " << i << endl;


            for (j=1; j<=m_iDeltaSteps && (i+j)<m_A.rows; j++){
                //qDebug() << "j+1: " << j+i << endl;
                m_A(i,i+j)= (1.0 / dSizechunk )  /  m_iDeltaSteps;
            }

        }

        // last state
        m_A(m_iN-1,m_iN-1)=1;

    }

    else{

        //Probabilita' iniziale degli stati (1, 0, ..., 0)
        fill(m_pi.begin(), m_pi.end(), 1.0 / m_iN);

        //Matrice A
        fill(m_A.begin(),m_A.end(),1.0 / m_iN);

    }



    //Matrice B

    ForwardIterator it;
    int count = 0;

    // azzero gli istogrammi
    int iState;
    for (iState =0; iState<m_iN; iState++)
        m_B[iState].Reset();


    //Per ogni frame
    for(it=ObservationsBegin; it!=ObservationsEnd; ++it){

        // recupero numero dello stato a cui assegnare la feature
        iState =  count / (T / m_iN);
        //qDebug() << count << " " << iState << endl;


        //accumulo
        if(iState > (m_iN-1))
            iState = m_iN -1;
        //cout << count << " " << iState << endl;
        m_B[iState].Accumulate(*it);

        count++;
    }

    // normalizzo
    for (iState =0; iState<m_iN; iState++)
        m_B[iState].Normalize();


    return true;
}








    template <class ForwardIterator>
    double  Cmsdhmm_hmm::LogLikelihood (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd,Mat_<double> *alphaT){

        int T;
        T= ObservationsEnd - ObservationsBegin;

        Mat_<double> alpha(T,m_iN);

        vector<double> scale (T,1);
        double logprobf;
        logprobf = ForwardWithScale(ObservationsBegin, ObservationsEnd,alpha, scale.begin());
        if(alphaT)
        {
            if(alphaT->rows==1 && alphaT->cols==m_iN)
                alpha.row(T-1).copyTo(*alphaT);
            else
                *alphaT=alpha.row(T-1).clone();
        }


        return logprobf;
    }



// ------------------------------------
// ------------------------------------
// ------------------------------------
// forward



//void Forward(HMM *phmm, int T, int *O, double **alpha, double *pprob)
template <typename ForwardIterator>
double Cmsdhmm_hmm::Forward(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd)
{

    ForwardIterator it;
    // conto il numero di osservazioni presenti
    unsigned int T=0;
    for (it = ObservationsBegin; it != ObservationsEnd; ++it) T++;

    if (!T)
        return 0;  // errore


        unsigned int     i, j;   /* state indices */
        unsigned int     t;      /* time index */


        double sum;     /* partial sum */
        double dBiO,dBjO;

        // creo la matrice degli alfa
        Mat_<double> alpha(T,m_iN);

        /* 1. Initialization */

        it = ObservationsBegin;
        for (i = 0; i < m_iN; i++){
            //alpha(0,i) = m_pi[i]* m_B(i,O[0]);
            dBiO = m_B[i].GetLikelihood(*it);
            alpha(0,i) = m_pi(i,0)* dBiO;
        }

        /* 2. Induction */

        t=1;
        ++it;
        for (; it != ObservationsEnd; ++it){
                for (j = 0; j < m_iN; j++) {
                        sum = 0.0;
                        for (i = 0; i < m_iN; i++)
                                sum += alpha(t-1,i) * (m_A(i,j));
                        //alpha(t,j) = sum*(m_B(j,O[t]));
                        dBjO = m_B[j].GetLikelihood(*it,false);
                        alpha(t,j) = sum * dBjO;

                }
                t++;
        }


        /* 3. Termination */
        double prob = 0.0;
        for (i = 0; i < m_iN; i++)
                prob += alpha(T-1,i);

        return  prob;

}


// void ForwardWithScale(HMM *phmm, int T, int *O, double **alpha, double *scale, double *pprob)
template <class ForwardIterator1, class ForwardIterator2>
double Cmsdhmm_hmm::ForwardWithScale(ForwardIterator1 ObservationsBegin, ForwardIterator1 ObservationsEnd, Mat_<double> &alpha, ForwardIterator2 ScaleBegin)
/*  pprob is the LOG probability */
{
    unsigned int	i, j; 	/* state indices */
    unsigned int	t;	/* time index */

    ForwardIterator1 it;
    // conto il numero di osservazioni presenti
    unsigned int T=0;
    T= SequenceLength(ObservationsBegin,ObservationsEnd);

    //	for (it = ObservationsBegin; it != ObservationsEnd; ++it) T++;

    if (!T)
        return 0;  // errore


    double sum;	/* partial sum */
    double dBiO,dBjO;
    double dprob = 0.0;
    double dScale;

    /* 1. Initialization */
    it = ObservationsBegin;
    t=0;
    dScale = 0;
    for (i = 0; i < m_iN; i++) {
        //alpha(0,i) = m_pi[i]* (m_B(i,O[0]));
        dBiO = m_B[i].GetLikelihood(*it);
        alpha(0,i) = m_pi(i,0)* dBiO;
        dScale  += alpha(0,i);
    }

    assert (dScale);

    //normalizzazione

    for (i = 0; i < m_iN; i++)
        alpha(0,i) /= dScale;

    // aggiungo la scala all'iteratore corrente
    *ScaleBegin = dScale;
    ++ScaleBegin;

    dprob += log(dScale);

    /* 2. Induction */
    ++t;
    ++it;

    for (; it != ObservationsEnd; ++it, t++){

        dScale = 0.0;
        for (j = 0; j < m_iN; j++) {
            sum = 0.0;
            for (i = 0; i < m_iN; i++) // per ogni stato di partenza
                sum += alpha(t-1,i)* (m_A(i,j));

            //alpha(t+1,j) = sum*(m_B(j,O[t+1]));
            dBjO = m_B[j].GetLikelihood(*it);
            alpha(t,j) = sum * dBjO;


            dScale += alpha(t,j);

        }

        assert (dScale);



        for (j = 0; j < m_iN; j++) {
            alpha(t,j) /= dScale;
        }


        m_final = alpha.clone();

        /*if(T-1==t)
            cout << alpha(t,j) << " " << dScale << endl;*/

        *ScaleBegin = dScale;
        ++ScaleBegin;

        dprob += log(dScale);
    }

    /* 3. Termination */
    assert (t==T); // devo essere arrivato in fondo

    // la somma dei log delle scale č il valore da restituire
    return dprob;

}


// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------
// Backward



//void Backward(HMM *phmm, int T, int *O, double **beta, double *pprob)
//double CHMM_GMM::Backward(const CArray<CArray<double>*> &O, CArray2D<double> &beta)
template <typename BackwardIterator>
double Cmsdhmm_hmm::Backward(BackwardIterator ObservationsBegin, BackwardIterator ObservationsEnd)
{
        unsigned int     i, j;   /* state indices */
         int     t;      /* time index */
        double sum;
        double dBjO;

    BackwardIterator it;
    unsigned int T;
    T = ObservationsEnd - ObservationsBegin;


        // creo la matrice dei beta
        Mat_<double> beta(T,m_iN);

        /* 1. Initialization */

        for (i = 0; i < m_iN; i++)
                beta(T-1,i) = 1.0;

        /* 2. Induction */
       t = T-2;
       for (it = ObservationsEnd; it != ObservationsBegin; --it){
                for (i = 0; i < m_iN; i++) {
                        sum = 0.0;
                        for (j = 0; j < m_iN; j++){
                                //sum += m_A(i,j) * m_B(j,O[t+1]) *beta(t+1,j);
                                dBjO = m_B[j].GetLikelihood(*it,false);
                                sum += m_A(i,j) * dBjO *beta(t+1,j);
                        }
                        beta(t,i) = sum;

                }
        }

        /* 3. Termination */
        double dProb = 0.0;
        for (i = 0; i < m_iN; i++)
                dProb+= beta(0,i);

        return dProb;

}


//void BackwardWithScale(HMM *phmm, int T, int *O, double **beta, 	double *scale, double *pprob)
//double  CHMM_GMM::BackwardWithScale(const CArray<CArray<double>*> &O, CArray2D<double> &beta,CArray<double> &scale)
template <class BidirectionalIterator1, class BidirectionalIterator2>
    double Cmsdhmm_hmm::BackwardWithScale(BidirectionalIterator1 ObservationsBegin, BidirectionalIterator1 ObservationsEnd, Mat_<double> &beta, BidirectionalIterator2 ScaleBegin)
{
    unsigned int     i, j;   /* state indices */
    int     t;      /* time index */
    double sum;
    double dScale;
    double dBjO;
    double dProb;


    unsigned int T;
    T = SequenceLength(ObservationsBegin,ObservationsEnd);

    BidirectionalIterator2 scale = ScaleBegin+T-1;
    BidirectionalIterator1 it;


    /* 1. Initialization */
    dScale = *scale;
    dProb = log(dScale);
    t = T-1;
    for (i = 0; i < m_iN; i++)
       beta(t,i) = 1.0/dScale;

        /* 2. Induction */


    // parto dall'ultimo valore valido
  it = ObservationsEnd;


  for (t=T-2; t>=0; --t ){

       --it;
        --scale;

        dScale = *scale;
        dProb += log(dScale);
         for (i = 0; i < m_iN; i++) {
            sum = 0.0;
            for (j = 0; j < m_iN; j++){
                dBjO = m_B[j].GetLikelihood(*it);
                //sum += m_A(i,j) * (m_B(j,O[t+1]))*beta(t+1,j);
                sum += m_A(i,j) * dBjO *beta(t+1,j);
            }
            assert (dScale);
            beta(t,i) = sum/dScale;
         }

   }


    /* 3. Termination */

    return dProb;




}



    // -----------------------------------------
    // -----------------------------------------
    // -----------------------------------------
    // BAUM


template <class BidirectionalIterator>
void Cmsdhmm_hmm::BaumWelch (BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation,
        Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma, int *pniter, double *plogprobinit, double *plogprobfinal)
{
    unsigned int	i, j, k;
    unsigned int	t, l = 0;

    unsigned int T;
    T= SequenceLength(FirstObservation,LastObservation);

    // todo: controllo dimensioni matrici

    double dOld, dNew;

    double	logprobf, logprobb;
    double	numeratorA, denominatorA;
    double dSum;
    unsigned int r,s;


    const int Size[] = {T,m_iN,m_iN};

    Mat_<double> xi (3,Size);
    //Mat_<double> gammail (T, m_iK);
    vector<double> scale (T,1);

    double delta, deltaprev, logprobprev;
    deltaprev = 10e-70;

    logprobf = ForwardWithScale ( FirstObservation,LastObservation, alpha, scale.begin());
    *plogprobinit = logprobf; /* log P(O |intial model) */

    logprobb = BackwardWithScale( FirstObservation,LastObservation, beta, scale.begin());


    ComputeGamma(alpha, beta, gamma);
    ComputeXi( FirstObservation, LastObservation, alpha, beta, xi);

    logprobprev = logprobf;

    do  {

        /* reestimate frequency of state i in time t=1 */

        dSum=0;
        for (i = 0; i < m_iN; i++) {
            m_pi(i,0) = gamma(0,i);
        }

        ResetB();


        /* reestimate transition matrix  and symbol prob in
           each state */
        for (i = 0; i < m_iN; i++) {    // PER OGNI STATO i-ESIMO

            denominatorA = 0.0;
            for (t = 0; t < T - 1; t++)
                denominatorA += gamma(t,i);

            dSum = 0;
            for (j = 0; j < m_iN; j++) {
                numeratorA = 0.0;
                for (t = 0; t < T - 1; t++)
                    numeratorA += xi(t,i,j);

                if (denominatorA)
                    m_A(i,j) = numeratorA/denominatorA;
                else
                    m_A(i,j) = 0;

                dSum +=m_A(i,j);
            }


            // rinormalizzo A affinchč sia stocastica, se qualcosa č andato storto
            //assert(fabs(dSum-1)<.001);
            /*
            if (fabs(dSum-1)<.001)
                for (j = 0; j < m_iN; j++) {
                    m_A(i,j) /= dSum;
                }
            else
                for (j = 0; j < m_iN; j++) {
                    m_A(i,j) = 1.0 / m_iN;
                }
           */


            // ricalcolo i B
            t=0;
            for ( BidirectionalIterator it = FirstObservation; it != LastObservation; ++it, ++t) {

                m_B[i].Accumulate(*it,gamma(t,i));
            }

            m_B[i].Normalize();

        }



        logprobf= ForwardWithScale( FirstObservation, LastObservation, alpha, scale.begin());
        logprobb= BackwardWithScale(FirstObservation, LastObservation, beta, scale.begin());
        ComputeGamma(alpha, beta, gamma);
        ComputeXi(FirstObservation, LastObservation, alpha, beta, xi);


        // provo a fare un decode della sequenza
        /*
        qDebug() << "\n\nIterazione: " << l << "\n";
        for (t=0; t<T; t++ ){
                double dMaxVal=0;
                int iMaxState=-1;
                for (i = 0; i < m_iN; i++) {
                    double dval;
                    dval = alpha(t,i)*beta(t,i);
                    if (iMaxState<0 || dMaxVal < dval)
                    {
                        iMaxState=i; dMaxVal=dval;
                    }

                }
                qDebug()<< "t: " << t << " - State: " << iMaxState << endl;
           }
            */



        /* compute difference between log probability of
           two iterations */
        delta = fabs( logprobf - logprobprev);
        logprobprev = logprobf;
        l++;

    }
    while (delta > DELTA); /* if log probability does not
                                  change much, exit */

    *pniter = l;
    *plogprobfinal = logprobf; /* log P(O|estimated model) */

}



//void CHMM_GMM::BaumWelch(const CArray<CArray<double>*> &O, int *pniter, double *plogprobinit, double *plogprobfinal)
template <class BidirectionalIterator>
    void Cmsdhmm_hmm::BaumWelch(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int *pniter, double *plogprobinit, double *plogprobfinal)

{

    int T;
    T = SequenceLength(FirstObservation, LastObservation);

    // alpha, beta, gamma
    Mat_<double> alpha (T,m_iN);
    Mat_<double> beta (T,m_iN);
    Mat_<double> gamma (T,m_iN);

    BaumWelch(FirstObservation, LastObservation, alpha,beta,gamma,pniter,plogprobinit,plogprobfinal);
}


//void ComputeXi(HMM* phmm, int T, int *O, double **alpha, double **beta, 	double ***xi)
//void CHMM_GMM::ComputeXi(const CArray<CArray<double>*> &O, CArray2D<double> &alpha, CArray2D<double> &beta, CArray3D<double> &xi)
template <class BidirectionalIterator>
void Cmsdhmm_hmm::ComputeXi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &xi)
{
    unsigned int i, j;
    unsigned  int t;
    double sum;
    double dBjO;

    unsigned int T;
        T= SequenceLength(FirstObservation,LastObservation);

    BidirectionalIterator it = FirstObservation;
    ++it; // salto il primo
    t=0;
    for (; it != LastObservation; ++it, ++t) {
    //for (t = 0; t < T - 1; t++) {
        sum = 0.0;
        for (i = 0; i < m_iN; i++)
            for (j = 0; j < m_iN; j++) {
                // ERRORE!!! B[j]!!! dBjO = m_B[i].GetLikelihood(*it);
                dBjO = m_B[j].GetLikelihood(*it);
                xi(t,i,j) = alpha(t,i)*beta(t+1,j)
                    *(m_A(i,j))
                    *dBjO;

                sum += xi(t,i,j);
            }

        assert (sum);

        for (i = 0; i < m_iN; i++)
            for (j = 0; j < m_iN; j++)
                xi(t,i,j)  /= sum;
    }
}


template <class BidirectionalIterator>
     vector<unsigned int> Cmsdhmm_hmm::Viterbi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation){


         BidirectionalIterator it;
         int T;
         T = SequenceLength(FirstObservation, LastObservation);

         // alpha, beta, gamma
         Mat_<double> delta (T,m_iN);
         Mat_<unsigned int> psi (T,m_iN);

         vector<unsigned int> ris;
         ris.resize(T,-1);

         double dBiO,dBjO;
         int t;
         int i,j;


         /* 1. Initialization */
         it = FirstObservation;
         t=0;
         for (i = 0; i < m_iN; i++)
         {

                 dBiO = m_B[i].GetLikelihood(*it);
                 delta (t,i) = m_pi(i,0)* dBiO;
                 psi (t,i) = 0;

         }

         /* 2. Recursion */

         ++t;
         ++it;
         double dmaxval, dval;
         int imaxstate;

         for (; it != LastObservation; ++it, t++){


             for (j = 0; j < m_iN; j++) {
                 dmaxval = 0;
                 imaxstate=-1;
                 dBjO = m_B[j].GetLikelihood(*it);

                 for (i = 0; i < m_iN; i++){ // per ogni stato di partenza
                     dval = delta(t-1,i)*m_A(i,j);
                     if (dval > dmaxval){
                         dmaxval = dval;
                         imaxstate = i;
                     }
                 }

                 delta(t,j) = dmaxval * dBjO;
                 psi(t,j) = imaxstate;
             }
         }

         /* 3. Termination */

         // in imaxstate c'è ancora l'ultimo stato

         ris[T-1] = imaxstate;
         // posso decodificare la sequenza di stati mediante psi
         for (t=T-2; t>0; t--)
         {
             ris[t] = psi(t+1,ris[t+1]);
             qDebug() << "istante " << t << " stato: " << ris[t] << "\n";
         }

         // restituisco la sequenza
         return ris;
     }


template <class BidirectionalIterator>
     void Cmsdhmm_hmm::BaumWelch_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence, int *pniter, double *plogprobinit, double *plogprobfinal){

         unsigned int	e,i, j, k;
         unsigned int	t, l = 0;

         unsigned int E;
         E = SequenceLength(FirstSequence,LastSequence);

         unsigned int T;

         double dOld, dNew;

         double	logprobf, logprobb;
         double	numeratorA, denominatorA;

         Vp_xi.clear();
         Vp_gammail.clear();
         Vp_gamma.clear();

         //Aree dati
         e = 0;
         for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e)
         {

             T = SequenceLength((*itSeq).begin(),(*itSeq).end());

             int iSize[] = {T,m_iN,m_iN};
             Mat_<double>* p_xi  = new Mat_<double> (3,iSize);
             Vp_xi.push_back(p_xi);

             Mat_<double>* p_gamma  = new Mat_<double> (T, m_iN);
             Vp_gamma.push_back(p_gamma);

         }

         double delta, deltaprev, logprobprev;
         deltaprev = 10e-70;





         // INIT - calcolo valori iniziali
         *plogprobinit =0;
         e=0;

         for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e){

             //CArray<CArray<double>*> & O = *SetofO[e];
             T= SequenceLength((*itSeq).begin(),(*itSeq).end());

             // alpha, beta e scale non devo conservarli e sono dipendenti da T.
             Mat_<double> alpha(T,m_iN);
             Mat_<double> beta(T,m_iN);
             vector<double> scale (T);

             // recupero gamma e xi correnti
             Mat_<double> & gamma = *Vp_gamma[e];
             Mat_<double> & xi= *Vp_xi [e];

             logprobf = ForwardWithScale( (*itSeq).begin(),(*itSeq).end(), alpha, scale.begin());
             logprobb = BackwardWithScale( (*itSeq).begin(),(*itSeq).end(), beta, scale.begin());
             ComputeGamma(alpha, beta, gamma);
             ComputeXi((*itSeq).begin(),(*itSeq).end(), alpha, beta, xi);
             (*plogprobinit)+=logprobf;


         } //fine iterazioni su sequenze

         logprobprev = *plogprobinit;



         //ITERAZIONI FINO A CONVERGENZA
         do  {

             // reestimate frequency of state i in time t=1
             for (i = 0; i < m_iN; i++) {
                 double dVal;
                 dVal = 0;
                 for (e = 0; e < E; e++){
                     Mat_<double> & gamma = *Vp_gamma[e];
                     dVal += gamma(0,i);
                 }
                 dVal /= E;
                 m_pi(i,0) = dVal;
             }

             ResetB();

             for (i = 0; i < m_iN; i++) {


                 // CALCOLO IL DENOMINATORE, CHE č COMUNE A TUTTI
                 denominatorA = 0.0;
                 for (e = 0; e < E; e++){
                     Mat_<double> & gamma = *Vp_gamma[e];
                     T= gamma.rows;
                     for (t = 0; t < T - 1; t++)
                         denominatorA += gamma(t,i);
                 }

                 //aggiorno Aij
                 double dSummmm=0;
                 for (j = 0; j < m_iN; j++) {
                     numeratorA = 0.0;
                     for (e = 0; e < E; e++){
                         Mat_<double> & xi = *Vp_xi[e];
                         T= xi.size[0];
                         for (t = 0; t < T - 1; t++)
                             numeratorA += xi(t,i,j);
                     }

                     // BUG: ora dovrebbero sommare ad 1
                     //m_A(i,j) = (DELTA / m_iN) + (1.0-DELTA) *numeratorA/denominatorA;
                     if (denominatorA)
                         m_A(i,j) = numeratorA/denominatorA;
                     else
                         m_A(i,j) =0;

                     dSummmm+=m_A(i,j);
                 }


                 e=0;
                 for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e){
                     Mat_<double> & gamma = *Vp_gamma[e];
                     t=0;
                     for ( auto it =  (*itSeq).begin(); it != (*itSeq).end(); ++it, ++t) {
                          m_B[i].Accumulate(*it,gamma(t,i));
                     }
                 }

                 m_B[i].Normalize();
            }

             // devo rifare i calcoli
             double logprobfinale;
             logprobfinale = 0;
             e=0;

             for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e){

                 //CArray<CArray<double>*> & O = *SetofO[e];
                 T= SequenceLength((*itSeq).begin(),(*itSeq).end());

                 // alpha, beta e scale non devo conservarli e sono dipendenti da T.
                 Mat_<double> alpha(T,m_iN);
                 Mat_<double> beta(T,m_iN);
                 vector<double> scale (T);

                 // recupero gamma e xi correnti
                 Mat_<double> & gamma = *Vp_gamma[e];
                 Mat_<double> & xi= *Vp_xi [e];

                 logprobf = ForwardWithScale( (*itSeq).begin(),(*itSeq).end(), alpha, scale.begin());
                 logprobb = BackwardWithScale( (*itSeq).begin(),(*itSeq).end(), beta, scale.begin());
                 ComputeGamma(alpha, beta, gamma);
                 ComputeXi((*itSeq).begin(),(*itSeq).end(), alpha, beta, xi);
                 (*plogprobfinal)+=logprobf;


             } //fine iterazioni su sequenze


             // compute difference between log probability of  two iterations
             delta = fabs(logprobfinale - logprobprev);
             logprobprev = logprobfinale;
             l++;


         }
         while (delta > DELTA); // if log probability does not   change much, exit //

         *pniter = l;
         *plogprobfinal = logprobprev; //log P(O|estimated model) //




     }



#endif // MSDHMM_HMM_H
