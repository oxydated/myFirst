//
//  transformations.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include "transformations.h"

static float tm[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

void rotateX( float theta, float* m, float* r ){
    tm[0]= 	1.0;		tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	cos(theta);	tm[9]=  sin(theta);     tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= -sin(theta);	tm[10]= cos(theta);     tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;

    multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void rotateY( float theta, float* m, float* r ){
    tm[0]=  cos(theta);	tm[4]= 	0.0;		tm[8]=     -sin(theta);	tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	1.0;		tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	sin(theta);	tm[6]= 	0.0;		tm[10]= 	cos(theta);	tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
    multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void rotateZ( float theta, float* m, float* r ){
    tm[0]= 	cos(theta);	tm[4]= 	sin(theta);	tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= -sin(theta);	tm[5]= 	cos(theta);	tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	1.0;		tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
    multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void translate( float tx, float ty, float tz, float* m, float* r ){
    tm[0]= 	1.0;		tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	tx;		
    tm[1]= 	0.0;		tm[5]= 	1.0;		tm[9]=      0.0;		tm[13]= 	ty;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	1.0;		tm[14]= 	tz;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
    multiplyMatrices( m, tm, r );
    //multiplyMatrices( tm, m, r );
}

void scale( float sx, float sy, float sz, float *m, float* r ){
    tm[0]= 	sx;         tm[4]= 	0.0;		tm[8]=      0.0;		tm[12]= 	0.0;		
    tm[1]= 	0.0;		tm[5]= 	sy;         tm[9]=      0.0;		tm[13]= 	0.0;		
    tm[2]= 	0.0;		tm[6]= 	0.0;		tm[10]= 	sz;         tm[14]= 	0.0;		
    tm[3]= 	0.0;		tm[7]= 	0.0;		tm[11]= 	0.0;		tm[15]= 	1.0;
    
    multiplyMatrices( m, tm, r );
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

	multiplyMatrices(m, tm, r);
}



