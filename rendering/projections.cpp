//
//  projections.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "../stdafx.h"
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

void getPerspectiveParameters(float &out_l, float &out_r, float &out_n, float &out_f, float &out_b, float &out_t) {
	out_l = le;
	out_r = ri;
	out_n = n;
	out_f = f;
	out_b = b;
	out_t = t;
}

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
	
    oxyde::linAlg::multiplyMatrices( m, tm, res );   
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

	oxyde::linAlg::multiplyMatrices(m, tm, res);
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
	
	oxyde::linAlg::multiplyMatrices( m, tm, res );
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

void intersectViewRayToSphere( float *camPos, float *targetPos, float *pointInSpace, float *intersection ) {

	float ix = NAN;
	float iy = NAN;
	float iz = NAN;

	float px = camPos[0];
	float py = camPos[1];
	float pz = camPos[2];

	float cx = targetPos[0];
	float cy = targetPos[1];
	float cz = targetPos[2];

	float qx = pointInSpace[0];
	float qy = pointInSpace[1];
	float qz = pointInSpace[2];

	float d = sqrt(pow(cx - px, 2) + pow(cy - py, 2) + pow(cz - pz, 2));
	float radius = (d*t) / sqrt(pow(n, 2) + pow(t, 2));

	float nNorm = sqrt(pow(-px + qx, 2) + pow(-py + qy, 2) + pow(-pz + qz, 2));
	float nx = (-px + qx) / nNorm;
	float ny = (-py + qy) / nNorm;
	float nz = (-pz + qz) / nNorm;

	float delta = 4 * pow(-(cx*nx) + cy*ny + cz*nz + nx*px - ny*py - nz*pz, 2)
		- 4 * (pow(cx, 2) + pow(cy, 2) + pow(cz, 2) - 2 * cx*px + pow(px, 2)
			- 2 * cy*py + pow(py, 2) - 2 * cz*pz + pow(pz, 2) - pow(radius, 2));

	if (delta >= 0.0) {

		float aPart = -(cx*nx) + cy*ny + cz*nz + nx*px - ny*py - nz*pz;

		float a = aPart - sqrt(delta) / 2.;
		float a2 = aPart + sqrt(delta) / 2.;

		ix = a*nx + px;
		iy = a*ny + py;
		iz = a*nz + pz;
	}

	intersection[0] = ix;
	intersection[1] = iy;
	intersection[2] = iz; 
	return;	  
}

void intersectViewRayToSphereCameraSpace(float *targetPosCamSpace, float *pointInCamSpace, float *intersection) {

	float ix = NAN;
	float iy = NAN;
	float iz = NAN;

	float px = pointInCamSpace[0];
	float py = pointInCamSpace[1];
	float pz = pointInCamSpace[2];

	float d = targetPosCamSpace[2];
	float radius = (d*t) / sqrt(pow(n, 2) + pow(t, 2));

	float nNorm = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));
	float nx = px / nNorm;
	float ny = py / nNorm;
	float nz = pz / nNorm;

	float delta = pow(d, 2)*(-1 + pow(nz, 2)) + pow(radius, 2);

	if (delta >= 0.0) {

		float a = d*nz - sqrt(pow(d, 2)*(-1 + pow(nz, 2)) + pow(radius, 2));
		//float a2 = aPart + sqrt(delta) / 2.;

		ix = a*nx;
		iy = a*ny;
		iz = a*nz;
	}

	intersection[0] = ix;
	intersection[1] = iy;
	intersection[2] = iz;
	return;
}
