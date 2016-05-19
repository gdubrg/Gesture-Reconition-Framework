//------------------------------------------------------------------
//
// Nome file: gmm_tiny.cpp
// Contenuto: Gaussian Mixture Model (tiny version rifatta da me) 
// Data Creazione: 01/10/08
// Autore: Roberto Vezzani	
//
//------------------------------------------------------------------



#include "gmmstd_gmm_tiny.h"
#include <time.h>

namespace gmmstd{

double rand_BoxMuller(){
    static int iset = 0;
    static double gset;
    double v1, v2, fac, rsq;
    if(iset == 0){

        /* Pick a point in the unit circle */
        do{
            v1 = 2.0*fabs(((double)rand() / RAND_MAX) )    -1.0;
            v2 = 2.0*fabs(((double)rand() / RAND_MAX) )    -1.0;

            rsq = v1*v1 + v2*v2;
        } while (rsq >= 1.0 || rsq == 0);

        /* Perform the Box-Muller transformation */
        fac = sqrt(-2*log(rsq)/rsq);
        gset = v1*fac;
        iset = 1;
        return v2*fac;
    }
    else {
        /* Return the deviate that was not returned last time */
        iset = 0;
        return gset;
    }
}

double random_Uniform(double lowest, double highest) {
	//srand((unsigned)time(0));   
	//inizializzo il seme solo la prima volta
	static bool bInitialized=false;
	if (!bInitialized){
                //srand(GetTickCount());
                    srand((unsigned)time(0));
                //srand(QTime::currentTime().msec());
		bInitialized = true;
	}
	return rand()/(double)RAND_MAX*(highest-lowest)+lowest;
}


// ---------------------------------------------------------

	

	// random initialization
	bool CGaussian::RandomInit(){
		// media random tra -1 e 1
		// covarianze: matrice di numeri random tra 1 e 2
		unsigned int i;
		m_covariance = Mat_<double>::eye(m_iSize, m_iSize);

		for (i=0; i<m_iSize; i++){
			m_means(i,0)=random_Uniform(-1,1);
			m_covariance(i,i)=random_Uniform(1,2);
		}
		return true;
	}



#define LOG2PI 1.83787

	// calcolo likelihood 
	double CGaussian::GetLogLikelihood(const vector<double> &value, bool bRecalcInverse){
		// inversa della matrice di covarianza (solo se richiesto)
	//	cout << m_covariance(0,0) << " <Cov " << endl;
	//	cout << m_InverseCovariance(0,0) << " <invCov " << endl;

		if (bRecalcInverse) 
			InverseRecalc();

		

		double dConst;
	
		dConst = -(((double)m_iSize/2.0) * LOG2PI) - 0.5 * m_dLogCovarianceDeterminant;

		//calcolo prodotto (x-m)' Sigma^-1 (x-m)
		double dValue=0;
		unsigned int i,j;
		for (i=0; i<m_iSize;i++){
			double dInnerValue=0;
			for (j=0; j<m_iSize;j++)
			{
				//cout  << m_InverseCovariance(i,j) << " " << value[j] << " " << m_means(j,0) << endl;
				dInnerValue+= m_InverseCovariance(i,j)*(value[j]-m_means(j,0));
				//cout << dInnerValue << endl;

			}

			dValue += dInnerValue * (value[i]-m_means(i,0));
			//cout << dValue << endl;
		}

		//cout << dValue << endl;

		// loglikelihood finale
		dValue = dConst - 0.5 * dValue;


		return dValue;
	}


	double CGaussian::GetLikelihood(const vector<double> &value, bool bRecalcInverse){

		// inversa della matrice di covarianza (solo se richiesto)
		if (bRecalcInverse) 
			InverseRecalc();

		double dConst;
		dConst = -(((double)m_iSize/2.0) * LOG2PI) - 0.5 * m_dLogCovarianceDeterminant;

		//calcolo prodotto (x-m)' Sigma^-1 (x-m)
		double dValue=0;
		unsigned int i,j;
		for (i=0; i<m_iSize;i++){
			double dInnerValue=0;
			for (j=0; j<m_iSize;j++)
			{
				//cout  << m_InverseCovariance(i,j) << " " << value[j] << " " << m_means(j,0) << endl;
				dInnerValue+= m_InverseCovariance(i,j)*(value[j]-m_means(j,0));
				//cout << dInnerValue << endl;

			}

			dValue += dInnerValue * (value[i]-m_means(i,0));
			//cout << dValue << endl;
		}

		//cout << dValue << endl;

		// loglikelihood finale
		dValue = dConst - 0.5 * dValue;

		// soglia per evitare valori piccoli underflow?!?
		/*if (dValue <= THLOG) 
			return 0;
			*/

		return exp(dValue);
	}


	


	bool CGaussian::ReSize(unsigned int iSize){
		// cambio le dimensioni dei vettori
		m_iSize = iSize;
		m_means.create(iSize,1);
		m_covariance.create(iSize,iSize);
		// metto tutte le componenti come normali
		SetToNormal();

		return true;

	}

	bool CGaussian::Set (const Mat_<double> &means, const Mat_<double> &covariance){
		m_means = means.clone();
		m_covariance = covariance.clone();
		m_iSize =means.rows;
		InverseRecalc();
		return true;
	}


	bool CGaussian::SetToNormal(){
		//m_means.create(m_iSize,1);
		m_means = Mat_<double>::zeros(m_iSize,1);
		m_covariance.create(m_iSize,m_iSize);
		m_covariance = Mat_<double>::eye(m_iSize,m_iSize);
		m_InverseCovariance.create(m_iSize,m_iSize);
		m_InverseCovariance= Mat_<double>::eye(m_iSize,m_iSize);
		m_dLogCovarianceDeterminant=0;
		return true;
	}

		// un po' di funzioni di utilità, tipo  salvataggio e lettura su file
	bool CGaussian::SaveToFile(FILE *f){
		
			// numeri vari
			fwrite(&m_iSize,sizeof(unsigned int),1,f);
			int i,j;
			for (i=0; i<m_iSize; i++)
				fwrite(&m_means(i,0),sizeof(double),1,f);
			for (i=0; i<m_iSize; i++)
				for (j=0; j<m_iSize; j++)
					fwrite(&m_covariance(i,j),sizeof(double),1,f);
			for (i=0; i<m_iSize; i++)
				for (j=0; j<m_iSize; j++)
					fwrite(&m_InverseCovariance(i,j),sizeof(double),1,f);
			fwrite(&m_dLogCovarianceDeterminant,sizeof(double),1,f);
		return true;
	}


	bool CGaussian::LoadFromFile(FILE *f){
		
			// numeri vari
			fread(&m_iSize,sizeof(unsigned int),1,f);
			int i,j;
			// resize
			ReSize(m_iSize);
			// lettura valori
			for (i=0; i<m_iSize; i++)
				fread(&m_means(i,0),sizeof(double),1,f);
			for (i=0; i<m_iSize; i++)
				for (j=0; j<m_iSize; j++)
					fread(&m_covariance(i,j),sizeof(double),1,f);
			for (i=0; i<m_iSize; i++)
				for (j=0; j<m_iSize; j++)
					fread(&m_InverseCovariance(i,j),sizeof(double),1,f);
						
			fread(&m_dLogCovarianceDeterminant,sizeof(double),1,f);

		return true;
	}


		
	// --------------------------------------------------------------------------------

		

		// rimozione
		bool CGMM_tiny::RemoveComponent(unsigned int iIndex){ //-1 = ultima
			if (m_iK<=0)
				return false;
			
			if (iIndex==-1 || iIndex>=m_iK)
			{
				m_Gaussians.pop_back();
			}
			else
			{				
				m_Gaussians.erase (m_Gaussians.begin()+iIndex);
				
			}
			
			m_iK--;
			return true;
		}
	
		bool CGMM_tiny::RemoveAllComponents(){
			while (RemoveComponent());
			return true;
		} 



		// cambio delle dimensioni
		bool CGMM_tiny::ReSize(unsigned int iM){
			m_iM = iM;
			// risetto le dimensioni delle gaussiane, se ne esistono
			for (unsigned int i=0; i<m_iK; i++)
				m_Gaussians[i].ReSize(iM); // resize di ogni gaussiana
			return true;
		}


		bool CGMM_tiny::SetGaussiansNumber(unsigned int iK){
			 while (m_iK>iK)
				 RemoveComponent();
			 while (m_iK<iK)
				 AddComponents();
			 return true;
		}


		// lettura delle dimensioni
		unsigned int CGMM_tiny::GetSize(){
			return m_iM;
		}

		unsigned int CGMM_tiny::GetGaussiansNumber(){
			return m_iK;
		}


		// -----------------------------------------------------------
		// aggiunta di una gaussiana
		bool CGMM_tiny::AddComponents(int n){
			// creo una gaussiana della dimensione giusta
			for (int i=0; i<n; i++)
				{
					CGaussian Gauss (m_iM);
					// la aggiungo
					m_Gaussians.push_back(Gauss);
					// sistemo il numero di gaussiane
					m_iK ++;
				}

			// TODO: normalizzare i pesi ?!?!
			m_weights.resize(m_iK);

			fill(m_weights.begin(),m_weights.end(),1.0/m_iK);


			return true;

		}

		bool CGMM_tiny::AddNewComponent(const Mat_<double> &mean, const Mat_<double> &covariance){
		// creo una gaussiana della dimensione giusta
			CGaussian Gauss (mean,covariance);
			// la aggiungo
			m_Gaussians.push_back(Gauss);
			// sistemo il numero
			m_iK++;

			// TODO: normalizzare i pesi ?!?!
			m_weights.resize(m_iK);
		
			fill(m_weights.begin(),m_weights.end(),1.0/m_iK);

			
			return true;
		}

		// settaggio dei pesi
		bool CGMM_tiny::SetWeights(const vector<double>& weights){
			int i;
			m_weights.resize(m_iK);
			for (i=0; i<m_iK; i++){
				m_weights[i] = weights[i];
				//cout << m_weights[i] << endl;
			}

			return true;
		}

		// lettura dei pesi
		bool CGMM_tiny::GetWeights(vector<double>& weights){
			weights = m_weights;
			return true;
		}



		// -----------------------------------------
		// Funzioni per la likelihood con feature salvate su vector

		// calcolo della likelihood di un valore
		double CGMM_tiny::GetLogLikelihood (const vector<double> &value,bool bRecalc){
			double dValue=0;
			unsigned int i;
			

			// shortcut for one subclass
			if(m_iK==1) {
				dValue = GetLogLikelihood_partial(value,0,bRecalc);
	 		}
			// compute mixture likelihood 
			else 
			{
				vector<double> vdLL (m_iK);

				double dBestLL;
				int iBestLL;
				for (i=0; i<m_iK; i++)
					vdLL[i] = GetLogLikelihood_partial(value,i,bRecalc);

				//cout <<m_iK <<endl;
				//cout <<m_weights[0] <<endl;

				dBestLL = THLOG;
				iBestLL=-1;

				for (i=0; i<m_iK; i++){
					//if ( m_weights[i] * exp(vdLL[i]) > m_weights[iBestLL] * exp(dBestLL)){
					if ( vdLL[i] > dBestLL && m_weights[i] >0){  // gaussiana con peso non nullo
						iBestLL = i;
						dBestLL=vdLL[i];
					}
				}
				// Sum weighted subclass likelihoods 
				/* RV CORRETTA:
				dValue = 0;
				for(i=0; i< m_iK; i++)
					dValue+= exp(vdLL[i]-dBestLL) * (m_weights[i]);
					*/
				dValue = 0;
				if (iBestLL == -1) // nessuna gaussiana buona trovata -> ritorno un loglikelihood basso basso
					return THLOG;
				

				for(i=0; i< m_iK; i++){
					double dDiff;
					dDiff= vdLL[i] -dBestLL;
					if (dDiff > THLOG){
						dValue+= exp(dDiff) * (m_weights[i]/m_weights[iBestLL]);
					}
				}

				dValue = log(dValue) + dBestLL + log(m_weights[iBestLL]);
			}

			if (dValue <THLOG)
				return THLOG;
			return dValue;
      }


		// calcolo della likelihood di un valore ristretta ad una sola gaussiana
		double CGMM_tiny::GetLogLikelihood_partial (const vector<double> &value, int iIndex,bool bRecalc){
			double dVal;
			dVal = m_Gaussians[iIndex].GetLogLikelihood(value,bRecalc);
			if (dVal <THLOG) // per rendere il conto compatibile con quello complessivo
				return THLOG;
			return dVal;
		}

	
		// forse non ha senso usare la log per poi tornare alla non log
		/*
		double CGMM_tiny::GetLikelihood (const vector<double> &value, bool bRecalc){
			if (m_iK==0) return 0;
			double dLog = GetLogLikelihood(value,bRecalc);
			if (dLog<THLOG)
				return 0;
			if (dLog>-(THLOG))
				return 999999;
			return exp(dLog);
		}
		*/
		double CGMM_tiny::GetLikelihood (const vector<double> &value, bool bRecalc){
			if (m_iK==0) return 0;
			double dVal=0;
			for (int i=0; i<m_iK; i++)
				dVal += m_weights[i] * GetLikelihood_partial(value,i,bRecalc);
			return dVal;
		}


		// calcolo della likelihood di un valore ristretto ad una sola gaussiana
		/*double CGMM_tiny::GetLikelihood_partial (const vector<double> &value, int iIndex, bool bRecalc){
			return exp(GetLogLikelihood_partial(value,iIndex, bRecalc));
		}
		*/
		double CGMM_tiny::GetLikelihood_partial (const vector<double> &value, int iIndex, bool bRecalc){
			double dVal;
			dVal = m_Gaussians[iIndex].GetLikelihood(value,bRecalc);
			return dVal;
		}






		// un po' di funzioni di utilità, tipo  salvataggio e lettura su file
		bool CGMM_tiny::SaveToFile(FILE *f){
			
			// numeri vari
			fwrite(&m_iK,sizeof(unsigned int),1,f);
			fwrite(&m_iM,sizeof(unsigned int),1,f);
			//pesi e gaussiane
			unsigned int i;
			for (i=0; i<m_iK; i++)
				fwrite(&m_weights[i],sizeof(double),1,f);
			for (i=0; i<m_iK; i++)
				m_Gaussians[i].SaveToFile(f);
			return true;
		}

		bool CGMM_tiny::LoadFromFile(FILE *f){


			// uccido le gaussiane attuali
			RemoveAllComponents();
			
			// leggo i vari numeri
			unsigned int iK,iM;
			fread(&iK,sizeof(unsigned int),1,f);
			fread(&iM,sizeof(unsigned int),1,f);


			ReSize(iM);
			SetGaussiansNumber(iK);

			//pesi e gaussiane
			unsigned int i;
			for (i=0; i<iK; i++)
				fread(&m_weights[i],sizeof(double),1,f);
			for (i=0; i<iK; i++)
				m_Gaussians[i].LoadFromFile(f);
			return true;
		}



		// accesso al singolo elemento
		// accesso alla j-esima cella del vettore delle medie della i-esima gaussiana
		double& CGMM_tiny::MeanValue(int iGaussianIndex, int jIndex){
			return m_Gaussians[iGaussianIndex].m_means(jIndex,0);
		}

		// accesso al (j,k)-esima cella della matrice di covarianza della i-esima gaussiana
		double& CGMM_tiny::CoVarianceValue(int iGaussianIndex, int jIndex, int kIndex){
			return m_Gaussians[iGaussianIndex].m_covariance(jIndex,kIndex);
		}

		// accesso al peso della i-esima gaussiana
		double& CGMM_tiny::WeightValue(int iGaussianIndex){
			return m_weights[iGaussianIndex];
		}


			// random initialization
	bool CGMM_tiny::RandomInit(){

		// pesi iniziali: uniformi
		m_weights.resize(m_iK, 1.0/m_iK);

		// gaussiane: random
		unsigned int i;
		for (i=0; i<m_iK; i++)
			m_Gaussians[i].RandomInit();
		return true;
	}



	// ---------------------------------------------------------
	
static const double rel_error= 1E-12;        //calculate 12 significant figures
//you can adjust rel_error to trade off between accuracy and speed
//but don't ask for > 15 figures (assuming usual 52 bit mantissa in a double)


 double erf(double x)
//erf(x) = 2/sqrt(pi)*integral(exp(-t^2),t,0,x)
//       = 2/sqrt(pi)*[x - x^3/3 + x^5/5*2! - x^7/7*3! + ...]
//       = 1-erfc(x)
{
    static const double two_sqrtpi=  1.128379167095512574;        // 2/sqrt(pi)
    if (fabs(x) > 2.2) {
        return 1.0 - erfc(x);        //use continued fraction when fabs(x) > 2.2
    }
    double sum= x, term= x, xsqr= x*x;
    int j= 1;
    do {
        term*= xsqr/j;
        sum-= term/(2*j+1);
        ++j;
        term*= xsqr/j;
        sum+= term/(2*j+1);
        ++j;
    } while (fabs(term)/sum > rel_error);
    return two_sqrtpi*sum;
}


 double erfc(double x)
//erfc(x) = 2/sqrt(pi)*integral(exp(-t^2),t,x,inf)
//        = exp(-x^2)/sqrt(pi) * [1/x+ (1/2)/x+ (2/2)/x+ (3/2)/x+ (4/2)/x+ ...]
//        = 1-erf(x)
//expression inside [] is a continued fraction so '+' means add to denominator only
{
    static const double one_sqrtpi=  0.564189583547756287;        // 1/sqrt(pi)
    if (fabs(x) < 2.2) {
        return 1.0 - erf(x);        //use series when fabs(x) < 2.2
    }
    if (x<0) {               //continued fraction only valid for x>0
        return 2.0 - erfc(-x);
    }
    double a=1, b=x;                //last two convergent numerators
    double c=x, d=x*x+0.5;          //last two convergent denominators
    double q1, q2= b/d;             //last two convergents (a/c and b/d)
    double n= 1.0, t;
    do {
        t= a*n+b*x;
        a= b;
        b= t;
        t= c*n+d*x;
        c= d;
        d= t;
        n+= 0.5;
        q1= q2;
        q2= b/d;
      } while (fabs(q1-q2)/q2 > rel_error);
    return one_sqrtpi*exp(-x*x)*q2;
}


 double gauss(double mean, double variance, double x){
		double dRis; 
		dRis = exp(-(mean-x)*(mean-x)/ (2.0 * variance));
		dRis= dRis / sqrt(2.0 * 3.14159265358979323846264338327950 * variance); 
		return dRis;
	
	}

 double cumGauss(double mean, double variance, double x){
		double dRis; 

		double dTerm;
		dTerm = (fabs(x- mean)) / sqrt(variance * 2.0);
		dRis = 0.5 * (1.0 + erf(dTerm));
		return dRis;
}


	
	} // namespace