/*
 * FFT.c
 *
 *  Created on: Nov 17, 2019
 *      Author: Brian Card
 */
#include "math.h"
#include "complex.h"
#include "FFT.h"

#define PI 3.1415926535

//Must use powers of 2
int logbase2(int sample)
{
    int value = 0;
    while (sample > 1)
    {
        sample = sample >> 1;
        value++;
    }

    return value;
}
//Must use powers of 2
int expbase2(int sample)
{
    int value = 1 << sample;
    return value;
}



float Wfactor(int k, int n, int N)
{
    return (sinf((2*PI*k*n)/N)*sinf((2*PI*k*n)/N))+(cosf((2*PI*k*n)/N)*cosf((2*PI*k*n)/N));
}

float cosfactor(int k, int n, int N)
{
    return cosf((2*PI*k*n)/N);
}

float sinfactor(int k, int n, int N)
{
    return sinf((2*PI*k*n)/N);
}

////////////////Bit Reverse Functions///////////////////////
void reversearray(COMPLEX *x,int* masks,int N)
{
    COMPLEX temp;
    int maskexp = logbase2(N);
//    int masks[N];
    int reverse= 0;
    int count1 = 0;
    int count2 = 0;
    for (count1 = 0; count1 < maskexp; count1++)
    {
        masks[count1] = 1<<((maskexp - 1) - count1);
    }
    for (count1 = 0; count1 < N; count1++)
    {
        for (count2 = 0; count2 < maskexp; count2++)
        {
            if (count1 & masks[count2])
            {
                reverse += 1<<(count2);
            }
        }
        if(reverse < count1)
        {
            reverse = 0;
        }
        else
        {
            temp = x[reverse];
            x[reverse] = x[count1];
            x[count1] = temp;
            reverse = 0;
        }
    }
}
////////////////Twiddle Factor function///////////////////////
void twiddle(COMPLEX *t, int n)
{
    int i;
    for(i =0; i<n/2; i++)
    {
        t[i].real = cosf(2*PI*i/n);
        t[i+n/2].real = -t[i].real;

        t[i].imag = -sinf(2*PI*i/n);
        t[i+n/2].imag = -t[i].imag;
    }
}
////////////////FFT function///////////////////////
void fftN(COMPLEX *x,COMPLEX *t, unsigned int s,unsigned int j)
{
    COMPLEX temp;
    temp = x[s];

    x[s].real = x[0].real - temp.real*t[j].real + temp.imag*t[j].imag;
    x[s].imag = x[0].imag - temp.real*t[j].imag - temp.imag*t[j].real;

    x[0].real = x[0].real + temp.real*t[j].real - temp.imag*t[j].imag;
    x[0].imag = x[0].imag + temp.real*t[j].imag + temp.imag*t[j].real;
}
void calcfftN(COMPLEX *x, COMPLEX *t, int N)
{
    float gain = (float)2/N;
    int stage_count,jump_count,i;
    int stage_max = logbase2(N);
    for(stage_count = 1; stage_count <= stage_max; stage_count++)
    {
        int jump_space = 1<<stage_count;
        int stride = jump_space>>1;
        int jump_max = 1<<(stage_max-stage_count);

        for(jump_count=0; jump_count<jump_max; jump_count++)
        {
            for(i=0; i<stride; i++)
            {
                fftN(&x[i+jump_count*jump_space], &t[0],stride,i*jump_max);
                if(stage_count == stage_max)
                {
                    x[i].real = gain*sqrtf(x[i].real*x[i].real+x[i].imag*x[i].imag);
                    x[i+stride].real = gain*sqrtf(x[i+stride].real*x[i+stride].real+x[i+stride].imag*x[i+stride].imag);
                    x[i].imag = 0;
                    x[i+stride].imag = 0;
                }
            }
        }
    }
}
