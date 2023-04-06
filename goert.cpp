#include "mex.h"
#include <iostream>
#include <complex>   
#include <math.h>

using namespace std; 

double goert(double *x, double fs, double N, double ft){
	double k;
	double omega,vk1=0,vk2=0,vk;
	std::complex<double> ii(0,-1);
	std::complex<double> wk;
	double yk;
	k=round((N*ft)/fs);
	omega=2*M_PI*k/N;
		
	for(int i=0;i<N;i++){
		vk=x[i]+2*vk1*cos(omega)-vk2;
		vk2=vk1;
		vk1=vk;
	}

	wk=-exp(ii*omega);
	yk = norm(vk1-wk*vk2);
	yk=yk/N;
	return yk;
}




void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]){
	double fs=12000;	
	double N=1024;
	double ft[8]={697,770,852,941,1209,1336,1477,1633};
	double *x,*y;
	if(nrhs!=1) {
		mexErrMsgIdAndTxt("goert:nrhs","Se requieren dos entradas.");
	}
	/*Verifica que haya un argumento de salida*/
	if(nlhs!=1) {
		mexErrMsgIdAndTxt("goert:nlhs","Se requiere una salida");
	}	
	
	x=mxGetPr(prhs[0]);
	plhs[0] = mxCreateDoubleMatrix(8,1,mxREAL);
	y = mxGetPr(plhs[0]);

	for (int i=0;i<8;i++){
		y[i]=goert(x,fs,N,ft[i]);
	}

}
