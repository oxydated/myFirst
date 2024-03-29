//
//  transformations.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "../stdafx.h"
#endif

#include <iostream>
#include "transformations.h"

static float tm[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

void rotateX( float theta, float* m, float* r ){
    tm[0]= 	1.0;		tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	cos(theta);	tm[9]=  sin(theta);     tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= -sin(theta);	tm[10]= cos(theta);     tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;

	oxyde::linAlg::multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void rotateY( float theta, float* m, float* r ){
    tm[0]=  cos(theta);	tm[4]= 	0.0;		tm[8]=     -sin(theta);	tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	1.0;		tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	sin(theta);	tm[6]= 	0.0;		tm[10]= 	cos(theta);	tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
	oxyde::linAlg::multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void rotateZ( float theta, float* m, float* r ){
    tm[0]= 	cos(theta);	tm[4]= 	sin(theta);	tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= -sin(theta);	tm[5]= 	cos(theta);	tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	1.0;		tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
	oxyde::linAlg::multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void translate( float tx, float ty, float tz, float* m, float* r ){
    tm[0]= 	1.0;		tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	tx;		
    tm[1]= 	0.0;		tm[5]= 	1.0;		tm[9]=      0.0;		tm[13]= 	ty;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	1.0;		tm[14]= 	tz;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
	oxyde::linAlg::multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void scale( float sx, float sy, float sz, float *m, float* r ){
    tm[0]= 	sx;         tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	sy;         tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	sz;         tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
	oxyde::linAlg::multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

//def quaterionRotationMatrix(q) :
void quaternionRotation(float *quat, float *m, float *r){

	//	qw, qx, qy, qz = q
	float &qw = quat[0], &qx = quat[1], &qy = quat[2], &qz = quat[3];

	//	return np.array((
	//	((1 - 2 * (qy*qy + qz*qz)),		2 * (qx*qy - qw*qz),		2 * (qx*qz + qw*qy),		0),
	//	(2 * (qx*qy + qw*qz),			(1 - 2 * (qx*qx + qz*qz)),	2 * (qy*qz - qw*qx),		0),
	//	(2 * (qx*qz - qw*qy),			2 * (qy*qz + qw*qx),		(1 - 2 * (qx*qx + qy*qy)),	0),
	//	(0,								0,							0,							1)
	//	))

	tm[0] = (1 - 2 * (qy*qy + qz*qz));	tm[4] = 2 * (qx*qy - qw*qz);		tm[8] = 2 * (qx*qz + qw*qy);		tm[12] = 0.0;
	tm[1] = 2 * (qx*qy + qw*qz);		tm[5] = (1 - 2 * (qx*qx + qz*qz));	tm[9] = 2 * (qy*qz - qw*qx);		tm[13] = 0.0;
	tm[2] = 2 * (qx*qz - qw*qy);		tm[6] = 2 * (qy*qz + qw*qx);		tm[10] = (1 - 2 * (qx*qx + qy*qy));	tm[14] = 0.0;
	tm[3] = 0.0;						tm[7] = 0.0;						tm[11] = 0.0;						tm[15] = 1.0;

	//tm[0] = 1.0;		tm[4] = 0.0;		tm[8] = 0.0;		tm[12] = tx;
	//tm[1] = 0.0;		tm[5] = 1.0;		tm[9] = 0.0;		tm[13] = ty;
	//tm[2] = 0.0;		tm[6] = 0.0;		tm[10] = 1.0;		tm[14] = tz;
	//tm[3] = 0.0;		tm[7] = 0.0;		tm[11] = 0.0;		tm[15] = 1.0;

	oxyde::linAlg::multiplyMatrices(m, tm, r);
}

void quaternionFromCosSinNormal(float cosAlpha, float sinAlpha, float *Normal, float * m, float * r)
{
	float sign = sinAlpha > 0.0 ? 1.0 : -1.0;

	float cosTheta = sign * sqrt((1.0 + cosAlpha) / 2.0);
	float sinTheta = sqrt((1.0 - cosAlpha) / 2.0);

	float qs = cosTheta;
	float qx = sinTheta * Normal[0];
	float qy = sinTheta * Normal[1];
	float qz = sinTheta * Normal[2];

	if ((fabs(cosAlpha - 1.0)) > 0.000001) {

		float rotQuat[] = { qs, qx, qy, qz };

		quaternionRotation(rotQuat, m, r);
	}
	else {
		oxyde::linAlg::copyMatrices(m, r);
	}
}

void quaternionFromVectorVToVectorU(float * v, float * u, float * m, float * r, debugQuat debugQuatFunc)
{
	float vx = v[0];
	float vy = v[1];
	float vz = v[2];

	float nv = sqrt(vx*vx + vy*vy + vz*vz);
	vx = vx / nv;
	vy = vy / nv;
	vz = vz / nv;

	float ux = u[0];
	float uy = u[1];
	float uz = u[2];

	float nu = sqrt(ux*ux + uy*uy + uz*uz);
	ux = ux / nu;
	uy = uy / nu;
	uz = uz / nu;

	float cosAlpha = ux*vx + uy*vy + uz*vz;
	float sinAlpha = sqrt(pow(uz*vy - uy*vz, 2) + pow(-uz*vx + ux*vz, 2) + pow(uy*vx - ux*vy, 2));

	float normX = (uz*vy - uy*vz) / sinAlpha;
	float normY = (-uz*vx + ux*vz) / sinAlpha;
	float normZ = (uy*vx - ux*vy) / sinAlpha;

	float Normal[] = { normX, normY, normZ };

	if (debugQuatFunc != NULL) {
		debugQuatFunc(cosAlpha, sinAlpha, Normal);
	}

	if (!isnan(cosAlpha) && !isnan(sinAlpha) && !isnan(normX) && !isnan(normY) && !isnan(normZ)) {
		quaternionFromCosSinNormal(cosAlpha, sinAlpha, Normal, m, r);
	}
	else {
		oxyde::linAlg::copyMatrices(m, r);
	}
}



