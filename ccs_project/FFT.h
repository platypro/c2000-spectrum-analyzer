/*
 * FFT.h
 *
 *  Created on: Nov 17, 2019
 *      Author: Brian Card
 */
#ifndef FFT_H_
#define FFT_H_

typedef struct
{
    float real;
    float imag;
} COMPLEX;
int logbase2(int sample);

int expbase2(int sample);

float Wfactor(int k, int n, int N);

float cosfactor(int k, int n, int N);

float sinfactor(int k, int n, int N);

void reversearray(COMPLEX *x, int N,int* masks);

void twiddle(COMPLEX *t, int n);

void fftN(COMPLEX *x,COMPLEX *t, unsigned int s,unsigned int j );

void calcfftN(COMPLEX *x, COMPLEX *t, int N);

#endif /* FFT_H_ */
