#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "floatInt.h"

float floatSum(float a,float b);
void floatPrint(char * pref,float a);

int main(int argc,char **argv) {
	if (argc!=3) {
		printf("Please invoke as %s <a> <b>\n",argv[0]);
		printf("   where <a> and <b> are floating point numbers.\n");
		return 1;
	}
	float a=atof(argv[1]);
	floatPrint("a             :",a);
	float b=atof(argv[2]);
	floatPrint("b             :",b);
	floatPrint("floatSum(a,b) :",floatSum(a,b));
	floatPrint("a + b         :",a+b);
	return 0;
}

float floatSum(float a,float b) {
	int expa = float2Exp(a);
	int expb = float2Exp(b);
	int exps;
	int fraca =float2Frac(a);
	int fracb =float2Frac(b);
	int fracs;
	int signa = float2Sign(a);
	int signb = float2Sign(b);
	int signs;
	if (expa > expb){
	   exps = expa;
	}
	if (expa < expb){
		 exps = expb;
	}
	fracs = ((int)(pow(-1,signa) * fraca) >> (exps - expa)) + ((int)(pow(-1,signb) * fracb) >> (exps - expb));
	if (fracs < 0){
		signs = 1;
		fracs = -1 * fracs;
	}
	else {
		signs = 0;
	}
	if (fracs <= 1){
		fracs = fracs << 1;
		exps = exps - 1 ;
	}
	if (fracs > 2){
		fracs= fracs >> 1;
		exps = 1+ exps;
	}
	if (fracs == 0){
		return 0;
	}
	if (exps > 126){
		signs = 127;
		fracs = 0;
	}
	if (exps < -126){
		return 0;
	}
	/*if (-1*isinf(a) && (float)(b)){
		return -INFINITY;
	}
	if (isinf(a) && (float)(b)){
		return INFINITY;
	}*/
	if (((a==0 && b ==0) || (a ==0 && b == -0)) || (a==0 && b== -0) || (isinf(a) && -1*isinf(b)) || (isinf(b) && -1*isinf(a))){
		return 0;
	}
	if (a == -1*0 && b== -1*0){
		return -1*0;
	}
	if ((isinf(a) && b==0) || (isinf(a) && b == -1*0) || (a ==0 && isinf(b)) || (a== -1*0 && isinf(b) ) || (isinf(a) && isinf(b)) || ((float)(a) && isinf(b))){
		return INFINITY;
	}
	if ((-1 * isinf(a) && b ==0) || (isinf(a) && b == -0) || (a==0 && -1*isinf(b)) || (a== -1*0 && -1*isinf(b)) || (-1*isinf(a) && -1*isinf(b)) || ((float)(a) && -1*isinf(b))){
		return -INFINITY;
	}
	if (isnan(a) || isnan(b)){
		return NAN;
	}
	if (((float)(a) && b == 0) || ((float)(a) && b == -1*0)){
		return a;
	}
	if (((float)(b) && a==0) || ((float)(b) && a== -1*0)){
		return b;
	}
	if ((float)(a) && (float)(b)){
		return a+b;
	}
	if (isinf(a)){
		return INFINITY;
	}
	if (-1*isinf(a)){
		return -INFINITY;
	}
	return makeFloat(signs, exps, fracs);
}

void floatPrint(char * pref,float a) {
	int norm=isNormal(a);
	int frac=float2Frac(a);
	int exp=float2Exp(a);
	if (isNormal(a)) {
		frac &= ~(1<<23); // Turn of 1. for print
	} else exp=-126;
	printf("%s %g = -1^%d x %d.%06x x 2^%d\n",pref,a,
		float2Sign(a),norm,frac,exp);
}
