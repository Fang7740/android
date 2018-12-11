#include <stm32f10x.h>
#include <stdio.h>
#include <math.h>

float b[3] = {0.0009446, 0.0018893, 0.0009446};   //float b[3] = {0.0009446, 0.0018893, 0.0009446};
float a[3] = {1,-1.8226949251963083,0.83718165125602262};
float X[3] ={0};
float Y[3] ={0}; 

float FILTER_LOWPASSED(float value)
{
	X[2]=X[1];
	X[1]=X[0];
	X[0]=value;
	Y[0]=Y[1];
	Y[1]=Y[2];
	Y[2]=(X[0]*b[2]-Y[0]*a[2]+X[1]*b[1]-Y[1]*a[1]+X[2]*b[0])/a[0];
	
	return Y[2];
}