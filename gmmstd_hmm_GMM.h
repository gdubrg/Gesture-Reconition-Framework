/*
**      Author: Tapas Kanungo, kanungo@cfar.umd.edu
**      Date:   15 December 1997
**      File:   hmm.h
**      Purpose: datastructures used for HMM. 
**      Organization: University of Maryland
**
**	Update:
**	Author: Tapas Kanungo
**	Purpose: include <math.h>. Not including this was
**		creating a problem with forward.c
**      $Id: hmm.h,v 1.9 1999/05/02 18:38:11 kanungo Exp kanungo $
**
**  Modified by Roberto Vezzani
**  
*/


#pragma once

#define DELTA 0.001 


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <numeric>


#include "gmmstd_gmm_tiny.h"
namespace gmmstd{

// classe per gli HMM che usano GMM come probabilità di emissione


template <class ForwardIterator>
int SequenceLength (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd){
	int T=0;
	for (ForwardIterator it = ObservationsBegin; it!=ObservationsEnd; ++it) T++;
	return T;
}



// media e varianza di vettori di double
template <class ForwardIterator1, class ForwardIterator2, class ForwardIterator3>
int MeansAndVarianceVD (ForwardIterator1 Begin, ForwardIterator1 End, ForwardIterator2 mean, ForwardIterator3 variance){
	
	int T=0;
	int i,iSize;
	// init dei vettori 
	iSize = (*Begin).size();
	vector<double> mean  (iSize);
	vector<double> variance (iSize);

	mean = 0;
	variance = 0;


	for (ForwardIterator1 it = Begin; it!=End; ++it) {
		// *it è un vettore di double
		for( i=0; i<iSize; i++)
		{ 
			mean[i] += (*it)[i];
			variance[i] += (*it)[i] * (*it)[i];
		}
		T++;
	}

	for( i=0; i<iSize; i++)
		{ assert (T);
			mean[i] /= T;
			variance[i] /= T;
			variance[i] -=  mean[i] * mean[i];
		}

	return T;
}



class CHMM_GMM{
 
public:
	// costruttore //--> void InitHMM(HMM *phmm, int N, int M, int seed);
	// N: numero di stat
	// M: dimension del feature vector
	// K: numero di gaussiane per ogni B
	// costruttore //--> void InitHMM(HMM *phmm, int N, int M, int seed);
	// N: numero di stat
	// M: dimension del feature vector
	// K: numero di gaussiane per ogni B
	CHMM_GMM (unsigned int N, unsigned  int M, unsigned int K);
	
	// distruttore --> void FreeHMM(HMM *phmm);
	~CHMM_GMM(){
			; // todo: nothing
		}

	
	
	//--> void ReadHMM(FILE *fp, HMM *phmm);
	//--> void PrintHMM(FILE *fp, HMM *phmm);
	//--> void InitHMM(HMM *phmm, int N, int M, int seed);

	

	// operazioni
public:

	//int GenInitalState();
	//int GenNextState(int q_t);
	//int GenSymbol(int q_t);
	//void GenSequenceArray(CArray<CArray<double>*> O, CArray<int> &q);

	// Log Likelihood calcolata con la forward (senza parametri) - iteratori std
	template <class ForwardIterator>
	double LogLikelihood (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd,Mat_<double>* alphaT=0);
	
	// calcolo del loglikelihood tenendo conto dalla lunghezza della osservazione
	template <class ForwardIterator>
	double LogLikelihood_WithDuration (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);
	
	// calcolo della probabilità della lunghezza della osservazione
	double LogLikelihoodOfDuration(int T);
	
	// init completamente random oppure con un vettore di osservazioni
	bool RandomInit();

	template <class ForwardIterator>
	bool Init_Equi (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

	template <class ForwardIterator>
	bool Init_Random (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

    template <class ForwardIterator>
    bool Init_NotRandom (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);

    template <class ForwardIterator>
    bool Init_NotRandom2 (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd);


	// funzioni per applicare vincoli sulla forma PRIMA dell'init e del learning affinchè siano efficaci
	bool SetDiagonalCovariance (bool bSet=true){
		m_bDiagonalCovariance = bSet;
		return bSet;
	}

	bool SetLeftRight (bool bSet=true){
		m_bLeftRight= bSet;
		return bSet;
	}

	

	// versioni senza alfa, beta eccc portate fuori
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

	

//void CHMM_GMM::BaumWelch(const CArray<CArray<double>*> &O, CArray2D<double> &alpha, CArray2D<double> &beta,
//	CArray2D<double> &gamma, int *pniter, double *plogprobinit, double *plogprobfinal)

	template <class BidirectionalIterator>
	void BaumWelch (BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, 
		Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma, int *pniter, double *plogprobinit, double *plogprobfinal);
		
	template <class BidirectionalIterator>
		void BaumWelch(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int *pniter, double *plogprobinit, double *plogprobfinal);

	



	
	// viterbi
	/*
	template <typename ForwardIterator, typename InsertIterator1, typename InsertIterator2, typename InsertIterator3>
	double Viterbi(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd, InsertIterator1 delta, InsertIterator2 psi, InsertIterator3 q);

	template <typename ForwardIterator, typename InsertIterator1, typename InsertIterator2, typename InsertIterator3>
	double ViterbiLog(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd, InsertIterator1 delta, InsertIterator2 psi, InsertIterator3 q);
	*/

	
	
	// -------------------------
	// RV: 23 ottobre 2008
	// inizializzazione e learning con un vettore di osservazioni; creo gli alpha, i beta, ecc.. internamente
	
	template <class BidirectionalIterator>
	bool Init_Random_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence);

	template <class BidirectionalIterator>
	void BaumWelch_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence, int *pniter, double *plogprobinit, double *plogprobfinal);


	// Funzioni di utilità
	
	void ComputeGamma(Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma);


	template <class BidirectionalIterator>
		void ComputeXi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &xi);

	template <class  BidirectionalIterator>
		void UpdateSigmaNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_sigmail, double &dDen_sigmail, int e, int i, int r, int s, int k);

		template <class  BidirectionalIterator>
	void  UpdateMuilNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_muil, double &dDen_muil, int e, int r, int k);

	template <class  BidirectionalIterator>
	void UpdateGammail(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int e, int i);
	
	double GetThreshold() {return m_dThreshold;}


	// leggo e scrivo su file
	bool LoadFromFile (FILE *f);
	bool SaveToFile (FILE *f);

	bool LoadFromFile (const char* strFileName);
	bool SaveToFile (const char* strFileName);


	// per ora lascio tutto public
public:

	unsigned int m_iN;		/* number of states;  Q={1,2,...,N} */
	unsigned int m_iM; 		/* size of the feature vector */
	unsigned int m_iK;      /* numero di gaussiane per ogni stato */

	
	
	Mat_<double> m_A;	/* A[1..N][1..N]. a[i][j] is the transition prob of going from state i at time t to state j at time t+1 */
	vector<CGMM_tiny> m_B;	/* m_B(j) è la GMM che definisce lo stato j */

	Mat_<double> m_pi;	/* pi[1..N] pi[i] is the initial state distribution. */
	Mat_<double> m_final;	/* final[1..N] final[i] is the final state distribution. */

	vector<int> inst;

	bool m_bDiagonalCovariance;
	bool m_bLeftRight;

	// gestione della durata della sequenza come parametro
	double m_dDurationMean;
	double m_dDurationVariance;

	double m_dThreshold;  // soglia appresa per il riconoscimento del modello
	bool m_bThreshold_normalized;	// thres was computed considering the normalization of sequence length?
	bool m_bThreshold_alphat;		// thres was computed considering the final state prob?
	bool m_bThreshold_length;		// thres was computed considering the statistical duration of the training sequence?

	// per uso interno
protected:
	vector<Mat_<double>*> Vp_xi; // (T,m_iN,m_iN);
	vector<Mat_<double>*> Vp_gammail; // (T,m_iK);
	vector<Mat_<double>*> Vp_gamma; // (T,m_iK);


	};


// ------------------------------------	 
// DEFINIZIONE FUNZIONI TEMPLATE
// ------------------------------------

template <class ForwardIterator>
bool CHMM_GMM::Init_NotRandom(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd){

    //Probabilità iniziali
    int i,j;
    double dVal, dSum = 0;

    //Controllo numero degli stati
    assert(m_iN);

    if(m_bLeftRight){

        //Probabilità iniziale degli stati (1, 0, ..., 0)
        fill(m_pi.begin(), m_pi.end(), 0);
        m_pi(0, 0) = 1.0;

        //Matrice A
        fill(m_A.begin(),m_A.end(),0.);
        for (i=0; i<m_iN-1; i++){
            m_A(i,i)=0.5;
            m_A(i,i+1)=0.5;
        }
        m_A(m_iN-1,m_iN-1)=1;

    }
    else{
        cout << "Inizializzazione solo con HMM left-right" << endl;
        EXIT_FAILURE;
    }

    //Matrice B
    vector<double> means (m_iN, 0.); //Vettore medie (un posto per ogni stato)
    vector<double> sqrtVariance (m_iN, 0.);
    int numChunk = m_iN;
    int sizeChunk = (ObservationsEnd - ObservationsBegin)/numChunk;
    int plus = (ObservationsEnd - ObservationsBegin)%numChunk;
    cout << "Numero pezzi: " << numChunk << ", con " << sizeChunk << " frame" << ", avanzo: " << plus << endl;

    ForwardIterator it;
    int count = 0;

    //Per ogni frame
    for(it=ObservationsBegin; it!=ObservationsEnd; ++it){
        //Per tutte le features del frame
        for(i=0; i<m_iM; ++i){
            double dVal;
            //Recupero valore
            dVal = (*it)[i];
            //Accumulo valore media
            means[count/sizeChunk] += dVal;
            //Accumulo valore varianza
            sqrtVariance[count/sizeChunk] += dVal*dVal;
        }
        //cout << (count)/sizeChunk << " " << count <<  " " << means[count/sizeChunk] << endl;

        //Prendo anche i frame che avanzano dalla divisione
        if( !(count == (ObservationsEnd - ObservationsBegin)-plus-1) )
            count++;
    }

    //Calcolo medie e varianze vere
    int total = m_iM*sizeChunk;
    for(i=0; i<m_iN-1; ++i){
        means[i] /= total;
        sqrtVariance[i] = 3.0*sqrt( (sqrtVariance[i]/total) - (means[i]*means[i]) ); //cout << means[i] << sqrtVariance[i] << endl;
    }
    //Ultimo posto può avere un numero di frame maggiore di quello calcolato
    total += (plus*m_iM);
    means[m_iN-1] /= total;
    sqrtVariance[m_iN-1] = 3.0*sqrt( (sqrtVariance[m_iN-1]/total) - (means[m_iN-1]*means[m_iN-1]) );
    //cout << means[m_iN-1] << sqrtVariance[m_iN-1] << endl;

    //Normalizzazione gaussiane
    int n,m,k;
    for (n=0; n<m_iN; n++)
        for (k=0; k<m_iK; k++)
            m_B[n].GetGaussian(k).SetToNormal();

    //Assegno le gaussiane
    for (n=0; n<m_iN; n++){
        for (k=0; k<m_iK; k++){
            for (m=0; m<m_iM; m++){
                // setto valore medio delle gaussiane
                m_B[n].MeanValue(k,m) = means[n];
                // setto varianza delle gaussiane
                m_B[n].CoVarianceValue(k,m,m)= (sqrtVariance[n]/3.0)*(sqrtVariance[n]/3.0);
            }
            // peso uniforme
            m_B[n].WeightValue(k)=1.0 / m_iK;
        }
    }

    m_dDurationMean = count;
    m_dDurationVariance = 1;

    return true;
}

template <class ForwardIterator>
bool CHMM_GMM::Init_NotRandom2(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd){

    //Probabilità iniziali
    int i,j;
    double dVal, dSum = 0;

    //Controllo numero degli stati
    assert(m_iN);

    if(m_bLeftRight){

        //Probabilità iniziale degli stati (1, 0, ..., 0)
        fill(m_pi.begin(), m_pi.end(), 0);
        m_pi(0, 0) = 1.0;

        //Matrice A
        fill(m_A.begin(),m_A.end(),0.);
        for (i=0; i<m_iN-1; i++){
            m_A(i,i)=0.5;
            m_A(i,i+1)=0.5;
            m_A(i,i+2)=0.5;
        }
        m_A(m_iN-1,m_iN-1)=1;

    }
    else{
        cout << "Inizializzazione possibile solo con HMM left-right" << endl;
        EXIT_FAILURE;
    }

    //Matrice B
    //vector<double> means(m_iN, 0.);//(m_iN, 0.); //Vettore medie (un posto per ogni stato)
    vector<vector<double>> means(m_iN, vector<double>(m_iM));
    //vector<double> sqrtVariance (m_iN, 0.);
    vector<vector<double>> sqrtVariance(m_iN, vector<double>(m_iM));
    int numChunk = m_iN;
    int sizeChunk = (ObservationsEnd - ObservationsBegin)/numChunk;
    int plus = (ObservationsEnd - ObservationsBegin)%numChunk;
    //DEBUG cout << "Numero pezzi: " << numChunk << ", con " << sizeChunk << " frame" << ", avanzo: " << plus << endl;

    ForwardIterator it;
    int count = 0;

    //Per ogni frame
    for(it=ObservationsBegin; it!=ObservationsEnd; ++it){
        //Per tutte le features del frame
        for(i=0; i<m_iM; ++i){
            double dVal;
            //Recupero valore
            dVal = (*it)[i];
            //Accumulo valore media
            means[count/sizeChunk][i] += dVal;
            //Accumulo valore varianza
            sqrtVariance[count/sizeChunk][i] += dVal*dVal;
        }
        //cout << (count)/sizeChunk << " " << count <<  " " << means[count/sizeChunk] << endl;

        //Prendo anche i frame che avanzano dalla divisione
        if( !(count == (ObservationsEnd - ObservationsBegin)-plus-1) )
            count++;
    }

    //Calcolo medie e varianze vere
    int total = sizeChunk;
    for(i=0; i<m_iN-1; ++i){
        for(int k=0; k<m_iM; ++k){
            means[i][k] /= total;
            sqrtVariance[i][k] = 20.0*sqrt( (sqrtVariance[i][k]/total) - (means[i][k]*means[i][k]) );
            //cout << means[i][k] << sqrtVariance[i][k] << endl;
        }
    }
    //Ultimo posto può avere un numero di frame maggiore di quello calcolato
    total += plus;
    for(i=0; i<m_iM; ++i){
        means[m_iN-1][i] /= total;
        sqrtVariance[m_iN-1][i] = 20.0*sqrt( (sqrtVariance[m_iN-1][i]/total) - (means[m_iN-1][i]*means[m_iN-1][i]) );
    }
    //cout << means[m_iN-1] << sqrtVariance[m_iN-1] << endl;

    //Normalizzazione gaussiane
    int n,m,k;
    for (n=0; n<m_iN; n++)
        for (k=0; k<m_iK; k++)
            m_B[n].GetGaussian(k).SetToNormal();

    //Assegno le gaussiane
    for (n=0; n<m_iN; n++){
        for (k=0; k<m_iK; k++){
            for (m=0; m<m_iM; m++){
                // setto valore medio delle gaussiane
                m_B[n].MeanValue(k,m) = means[n][m];
                // setto varianza delle gaussiane
                m_B[n].CoVarianceValue(k,m,m)= (sqrtVariance[n][m])*(sqrtVariance[n][m]);
            }
            // peso uniforme
            m_B[n].WeightValue(k)=1.0 / m_iK;
        }
    }

    m_dDurationMean = count;
    m_dDurationVariance = 1;

    return true;
}

// ------------------------------------
// INIT


	// init con un vettore di osservazioni
	// probabilità degli stati: equamente distribuiti
	// matrice A: equamente distrib
	//bool CHMM_GMM::Init_Random(const CArray<CArray<double>*> &O){

	template <class ForwardIterator>
	bool CHMM_GMM::Init_Random (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd)
	{

		// probabilità iniziali
		int i,j;
		double dVal,dSum=0;
		
		assert(m_iN);

		if (m_bLeftRight){
			// prob iniziale
			fill(m_pi.begin(),m_pi.end(), 0);
			m_pi(0,0)=1.0;
			//matrice A: 
			fill(m_A.begin(),m_A.end(),0.); 
			for (i=0; i<m_iN-1; i++){
				m_A(i,i)=0.5;
				m_A(i,i+1)=0.5;
			}
			m_A(m_iN-1,m_iN-1)=1;
		}

		else
		{   // prob iniz
			fill(m_pi.begin(),m_pi.end(), 1.0/m_iN);
			//matrice A: 
			fill(m_A.begin(),m_A.end(),1.0/m_iN); 

		}
		


		
		// matrice B: distribuzioni random ma comprese tra valori compatibili con le osservazioni
		vector<double> means (m_iM,0.);
		vector<double> Sqrtvariance (m_iM,0.);
		

		// per ogni osservazione
		ForwardIterator it;
		int count = 0;
		for (it = ObservationsBegin; it != ObservationsEnd; ++it){
			// per ogni coordinata
			for (i=0; i<m_iM; i++)
			{   double dVal;
				// recupero valore
				dVal = (*it)[i];
				means[i]+=dVal ;
				Sqrtvariance[i]+=dVal*dVal;
			}
			count++;
		}

		// calcolo varianze e medie vere
		for (i=0; i<m_iM; i++)
			{
				means[i]/=count;
				Sqrtvariance[i]=3.0*sqrt((Sqrtvariance[i]/count)- (means[i]*means[i]));
			}


		int n,m,k;
		// ora imposto tutte le gaussiane a normali (covarianza diagonale)
		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
				m_B[n].GetGaussian(k).SetToNormal();
			

		
		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
			{
				for (m=0; m<m_iM; m++)
				{
					// setto valore medio delle gaussiane
					m_B[n].MeanValue(k,m) = random_Uniform(means[m]-Sqrtvariance[m],means[m]+Sqrtvariance[m]);
					// setto varianza delle gaussiane
					m_B[n].CoVarianceValue(k,m,m)=(Sqrtvariance[m]/3.0)*(Sqrtvariance[m]/3.0);
				}
				// peso uniforme
				m_B[n].WeightValue(k)=1.0 / m_iK;
			}


		m_dDurationMean= count;
		m_dDurationVariance=1;

		return true;
	}


	// init con un vettore di osservazioni
	// probabilità degli stati: equamente distribuiti
	// matrice A: 0.6 sulla diagonale, il resto equamente distrib
	// gaussiane: equamente distribuite 
	//bool CHMM_GMM::Init_Equi(const CArray<CArray<double>*> &O){
	template <class ForwardIterator>
	bool CHMM_GMM::Init_Equi (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd)
	{

		// probabilità iniziali
		int i,j;
		double dVal,dSum=0;

		if (m_bLeftRight){
			// prob iniziale
			fill(m_pi.begin(),m_pi.end(), 0);
			m_pi(0,0)=1.0;
			//matrice A: 
			fill(m_A.begin(),m_A.end(),0.); 
			for (i=0; i<m_iN-1; i++){
				m_A(i,i)=0.5;
				m_A(i,i+1)=0.5;
			}
			m_A(m_iN-1,m_iN-1)=1;
		}

		else
		{   // prob iniz
			fill(m_pi.begin(),m_pi.end(), 1.0/m_iN);
			//matrice A: 0.6 sulla diagonale, 0.4/(numstates-1) dalle altre parti
			fill(m_A.begin(),m_A.end(), 0.4/(m_iN-1));
			for (i=0; i<m_iN; i++)
				m_A(i,i)=0.6;

		}
		

		
		
		// matrice B: distribuzioni random ma comprese tra valori compatibili con le osservazioni
		vector<double> means (m_iM,0);
		vector<double> Sqrtvariance (m_iM,0);

		// per ogni osservazione
		ForwardIterator it;
		int T = 0;
		for (it = ObservationsBegin; it != ObservationsEnd; ++it){
			// per ogni coordinata
			for (i=0; i<m_iM; i++)
			{   double dVal;
				dVal = (*it)[i];
				means[i]+=dVal;
				Sqrtvariance[i]+=dVal*dVal;
			}
			T++;
		}

		// calcolo varianze e medie vere
		for (i=0; i<m_iM; i++)
			{
				means[i]/=T;
				Sqrtvariance[i]=sqrt((Sqrtvariance[i]/T)- (means[i]*means[i]));
			}


		int n,m,k;
		// ora imposto le gaussiane a normali
		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
				m_B[n].GetGaussian(k).SetToNormal();
			

		
		
		
		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
			{
				for (m=0; m<m_iM; m++)
				{
					// setto valore medio delle gaussiane
					m_B[n].MeanValue(k,m) = means[m]+ (2*(k-(m_iK/2))*Sqrtvariance[m]);
					// setto varianza delle gaussiane
					m_B[n].CoVarianceValue(k,m,m)=(Sqrtvariance[m])*(Sqrtvariance[m]);
				}
				// peso uniforme
				m_B[n].WeightValue(k)=1.0 / m_iK;
			}

		
		m_dDurationMean= T;
		m_dDurationVariance=1;




		return true;
	}

	template <class ForwardIterator>
	double CHMM_GMM::LogLikelihood (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd,Mat_<double> *alphaT){
	
		int T;
		T= SequenceLength(ObservationsBegin,ObservationsEnd);
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


	//	double CHMM_GMM::LogLikelihood_WithDuration(const CArray<CArray<double>*> & O){
	template <class ForwardIterator>
	double CHMM_GMM::LogLikelihood_WithDuration (ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd)
	{
		// qui secondo me manca 
		// T= SequenceLength(ObservationsBegin,ObservationsEnd);

		double logprobf;
		logprobf = LogLikelihood( ObservationsBegin, ObservationsEnd);
		double dProbLung;
		dProbLung = LogLikelihoodOfDuration(T);
		return logprobf + dProbLung;
	}

			

	template <class BidirectionalIterator>
	bool CHMM_GMM::Init_Random_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence)
	{
		// probabilità iniziali
		int i,j;
		double dVal,dSum=0;


		if (m_bLeftRight){
			// prob iniziale
			fill(m_pi.begin(),m_pi.end(), 0);
			m_pi(0,0)=1.0;
			//matrice A: 
			fill(m_A.begin(),m_A.end(),0.); 
			for (i=0; i<m_iN-1; i++){
				m_A(i,i)=0.5;
				m_A(i,i+1)=0.5;
			}
			m_A(m_iN-1,m_iN-1)=1;
		}

		else
		{   // prob iniz
			fill(m_pi.begin(),m_pi.end(), 1.0/m_iN);
			//matrice A: 0.6 sulla diagonale, 0.4/(numstates-1) dalle altre parti
			fill(m_A.begin(),m_A.end(), 0.4/(m_iN-1));
			for (i=0; i<m_iN; i++)
				m_A(i,i)=0.6;

		}

		
		// matrice B: distribuzioni random ma comprese tra valori compatibili con le osservazioni
		vector<double> means (m_iM,0.);
		vector<double> Sqrtvariance (m_iM,0.);
		
		BidirectionalIterator seq;
		double T=0; // total duration
		int E=0; // number of sequences
		double SumOfSquareT=0;
	
		for (seq = FirstSequence; seq != LastSequence; ++seq)
		{
			int len;
			means = std::accumulate((*seq).begin(), (*seq).end(), means, vecsum<vector<double>>());
			Sqrtvariance = std::accumulate((*seq).begin(), (*seq).end(), Sqrtvariance, vecsumquad<vector<double>>());
			len = SequenceLength((*seq).begin(), (*seq).end());
			T += len;
			SumOfSquareT += len*len;
			E++;
		}

		
		// calcolo varianze e medie vere
		for (i=0; i<m_iM; i++)
			{
				means[i]/=T; // T*E;
				Sqrtvariance[i]=3.0*sqrt((Sqrtvariance[i]/T)- (means[i]*means[i]));
			}


		int n,m,k;
		// ora imposto le gaussiane a normali
		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
				m_B[n].GetGaussian(k).SetToNormal();
			

		for (n=0; n<m_iN; n++)
			for (k=0; k<m_iK; k++)
			{
				for (m=0; m<m_iM; m++)
				{
					// setto valore medio delle gaussiane
					m_B[n].MeanValue(k,m) = random_Uniform(means[m]-3.0*Sqrtvariance[m],means[m]+3.0*Sqrtvariance[m]);
					// setto varianza delle gaussiane
					m_B[n].CoVarianceValue(k,m,m)=(Sqrtvariance[m])*(Sqrtvariance[m]);
				}
				// peso uniforme
				m_B[n].WeightValue(k)=1.0 / m_iK;
			}


		// media e varianza delle lunghezze
		m_dDurationMean =  T / E;
		m_dDurationVariance = (SumOfSquareT / E) - (m_dDurationMean *m_dDurationMean);


		return true;
	}


// ------------------------------------
// ------------------------------------
// ------------------------------------
// forward


	
//void Forward(HMM *phmm, int T, int *O, double **alpha, double *pprob)
template <typename ForwardIterator>
double CHMM_GMM::Forward(ForwardIterator ObservationsBegin, ForwardIterator ObservationsEnd)
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
double CHMM_GMM::ForwardWithScale(ForwardIterator1 ObservationsBegin, ForwardIterator1 ObservationsEnd, Mat_<double> &alpha, ForwardIterator2 ScaleBegin)
/*  pprob is the LOG probability */
{
	unsigned int	i, j; 	/* state indices */
	unsigned int	t;	/* time index */

	for(int i=0; i<8;++i)
		inst[i] = 0;

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
	double ok8 = true;
	double ok7 = true;
	double ok6 = true;
	double ok5 = true;
	double ok4 = true;
	double ok3 = true;
	double ok2 = true;
	double ok1 = true;
    for (; it != ObservationsEnd; ++it, t++){

		dScale = 0.0;
		for (j = 0; j < m_iN; j++) {
			sum = 0.0;
			for (i = 0; i < m_iN; i++) // per ogni stato di partenza
				sum += alpha(t-1,i)* (m_A(i,j)); 

			//alpha(t+1,j) = sum*(m_B(j,O[t+1]));
			dBjO = m_B[j].GetLikelihood(*it,false);
			alpha(t,j) = sum * dBjO;


			dScale += alpha(t,j);
			
		}

		assert (dScale);

			

		for (j = 0; j < m_iN; j++) {
			alpha(t,j) /= dScale; 
		}

		//ANALISI ISTANTE TRANSIZIONE
		if(alpha(t, (m_iN-8))>0.90 && ok1){
			inst[0] = t;
			ok1 = false;
		}
		if(alpha(t, (m_iN-7))>0.90 && ok2){
			inst[1] = t;
			ok2 = false;
		}
		if(alpha(t, (m_iN-6))>0.90 && ok3){
			inst[2] = t;
			ok3 = false;
		}
		if(alpha(t, (m_iN-5))>0.90 && ok4){
			inst[3] = t;
			ok4 = false;
		}
		if(alpha(t, (m_iN-4))>0.90 && ok5){
			inst[4] = t;
			ok5 = false;
		}
		if(alpha(t, (m_iN-3))>0.90 && ok6){
			inst[5] = t;
			ok6 = false;
		}
		if(alpha(t, (m_iN-2))>0.90 && ok7){
			inst[6] = t;
			ok7 = false;
		}
		if(alpha(t, (m_iN-1))>0.90 && ok8){
			inst[7] = t;
			ok8 = false;
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

	// la somma dei log delle scale è il valore da restituire
	return dprob;
	
}


// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------
// Backward



//void Backward(HMM *phmm, int T, int *O, double **beta, double *pprob)
//double CHMM_GMM::Backward(const CArray<CArray<double>*> &O, CArray2D<double> &beta)
template <typename BackwardIterator>
double CHMM_GMM::Backward(BackwardIterator ObservationsBegin, BackwardIterator ObservationsEnd)
{
        unsigned int     i, j;   /* state indices */
         int     t;      /* time index */
        double sum;
		double dBjO;

	BackwardIterator it;	
	unsigned int T;
	T = SequenceLength (ObservationsBegin, ObservationsEnd);
	
 
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
	double CHMM_GMM::BackwardWithScale(BidirectionalIterator1 ObservationsBegin, BidirectionalIterator1 ObservationsEnd, Mat_<double> &beta, BidirectionalIterator2 ScaleBegin)
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
				dBjO = m_B[j].GetLikelihood(*it,false);
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
	
	


//-----------------------
// versione con vettore di osservazioni
// devo fare tutto come il caso di singola osservazione, ma con vettori 
//void CHMM_GMM::BaumWelch( const CArray<CArray<CArray<double>*>*> &SetofO, int *pniter, double *plogprobinit, double *plogprobfinal){
	template <class BidirectionalIterator>
	void CHMM_GMM::BaumWelch_Multiple(BidirectionalIterator FirstSequence, BidirectionalIterator LastSequence, int *pniter, double *plogprobinit, double *plogprobfinal)
{
	unsigned int	e,i, j, k;
	unsigned int	t, l = 0;

	// leggo il numero di osservazioni
	unsigned int E;
	E = SequenceLength(FirstSequence,LastSequence);

	
	unsigned int T;
	// non esiste un T fisso...
	//	T= O.GetSize();

	double dOld, dNew;

	double	logprobf, logprobb;
	double	numeratorA, denominatorA;

	Vp_xi.clear();
	Vp_gammail.clear();
	Vp_gamma.clear();

	//CArray<CArray<double>*> Vp_scale (0); // (T);

	// creo le aree dati
	//for (e=0; e<E; e++)
	e=0;
	for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e)
	{

		T= SequenceLength((*itSeq).begin(),(*itSeq).end());
		
		int iSize[] = {T,m_iN,m_iN};
		Mat_<double>* p_xi  = new Mat_<double> (3,iSize);
		Vp_xi.push_back(p_xi);

		Mat_<double>* p_gamma  = new Mat_<double> (T,m_iN);
		Vp_gamma.push_back(p_gamma);

		Mat_<double>* p_gammail  = new Mat_<double> (T,m_iK);
		Vp_gammail.push_back(p_gammail);

	
	}

	

	double delta, deltaprev, logprobprev;
	deltaprev = 10e-70;

	

	// INIT - calcolo valori iniziali
	*plogprobinit =0;
	double dSumOfT, dSumofSquareT;
	dSumOfT = 0;
	dSumofSquareT=0;
	//for (e=0; e<E; e++)
	e=0;
	for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e)
	{
		
		//CArray<CArray<double>*> & O = *SetofO[e];
		T= SequenceLength((*itSeq).begin(),(*itSeq).end());
		dSumOfT+=T;
		dSumofSquareT+= T*T;

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

		// CONTROLLO CORRETTEZZA GAMMA E XI
		for (int t1 = 0; t1<T; t1++)
		{
			double dSumxij=0;
			for (int i = 0; i<m_iN; i++)
			{
				for (int j = 0; j<m_iN; j++)
				{
					dSumxij+= xi(t1,i,j);
				}
			}

				
		}

	}

	logprobprev = *plogprobinit;


	// ------------------------------------------------------
	// ITERAZIONI FINO A CONVERGENZA 

	do  {	

		// reestimate frequency of state i in time t=1 
		for (i = 0; i < m_iN; i++) {
			double dVal;
			dVal = 0;
			for (e = 0; e < E; e++){
				Mat_<double> & gamma = *Vp_gamma[e];
				dVal+= gamma(0,i);
			}
			dVal /= E;
			//m_pi(i,0) = (DELTA / m_iN) + (1.0-DELTA) * dVal;  // fattore DELTA per evitare di avere 0 
			m_pi(i,0) = dVal;  
		}

		

		// reestimate transition matrix and symbol prob in each state 
		for (i = 0; i < m_iN; i++) { 

			
			// CALCOLO IL DENOMINATORE, CHE è COMUNE A TUTTI
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

			// rinormalizzo A affinchè sia stocastica
			//assert(fabs(dSummmm-1)<.001);
			if (fabs(dSummmm-1)>.001)
				for (j = 0; j < m_iN; j++) {
					m_A(i,j) /= dSummmm;
				}



			// formule sul paper
			// calcolo i gammail-esimi (log)
			//for (e = 0; e < E; e++){
			e=0;
			for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e)
			{

				UpdateGammail((*itSeq).begin(),(*itSeq).end(),e,i);

			}


			//update di cil, muil, sigmail
			double dNum_cil;
			
			// cil (ovvero i m_B[i].WeightValue(k))
			// denominatore è lo stesso di denominatorA, quindi non lo ricalcolo
			for (k = 0; k < m_iK; k++){
				dNum_cil = 0;
				// sommo anche in E
				for (e = 0; e < E; e++){
					Mat_<double> & gammail = *Vp_gammail[e];
					T= gammail.rows;
					for (t = 0; t < T; t++){
						dNum_cil+= gammail(t,k);
					}
				}
				dOld = m_B[i].WeightValue(k);
				dNew = dNum_cil / denominatorA;
				m_B[i].WeightValue(k)=dNew;
			}


			//muil
			unsigned int r,s;
			double dNum_muil,dDen_muil;
			for (k = 0; k < m_iK; k++){  // for each gaussian
				for (r=0; r< m_iM; r++){  // for each coordinate
					dNum_muil = 0;
					dDen_muil = 0;
					e=0;
					for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq)
					{
						//for (t = 0; t < T; t++){  // for each t
						UpdateMuilNumDen((*itSeq).begin(),(*itSeq).end(),dNum_muil,dDen_muil,e,r,k);
						e++;
						
					}
					dOld = m_B[i].MeanValue(k,r);
					dNew = dNum_muil / dDen_muil;
					m_B[i].MeanValue(k,r)=dNew;
				}
			}


			//sigmail
			double dNum_sigmail,dDen_sigmail;
			for (k = 0; k < m_iK; k++){  // for each gaussian
				for (r=0; r< m_iM; r++){  // for each coordinate
						for (s=0; s< m_iM; s++){  // for each coordinate
							dNum_sigmail = 0;
							dDen_sigmail = 0;
							//	for (e = 0; e < E; e++){
							e=0;
							for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq)
							{
								//CArray<CArray<double>*> & O = *SetofO[e];
								//T= gammail.GetSizeX();
								//for (t = 0; t < T; t++){  // for each t
								UpdateSigmaNumDen((*itSeq).begin(),(*itSeq).end(),dNum_sigmail,dDen_sigmail,e,i,r,s,k);
								e++;
									
							}

							// TODO: Controllare vincoli per evitare che sia singolare la matrice!!!

							dOld=m_B[i].CoVarianceValue(k,r,s);
							if (r!=s) {
								if ((dNum_sigmail <= DELTA*dDen_sigmail)|| (m_bDiagonalCovariance))
									m_B[i].CoVarianceValue(k,r,s)=0;
								else
									m_B[i].CoVarianceValue(k,r,s)= dNum_sigmail / dDen_sigmail;
							}
							else{  /// sulla diagonale
								if (dNum_sigmail <= DELTA*dDen_sigmail)
									m_B[i].CoVarianceValue(k,r,s)=DELTA;
								else
									m_B[i].CoVarianceValue(k,r,s)= dNum_sigmail / dDen_sigmail;
							}

					}
				}
				// ricalcolo le inverse
				m_B[i].GetGaussian(k).InverseRecalc();
			}
			
		}

		// devo rifare i calcoli
		double logprobfinale;
		logprobfinale = 0;
		e=0;
		for (BidirectionalIterator itSeq=FirstSequence; itSeq != LastSequence; ++itSeq, ++e)
		{
		
			//CArray<CArray<double>*> & O = *SetofO[e];
			T= SequenceLength((*itSeq).begin(),(*itSeq).end());

			
			// alpha, beta e scale non devo conservarli e sono dipendenti da T. 
			Mat_<double> alpha(T,m_iN);
			Mat_<double> beta(T,m_iN);
			vector<double> scale (T);
			

			Mat_<double> & gamma = *Vp_gamma[e];
			Mat_<double> & xi= *Vp_xi [e];
			

			logprobf = ForwardWithScale( (*itSeq).begin(),(*itSeq).end(), alpha, scale.begin());
			logprobb = BackwardWithScale( (*itSeq).begin(),(*itSeq).end(), beta, scale.begin());
			ComputeGamma(alpha, beta, gamma);
			ComputeXi((*itSeq).begin(),(*itSeq).end(), alpha, beta, xi);
			logprobfinale+=logprobf;
		}
						
		// compute difference between log probability of  two iterations 
		delta = fabs(logprobfinale - logprobprev); 
		logprobprev = logprobfinale;
		l++;
		
		
	}
	while (delta > DELTA); // if log probability does not   change much, exit //

	*pniter = l;
	*plogprobfinal = logprobprev; //log P(O|estimated model) //

	// media e varianza delle lunghezze
		m_dDurationMean = dSumOfT / E;
		m_dDurationVariance = (dSumofSquareT / E) - (m_dDurationMean *m_dDurationMean);

}



template <class  BidirectionalIterator>
void  CHMM_GMM::UpdateSigmaNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_sigmail, double &dDen_sigmail, int e, int i,  int r, int s, int k)
{
	int t;
	t=0;
	Mat_<double> & gammail = *Vp_gammail[e];
	for (BidirectionalIterator itObs = FirstObservation; itObs  != LastObservation; ++itObs)
		{
			dNum_sigmail+= gammail(t,k) * ((*itObs)[r] -m_B[i].MeanValue(k,r))*((*itObs)[s]-m_B[i].MeanValue(k,s));
			dDen_sigmail+= gammail(t,k);
			t++;
		}
}

template <class  BidirectionalIterator>
void  CHMM_GMM::UpdateMuilNumDen(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, double &dNum_muil, double &dDen_muil, int e, int r, int k)
{
	int t;
	t=0;
	Mat_<double> & gammail = *Vp_gammail[e];

	for (BidirectionalIterator itObs = FirstObservation; itObs  != LastObservation; ++itObs)
		{
			dNum_muil+= gammail(t,k) * ((*itObs) [r]);
			dDen_muil+= gammail(t,k);
			t++;
		}
}



template <class  BidirectionalIterator>
void CHMM_GMM::UpdateGammail(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int e, int i)
{
	Mat_<double> & gamma = *Vp_gamma[e];
	Mat_<double> & gammail = *Vp_gammail[e];

				//CArray<CArray<double>*> & O = *SetofO[e];
				//T= gamma.GetSizeX();
				//for (t = 0; t < T; t++){
	int t=0;
	for (BidirectionalIterator itObs = FirstObservation; itObs  != LastObservation; ++itObs, ++t)
	{
			double dTerm1;
			dTerm1 = log(gamma(t,i)) - m_B[i].GetLogLikelihood(*itObs,false);
			for (int k = 0; k < m_iK; k++)
			{
				gammail(t,k) = exp(dTerm1 + log(m_B[i].WeightValue(k)) + (m_B[i].GetLogLikelihood_partial(*itObs,k,false)));
			}
	}
}
	



	// --------------------------------------


template <class BidirectionalIterator>
void CHMM_GMM::BaumWelch (BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, 
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
	double dNum_sigmail,dDen_sigmail;
	double dNum_cil,dDen_cil;
	double dSum;
	unsigned int r,s;
	double dNum_muil,dDen_muil;
	

	const int Size[] = {T,m_iN,m_iN};
	 
	Mat_<double> xi (3,Size);
	Mat_<double> gammail (T,m_iK);
	vector<double> scale (T,1);

	double delta, deltaprev, logprobprev;
	deltaprev = 10e-70;

	logprobf = ForwardWithScale ( FirstObservation,LastObservation, alpha, scale.begin());
	*plogprobinit = logprobf; /* log P(O |intial model) */

	//DEBUG cout << "iniziale: " <<logprobf << endl;
	
	logprobb = BackwardWithScale( FirstObservation,LastObservation, beta, scale.begin());



	ComputeGamma(alpha, beta, gamma);
	ComputeXi( FirstObservation, LastObservation, alpha, beta, xi);

	logprobprev = logprobf;

	do  {	

		/* reestimate frequency of state i in time t=1 */
		
		dSum=0;
		for (i = 0; i < m_iN; i++) {
			/*m_pi(i,0) = (DELTA/m_iN) + (1.0-DELTA) * gamma(0,i);
			dSum += m_pi(i,0);
			*/
			
			m_pi(i,0) = gamma(0,i);

		}

		
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

				//assert(denominatorA);
				//assert(numeratorA <= denominatorA);
				//m_A(i,j) = (DELTA/m_iN) + (1.0-DELTA) *numeratorA/denominatorA;
				if (denominatorA)
					m_A(i,j) = numeratorA/denominatorA;
				else
					m_A(i,j) = 0; 

				dSum +=m_A(i,j);
			}


		
			// rinormalizzo A affinchè sia stocastica, se qualcosa è andato storto
			//assert(fabs(dSum-1)<.001);
            /* arg....
			if (fabs(dSum-1)>.001)
				for (j = 0; j < m_iN; j++) {
					m_A(i,j) /= dSum;
				}
			else
				for (j = 0; j < m_iN; j++) {
					m_A(i,j) = 1.0 / m_iN;
				}
                */


			
			// formule sul paper
			// calcolo i gammail (log)
			t=0;
			for ( BidirectionalIterator it = FirstObservation; it != LastObservation; ++it, ++t) {

				double dTerm1;
				double dTerm2;

				dTerm1 = log(gamma(t,i)) - m_B[i].GetLogLikelihood(*it,false);
				double dSum;
				dSum =0;
				for (k = 0; k < m_iK; k++)
				{
					dTerm2 = m_B[i].GetLogLikelihood_partial(*it,k,false);
					gammail(t,k) = m_B[i].WeightValue(k) * exp(dTerm1 + dTerm2);
					dSum+=gammail(t,k);
					
				}

				// gammail devono sommare a gamma
			}


			//update
			
			
			// cil
			for (k = 0; k < m_iK; k++){
				dNum_cil = 0;
				dDen_cil = 0;
				for (t = 0; t < T; t++){
					dNum_cil+= gammail(t,k);
					dDen_cil+= gamma(t,i);
				}
				dOld = m_B[i].WeightValue(k);
				assert(dDen_cil);
				dNew = dNum_cil / dDen_cil;
				m_B[i].WeightValue(k)=dNew;
			}

			
			// controllo somme dei pesi
			double dSum=0;
			
			for (k = 0; k < m_iK; k++){
				
				dSum += m_B[i].WeightValue(k);
			}

			
			// controllo
			assert(fabs(dSum-1)<.001);
			
				

			//muil

			for (k = 0; k < m_iK; k++){  // for each gaussian
				for (r=0; r< m_iM; r++){  // for each coordinate
					dNum_muil = 0;
					dDen_muil = 0;
					t=0;
					for ( BidirectionalIterator it = FirstObservation; it != LastObservation; ++it, ++t) {
						dNum_muil+= gammail(t,k) * (*it)[r];
						dDen_muil+= gammail(t,k);
					}
					dOld = m_B[i].MeanValue(k,r);
					if (dDen_muil){
						//assert(dDen_muil!=0);
						// gaussiana che è morta
						dNew = dNum_muil / dDen_muil;
						m_B[i].MeanValue(k,r)=dNew;
					}
				}
			}

			//sigmail
			
			for (k = 0; k < m_iK; k++){  // for each gaussian
				for (r=0; r< m_iM; r++){  // for each coordinate
						for (s=0; s< m_iM; s++){  // for each coordinate
							dNum_sigmail = 0;
							dDen_sigmail = 0;
							t=0;
							for ( BidirectionalIterator it = FirstObservation; it != LastObservation; ++it, ++t) {
								dNum_sigmail+= gammail(t,k) * ((*it)[r] -m_B[i].MeanValue(k,r))*((*it)[s]-m_B[i].MeanValue(k,s));
								dDen_sigmail+= gammail(t,k);
							}
							dOld=m_B[i].CoVarianceValue(k,r,s);
							if (dDen_sigmail){
								assert (dDen_sigmail);
								if (r!=s){
									if ((dNum_sigmail <= DELTA *  dDen_sigmail )|| (m_bDiagonalCovariance))
										m_B[i].CoVarianceValue(k,r,s)=0;
									else
										m_B[i].CoVarianceValue(k,r,s)= dNum_sigmail / dDen_sigmail;
								}

								else // elemento sulla diagonale
								{
									if (dNum_sigmail <= DELTA *  dDen_sigmail )
										m_B[i].CoVarianceValue(k,r,s)=DELTA;  // piccolo -> lo porto almeno a DELTA
									else
										m_B[i].CoVarianceValue(k,r,s)= dNum_sigmail / dDen_sigmail;
								}

							}
							//dNew=m_B[i].CoVarianceValue(k,r,s);
							///dNew = dNew;
					}
				}
				// ricalcolo le inverse
				m_B[i].GetGaussian(k).InverseRecalc();
			}



			/* sostiuita sopra
			denominatorB = denominatorA + gamma(T-1,i); 
			for (k = 0; k < m_iM; k++) {
				numeratorB = 0.0;
				for (t = 0; t < T; t++) {
					if (O[t] == k) 
						numeratorB += gamma(t,i);
				}

				m_B(i,k) = .001 +
						.999*numeratorB/denominatorB;
			}
			*/
		}


		logprobf= ForwardWithScale( FirstObservation, LastObservation, alpha, scale.begin());
		logprobb= BackwardWithScale(FirstObservation, LastObservation, beta, scale.begin());
		ComputeGamma(alpha, beta, gamma);
		ComputeXi(FirstObservation, LastObservation, alpha, beta, xi);

		
		// provo a fare un decode della sequenza
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
				//DEBUG cout << "t: " << t << " - State: " << iMaxState << endl;
		   }

    
		
		
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

	// media e varianza delle lunghezze
	m_dDurationMean = T;
	m_dDurationVariance = 25; //TODO: SOGLIA O VALORE ...

	
}


	
//void CHMM_GMM::BaumWelch(const CArray<CArray<double>*> &O, int *pniter, double *plogprobinit, double *plogprobfinal)
template <class BidirectionalIterator>
	void CHMM_GMM::BaumWelch(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, int *pniter, double *plogprobinit, double *plogprobfinal)

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
void CHMM_GMM::ComputeXi(BidirectionalIterator FirstObservation, BidirectionalIterator LastObservation, Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &xi)
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
				dBjO = m_B[j].GetLikelihood(*it,false);
				xi(t,i,j) = alpha(t,i)*beta(t+1,j)
					*(m_A(i,j))
					*dBjO;
				if (xi(t,i,j)==0)
					sum=sum;
				sum += xi(t,i,j);
			}

		assert (sum);
		
		for (i = 0; i < m_iN; i++) 
			for (j = 0; j < m_iN; j++) 
				xi(t,i,j)  /= sum;
	}
}

	
	} // namespace
