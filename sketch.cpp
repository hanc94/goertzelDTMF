//Archivos de cabecera
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "simdsp.h"
#include <complex>
//Frecuencia de muestreo
#define FS  12000
//Tamaño del bloque de datos
#define N_DATOS 2048
//Para almacenar el resultado de la goertzel
double Yr[N_DATOS];
//Umbral a partir del cual se detecta un tono (fracción de la energía de la señal de entrada)
#define THRESHOLD_ON 0.035

using namespace std;

std::complex<double> goert(double *x,double ft){
	double k;
	double omega,vk1=0,vk2=0,vk;
	std::complex<double> ii(0,-1);
	std::complex<double> wk;
	std::complex<double> yk;
	k=round(((double)N_DATOS*ft)/(double)FS);
	omega=2*M_PI*k/(double)N_DATOS;
	for(int i=0;i<N_DATOS;i++){
		vk=x[i]+2*vk1*cos(omega)-vk2;
		vk2=vk1;
		vk1=vk;
	}

	wk=-exp(ii*omega);
	yk = vk1-wk*vk2;
	return yk;
}



/*
   This computes an in-place complex-to-complex FFT 
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform 
*/




//Rutina que simulará la ISR que se invoca una vez ha finalizado
//la transferencia por DMA, audio apunta a los datos que se procesarán
void dma_callbackfnc(short *audio){

	//Acumuladores para cálculo de energía	
	double e_in,en[8];
	char simbols[16]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	double ft[8]={697,770,852,941,1209,1336,1477,1633};
	std::complex<double> y[8];
	int s=-1,aux=0;
	//Produce un nuevo llamado a la rutina para capturar datos por DMA.
        //Debería hacerse antes de empezar a procesar para asegurar la operación en tiempo real
	captureBlock(dma_callbackfnc );

         //Calcula energía de la señal de entrada y copia señal a vector de doubles
	e_in=0.0;       
        for(int n=0;n<1024;n++){
	    Yr[n]=(double)audio[n];
	    e_in+=(double)audio[n]*(double)audio[n];
	}

	for (int i=0;i<8;i++){
		y[i]=goert(Yr,ft[i]);
		en[i] = norm(y[i]);
		en[i] = en[i]/(double)N_DATOS;
	}
	
	
	for(int i=0;i<4;i++){
		for(int j=4;j<8;j++){
			s++;
			if((en[i]>=(THRESHOLD_ON*e_in))&&(en[j]>=(THRESHOLD_ON*e_in))){
				aux=1;
				goto double_break;
			}			
		}
	}
	double_break:
	if(aux==1)
		cout<<simbols[s]<<endl;
}

//Función de configuración inicial
void dsp_setup(){

	enableAudio(N_DATOS,FS);	

	//Produce un llamado a la captura de datos por DMA. Esta función invoca
	//la función callbackfnc una vez la captura de un buffer ha finalizado
	captureBlock(dma_callbackfnc );

	cout<<"DTMF"<<endl;

}

void dsp_loop(){
	//El bucle principal en este caso no hace nada, pues todo el
	//procesamiento se hace vía DMA
}
