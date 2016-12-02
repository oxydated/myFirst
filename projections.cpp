//
//  projections.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include<math.h>
#include "projections.h"

static float tm[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

static float le = 0.0;
static float ri = 0.0;
static float n = 0.0;
static float f = 0.0;
static float b = 0.0;
static float t = 0.0;

static float tx = 0.0;
static float ty = 0.0;

void perspectiveMatrix( float in_l, float in_r, float in_n, float in_f, float in_b, float in_t, float *m, float *res ){
	le = in_l;
	ri = in_r;
	n = in_n;
	f = in_f;
	b = in_b;
	t = in_t;

	//Already in Column Major Order. Should not be transposed.
	
	tm[0]= 	2*n/(ri-le);    tm[4]= 	0.0;            tm[8]=  (ri+le)/(ri-le);    tm[12]= 	0.0;
	tm[1]= 	0.0;            tm[5]= 	2*n/(t-b);      tm[9]=  (t+b)/(t-b);        tm[13]= 	0.0;		
	tm[2]= 	0.0;            tm[6]= 	0.0;            tm[10]= (-f-n)/(f-n);		tm[14]= 	-2*(f*n)/(f-n);		
	tm[3]= 	0.0;            tm[7]= 	0.0;            tm[11]= -1.0;               tm[15]= 	0.0;
    
    //multiplyMatrices( tm, m, res );   
	
    multiplyMatrices( m, tm, res );   
}

void windowCoordToCameraSpace( float xw, float yw, float z, float *pointInCamSpace) {
	float xSpace = ((le*(-1 + xw) - ri*(1 + xw))*z) / (2.0*n);
	float ySpace = ((b*(-1 + yw) - t*(1 + yw))*z) / (2.0*n);
	pointInCamSpace[0] = xSpace;
	pointInCamSpace[1] = ySpace;
	pointInCamSpace[2] = z;
	pointInCamSpace[3] = 1.0;
}

void invertedPerspectiveMatrix(float l, float r, float n, float f, float b, float t, float *m, float *res) {
	tm[0] = 0. + (-0.5*l + 0.5*r) / n;      tm[1] = 0.;								tm[2] = 0.;      tm[3] = 0.;
	tm[4] = 0.;								tm[5] = 0. + (-0.5*b + 0.5*t) / n;      tm[6] = 0.;      tm[7] = 0.;
	tm[8] = 0.;								tm[9] = 0.;								tm[10] = 0.;     tm[11] = 0. + 0.5 / f - 0.5 / n;

	tm[12] = (-0.125*(f - 1.*n)*(l - 1.*r)*(0. - (4 * f*pow(n, 2)*(l + r)) / ((f - n)*(l - r)*(b - t)))*(b - 1.*t)) / (f*pow(n, 3));
	tm[13] = (-0.125*(f - 1.*n)*(l - 1.*r)*(b - 1.*t)*(0. - (2 * f*n*(0. + (2 * n*(b + t)) / ((l - r)*(b - t)))) / (f - n))) / (f*pow(n, 3));
	tm[14] = -1.;
	tm[15] = (-0.125*(f - 1.*n)*(l - 1.*r)*(0. - (4 * f*pow(n, 2)) / ((f - n)*(l - r)*(b - t)) - (4 * pow(n, 3)) / ((f - n)*(l - r)*(b - t)))*(b - 1.*t)) / (f*pow(n, 3));

	multiplyMatrices(m, tm, res);
}

void viewportMatrix( float x0, float y0, float w, float h, float ne, float fa,  float *m, float *res ){
	tx = float(w);
	ty = float(h);

 //   tm[0]= 	1.0;			tm[4]= 	0.0;            tm[8]=      0.0;        tm[12]= 	0;		
 //   tm[1]= 	0.0;            tm[5]=	w/h;			tm[9]=      0.0;        tm[13]= 	0;		
	//tm[2] = 0.0;            tm[6] = 0.0;            tm[10] =(fa - ne) / 2.0;		tm[14] = 0.0;
 //   tm[3]= 	0.0;            tm[7]= 	0.0;            tm[11]= (fa + ne) / 2.0;		tm[15]= 	1.0;

	tm[0] = 1.0;			tm[4] = 0.0;            tm[8] = 0.0;        tm[12] = 0;
	tm[1] = 0.0;            tm[5] = w / h;			tm[9] = 0.0;        tm[13] = 0;
	tm[2] = 0.0;            tm[6] = 0.0;            tm[10] = 1.0;		tm[14] = 0.0;
	tm[3] = 0.0;            tm[7] = 0.0;            tm[11] = 0.0;		tm[15] = 1.0;
    
    //multiplyMatrices( tm, m, res );   
	
    multiplyMatrices( m, tm, res );   
}

void normalizedScreenCoordFromWindowCoord(float sx, float sy, float &x, float &y) {
	//x = (2.0*(sx / tx) - 1.0)/2.0;
	//x = (tx/ty)*(-2 * (tx / 2.0 - sx)) / tx;
	//y = ((2.0*((ty - sy) / ty) - 1.0)*(ty / tx))/2.0;
	//y = (2 * (-sy + ty / 2.)) / ty;
	float vx = -1 + (2 * sx) / tx;
	float vy = -1 + (2 * (ty - sy)) / ty;
	x = vx;
	y = (ty * vy) / tx;
}
