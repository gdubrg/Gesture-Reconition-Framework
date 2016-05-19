//------------------------------------------------------------------
//
// Nome file: gmm_tiny.h
// Contenuto: Gaussian Mixture Model (tiny version rifatta da me) 
// Data Creazione: 01/10/08
// Autore: Roberto Vezzani	
//
//------------------------------------------------------------------




#pragma once

#define THLOG -150
#define THVAL 0.00001

#include <math.h>
#include "opencv2/opencv.hpp"

#include <vector>
#include <iterator>

using namespace cv;
using namespace std;


namespace gmmstd{

double rand_BoxMuller();
double random_Uniform(double lowest, double highest) ;




// singola gaussiana
class CGaussian
{
public:
	
	CGaussian():
	   m_iSize (1), m_means(1,1,0.),m_covariance(1,1,0.), m_InverseCovariance(1,1,0.)
		{	SetToNormal();	}
	
	   // copy constructor
	   CGaussian(const CGaussian & ref)
	   {
		   m_iSize =ref.GetSize();
		   m_means = ref.m_means.clone();
		   m_covariance= ref.m_covariance.clone();
		   m_InverseCovariance= ref.m_InverseCovariance.clone();
		   m_dLogCovarianceDeterminant = ref.m_dLogCovarianceDeterminant;
	   }

	   //assegnamento
	   CGaussian & operator= (const CGaussian & ref)
	   {
		   m_iSize =ref.GetSize();
		   m_means = ref.m_means.clone();
		   m_covariance= ref.m_covariance.clone();
		   m_InverseCovariance= ref.m_InverseCovariance.clone();
		   m_dLogCovarianceDeterminant = ref.m_dLogCovarianceDeterminant;
		   return *this;
	   }


	CGaussian  (const unsigned int iSize):
		m_iSize (iSize), m_means(iSize,1,0.),m_covariance(iSize,iSize,0.), m_InverseCovariance(iSize,iSize,0.)
	{	SetToNormal(); }


	CGaussian (const Mat_<double> &means, const Mat_<double> &covariance){
		m_means = means.clone();
		m_covariance = covariance.clone();
		m_InverseCovariance = covariance.clone();
	 	m_iSize = means.rows;
		InverseRecalc();
	}


	// inizializzazioni
	bool RandomInit();


	bool ReSize(unsigned int iSize);
	int GetSize() const {
		return m_iSize;}

	bool Set (const Mat_<double> &means, const Mat_<double> &covariance);
	bool SetToNormal();

	// calcolo likelihood con vector<double>
	double GetLogLikelihood(const vector<double> &value, bool bRecalcInverse=true);
	double GetLikelihood(const vector<double> &value, bool bRecalcInverse=true);

	// ricalcola la matrice inversa della covarianza
	bool InverseRecalc(){
		assert (m_InverseCovariance.size[0]==m_covariance.size[0]);
		assert (m_InverseCovariance.size[1]==m_covariance.size[1]);

		double dDetermin;
		dDetermin = determinant(m_covariance);
		assert (dDetermin);
		if (dDetermin!=0){
			m_InverseCovariance = m_covariance.clone();
			m_InverseCovariance=m_InverseCovariance.inv();
			m_dLogCovarianceDeterminant = log(dDetermin);
		}
		double dDetermin2;
		dDetermin2 = determinant(m_InverseCovariance);
		assert(dDetermin2);

		return true;
	}

	// un po' di funzioni di utilità, tipo  salvataggio e lettura su file
	bool SaveToFile(FILE *f);
	bool LoadFromFile(FILE *f);



	// lascio tutto pubblico
public:
	int m_iSize; // tengo per comodità
	Mat_<double> m_means;
	Mat_<double> m_covariance;
	Mat_<double> m_InverseCovariance;
	double m_dLogCovarianceDeterminant;
};



// classe principale per gestire una mixture di gaussiane multidimensionali 
class CGMM_tiny{

	public:

		// costruttore: dimensione dello spazio delle feature M (default=1) e
		// numero iniziale di gaussiane K (default=1)
		// costruttore di default
		CGMM_tiny():
		m_weights(0), m_Gaussians(0)
		{
			m_iK = 0;  // nessuna gaussiana
			m_iM = 1;
			m_bDiagonal = true;
			// aggiungo una gaussiana
			//AddComponents();
		}

		// costruttore di copia
		CGMM_tiny(const CGMM_tiny &ref):
		m_weights(ref.m_weights), m_Gaussians(ref.m_Gaussians), m_iM(ref.m_iM),m_bDiagonal(ref.m_bDiagonal), m_iK(ref.m_iK)
		{
			;
		}
	
		// costruttore: dimensione dello spazio delle feature M (default=1) e
		// numero iniziale di gaussiane K (default=1)
		CGMM_tiny(unsigned int iM,unsigned int iK):
		m_weights(0), m_Gaussians(0)
		{
			m_iK = 0;
			m_iM = iM;
			m_bDiagonal = true;
			// aggiungo le gaussiane iniziali
			for (unsigned int i=0; i<iK; i++)
				AddComponents();
		}


		CGMM_tiny::~CGMM_tiny(){
			while (m_iK)
				RemoveComponent();
			}


		


		// TODO: devo fare costruttore di copia e assegnamento



		void ForceDiagonalCovariance(bool bForce){
			m_bDiagonal=bForce;
			// todo: azzero gli altri termini
		}


		

		// cambio delle dimensioni
		bool ReSize(unsigned int iM);
		bool SetGaussiansNumber(unsigned int iK);

		// inizializzazioni
		bool RandomInit();

		// lettura delle dimensioni
		unsigned int GetSize();
		unsigned int GetGaussiansNumber();

		// aggiunta di una gaussiana
		bool AddComponents(int n=1);
		bool AddNewComponent(const Mat_<double> &mean, const Mat_<double> &variance);

		// rimozione di una componente
		bool RemoveComponent(unsigned int iIndex=-1); //-1 = ultima

		// rimozione di tutte le componenti
		bool RemoveAllComponents(); 

		

		// settaggio dei pesi
		bool SetWeights(const vector<double>& weights);

		// lettura dei pesi
		bool GetWeights(vector<double>& weights);


		CGaussian & GetGaussian(int n){
			return m_Gaussians[n];
		}

		// -----------------------------------------
		// Funzioni per la likelihood con feature salvate su vector
		// calcolo della likelihood di un valore
		double GetLikelihood (const vector<double> &value, bool bRecalc=true);

		// calcolo della likelihood di un valore ristretta ad una sola gaussiana
		double GetLikelihood_partial (const vector<double> &value, int iIndex, bool bRecalc=true);

		// calcolo della likelihood di un valore
		double GetLogLikelihood (const vector<double> &value, bool bRecalc=true);

		// calcolo della likelihood di un valore ristretta ad una sola gaussiana
		double GetLogLikelihood_partial (const vector<double> &value, int iIndex, bool bRecalc=true);
		// ------------------------------------------

			


		// un po' di funzioni di utilità, tipo  salvataggio e lettura su file
		bool SaveToFile(FILE *f);
		bool LoadFromFile(FILE *f);

		// accesso al singolo elemento
		// accesso alla j-esima cella del vettore delle medie della i-esima gaussiana
		double& MeanValue(int iGaussianIndex, int jIndex);
		// accesso al (j,k)-esima cella della matrice di covarianza della i-esima gaussiana
		double& CoVarianceValue(int iGaussianIndex, int jIndex, int kIndex);
		// accesso al peso della i-esima gaussiana
		double& WeightValue(int iGaussianIndex);


//	protected:

		unsigned int m_iM; // feature space size
		unsigned int m_iK; // number of gaussians

		vector<CGaussian> m_Gaussians; // array of gaussian components
		vector<double> m_weights; // Vector of weights

		bool m_bDiagonal;
	
};


//-----------------------------------
// FUNZIONI DI UTILITA' PER GAUSSIANE
// erf e erfc

/***************************
*   erf.cpp
*   author:  Steve Strand
*   written: 29-Jan-04
***************************/


double erfc(double x);
double erf(double x);

double gauss(double mean, double variance, double x);
double cumGauss(double mean, double variance, double x);



// TEMPLATE STRUCT plus
template<class _Ty>
	struct vecsum
		: public binary_function<_Ty, _Ty, _Ty>
	{	// functor for operator+
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator+ to operands
			_Ty somma;
			transform (_Left.begin(),_Left.end(),_Right.begin(),back_inserter(somma),plus<double>());
		return somma;
		}
	};

	
// TEMPLATE STRUCT plus
template<class _Ty>
	struct vecsumquad
		: public binary_function<_Ty, _Ty, _Ty>
	{	// functor for operator+
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator+ to operands
			
			_Ty somma;
			_Ty quad;
			transform (_Right.begin(),_Right.end(),_Right.begin(),back_inserter(quad),multiplies<double>());
			transform (_Left.begin(),_Left.end(),quad.begin(),back_inserter(somma),plus<double>());
		return (somma);
		}
	};

	
	} // namespace
