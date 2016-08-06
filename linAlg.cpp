//
//  linAlg.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 31/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "linAlg.h"

void multiplyMatrices( float *n, float *m, float *r ){ 
	r[0] = n[0]*m[0] 	+ n[4]*m[1] 	+ n[8]*m[2] 	+ n[12]*m[3];
	r[4] = n[0]*m[4] 	+ n[4]*m[5] 	+ n[8]*m[6] 	+ n[12]*m[7];
	r[8] = n[0]*m[8] 	+ n[4]*m[9] 	+ n[8]*m[10] 	+ n[12]*m[11];
	r[12] = n[0]*m[12] 	+ n[4]*m[13] 	+ n[8]*m[14] 	+ n[12]*m[15];
	r[1] = n[1]*m[0] 	+ n[5]*m[1] 	+ n[9]*m[2] 	+ n[13]*m[3];
	r[5] = n[1]*m[4] 	+ n[5]*m[5] 	+ n[9]*m[6] 	+ n[13]*m[7];
	r[9] = n[1]*m[8] 	+ n[5]*m[9] 	+ n[9]*m[10] 	+ n[13]*m[11];
	r[13] = n[1]*m[12] 	+ n[5]*m[13] 	+ n[9]*m[14] 	+ n[13]*m[15];
	r[2] = n[2]*m[0] 	+ n[6]*m[1] 	+ n[10]*m[2] 	+ n[14]*m[3];
	r[6] = n[2]*m[4] 	+ n[6]*m[5] 	+ n[10]*m[6] 	+ n[14]*m[7];
	r[10] = n[2]*m[8] 	+ n[6]*m[9] 	+ n[10]*m[10] 	+ n[14]*m[11];
	r[14] = n[2]*m[12] 	+ n[6]*m[13] 	+ n[10]*m[14] 	+ n[14]*m[15];
	r[3] = n[3]*m[0] 	+ n[7]*m[1] 	+ n[11]*m[2] 	+ n[15]*m[3];
	r[7] = n[3]*m[4] 	+ n[7]*m[5] 	+ n[11]*m[6] 	+ n[15]*m[7];
	r[11] = n[3]*m[8] 	+ n[7]*m[9] 	+ n[11]*m[10] 	+ n[15]*m[11];
	r[15] = n[3]*m[12] 	+ n[7]*m[13] 	+ n[11]*m[14] 	+ n[15]*m[15];
}

void sumMatrices( float *a, float *b, float *sm ){
	// OLD
    sm[0] = a[0]+b[0];
    sm[1] = a[1]+b[1];
    sm[2] = a[2]+b[2];
    sm[3] = a[3]+b[3];
    sm[4] = a[4]+b[4];
    sm[5] = a[5]+b[5];
    sm[6] = a[6]+b[6];
    sm[7] = a[7]+b[7];
    sm[8] = a[8]+b[8];
    sm[9] = a[9]+b[9];
    sm[10] = a[10]+b[10];
    sm[11] = a[11]+b[11];
    sm[12] = a[12]+b[12];
    sm[13] = a[13]+b[13];
    sm[14] = a[14]+b[14];
    sm[15] = a[15]+b[15];
}


void multiplyMatrixByScalar( float s, float *m, float *sm ){
	// OLD
    sm[1] = s * m[1];
    sm[2] = s * m[2];
    sm[3] = s * m[3];
    sm[4] = s * m[4];
    sm[5] = s * m[5];
    sm[6] = s * m[6];
    sm[7] = s * m[7];
    sm[8] = s * m[8];
    sm[9] = s * m[9];
    sm[10] = s * m[10];
    sm[11] = s * m[11];
    sm[12] = s * m[12];
    sm[13] = s * m[13];
    sm[14] = s * m[14];
    sm[15] = s * m[15];
    
}

void multiplyMatrixByVector( float *m, float *v, float *rV){
	rV[0] = m[0]*v[0] 	+ m[4]*v[1] 	+ m[8]*v[2] 	+ m[12]*v[3];
	rV[1] = m[1]*v[0] 	+ m[5]*v[1] 	+ m[9]*v[2] 	+ m[13]*v[3];
	rV[2] = m[2]*v[0] 	+ m[6]*v[1] 	+ m[10]*v[2] 	+ m[14]*v[3];
	rV[3] = m[3]*v[0] 	+ m[7]*v[1] 	+ m[11]*v[2] 	+ m[15]*v[3];
}

void multiplyVectorByMatrix( float *v, float *m, float *rV){
	rV[0] = v[0]*m[0] 	+ v[1]*m[1] 	+ v[2]*m[2] 	+ v[3]*m[3];
	rV[1] = v[0]*m[4] 	+ v[1]*m[5] 	+ v[2]*m[6] 	+ v[3]*m[7];
	rV[2] = v[0]*m[8] 	+ v[1]*m[9] 	+ v[2]*m[10] 	+ v[3]*m[11];
	rV[3] = v[0]*m[12] 	+ v[1]*m[13] 	+ v[2]*m[14] 	+ v[3]*m[15];
}

void transposeMatrix( float *m, float *mT ){
	mT[0] = m[0];
	mT[4] = m[1];
	mT[8] = m[2];
	mT[12] = m[3];
	mT[1] = m[4];
	mT[5] = m[5];
	mT[9] = m[6];
	mT[13] = m[7];
	mT[2] = m[8];
	mT[6] = m[9];
	mT[10] = m[10];
	mT[14] = m[11];
	mT[3] = m[12];
	mT[7] = m[13];
	mT[11] = m[14];
	mT[15] = m[15];
}

void invertMatrix( float *m, float *inv){
	float cof0_0 = 1.0	* (m[5]*m[10]*m[15] + m[9]*m[14]*m[7] + m[13]*m[6]*m[11] - m[13]*m[10]*m[7] - m[9]*m[6]*m[15] - m[5]*m[14]*m[11]);
	float cof0_1 = -1.0	* (m[1]*m[10]*m[15] + m[9]*m[14]*m[3] + m[13]*m[2]*m[11] - m[13]*m[10]*m[3] - m[9]*m[2]*m[15] - m[1]*m[14]*m[11]);
	float cof0_2 = 1.0	* (m[1]*m[6]*m[15] + m[5]*m[14]*m[3] + m[13]*m[2]*m[7] - m[13]*m[6]*m[3] - m[5]*m[2]*m[15] - m[1]*m[14]*m[7]);
	float cof0_3 = -1.0	* (m[1]*m[6]*m[11] + m[5]*m[10]*m[3] + m[9]*m[2]*m[7] - m[9]*m[6]*m[3] - m[5]*m[2]*m[11] - m[1]*m[10]*m[7]);
	float cof1_0 = -1.0	* (m[4]*m[10]*m[15] + m[8]*m[14]*m[7] + m[12]*m[6]*m[11] - m[12]*m[10]*m[7] - m[8]*m[6]*m[15] - m[4]*m[14]*m[11]);
	float cof1_1 = 1.0	* (m[0]*m[10]*m[15] + m[8]*m[14]*m[3] + m[12]*m[2]*m[11] - m[12]*m[10]*m[3] - m[8]*m[2]*m[15] - m[0]*m[14]*m[11]);
	float cof1_2 = -1.0	* (m[0]*m[6]*m[15] + m[4]*m[14]*m[3] + m[12]*m[2]*m[7] - m[12]*m[6]*m[3] - m[4]*m[2]*m[15] - m[0]*m[14]*m[7]);
	float cof1_3 = 1.0	* (m[0]*m[6]*m[11] + m[4]*m[10]*m[3] + m[8]*m[2]*m[7] - m[8]*m[6]*m[3] - m[4]*m[2]*m[11] - m[0]*m[10]*m[7]);
	float cof2_0 = 1.0	* (m[4]*m[9]*m[15] + m[8]*m[13]*m[7] + m[12]*m[5]*m[11] - m[12]*m[9]*m[7] - m[8]*m[5]*m[15] - m[4]*m[13]*m[11]);
	float cof2_1 = -1.0	* (m[0]*m[9]*m[15] + m[8]*m[13]*m[3] + m[12]*m[1]*m[11] - m[12]*m[9]*m[3] - m[8]*m[1]*m[15] - m[0]*m[13]*m[11]);
	float cof2_2 = 1.0	* (m[0]*m[5]*m[15] + m[4]*m[13]*m[3] + m[12]*m[1]*m[7] - m[12]*m[5]*m[3] - m[4]*m[1]*m[15] - m[0]*m[13]*m[7]);
	float cof2_3 = -1.0	* (m[0]*m[5]*m[11] + m[4]*m[9]*m[3] + m[8]*m[1]*m[7] - m[8]*m[5]*m[3] - m[4]*m[1]*m[11] - m[0]*m[9]*m[7]);
	float cof3_0 = -1.0	* (m[4]*m[9]*m[14] + m[8]*m[13]*m[6] + m[12]*m[5]*m[10] - m[12]*m[9]*m[6] - m[8]*m[5]*m[14] - m[4]*m[13]*m[10]);
	float cof3_1 = 1.0	* (m[0]*m[9]*m[14] + m[8]*m[13]*m[2] + m[12]*m[1]*m[10] - m[12]*m[9]*m[2] - m[8]*m[1]*m[14] - m[0]*m[13]*m[10]);
	float cof3_2 = -1.0	* (m[0]*m[5]*m[14] + m[4]*m[13]*m[2] + m[12]*m[1]*m[6] - m[12]*m[5]*m[2] - m[4]*m[1]*m[14] - m[0]*m[13]*m[6]);
	float cof3_3 = 1.0	* (m[0]*m[5]*m[10] + m[4]*m[9]*m[2] + m[8]*m[1]*m[6] - m[8]*m[5]*m[2] - m[4]*m[1]*m[10] - m[0]*m[9]*m[6]);


	float Det = m[0]*cof0_0 + m[4]*cof0_1 + m[8]*cof0_2 + m[12]*cof0_3;


	inv[0] = cof0_0 / Det;
	inv[4] = cof0_1 / Det;
	inv[8] = cof0_2 / Det;
	inv[12] = cof0_3 / Det;
	inv[1] = cof1_0 / Det;
	inv[5] = cof1_1 / Det;
	inv[9] = cof1_2 / Det;
	inv[13] = cof1_3 / Det;
	inv[2] = cof2_0 / Det;
	inv[6] = cof2_1 / Det;
	inv[10] = cof2_2 / Det;
	inv[14] = cof2_3 / Det;
	inv[3] = cof3_0 / Det;
	inv[7] = cof3_1 / Det;
	inv[11] = cof3_2 / Det;
	inv[15] = cof3_3 / Det;
}

void vectorCrossProduct( float *v, float *u, float *o ){
	// OLD
    o[0] = v[1]*u[2] - v[2]*u[1];
    o[1] = v[2]*u[0] - v[0]*u[2];
    o[2] = v[0]*u[1] - v[1]*u[0];
}

void vectorDotProduct( float *v, float *u, float *s ){
	// OLD
    *s = v[0]*u[0]+v[1]*u[1]+v[2]*u[2];
}

void identity( float *i ){
	i[0] = 1.0;
	i[4] = 0.0;
	i[8] = 0.0;
	i[12] = 0.0;
	i[1] = 0.0;
	i[5] = 1.0;
	i[9] = 0.0;
	i[13] = 0.0;
	i[2] = 0.0;
	i[6] = 0.0;
	i[10] = 1.0;
	i[14] = 0.0;
	i[3] = 0.0;
	i[7] = 0.0;
	i[11] = 0.0;
	i[15] = 1.0;
}

void printVector( float *v ){
	// OLD
    printf("[ %f    %f  %f  %f ]\n", v[0], v[1], v[2], v[3] );
}

void printMatrix( float *m ){    
	// OLD
    printf("[ %f    %f  %f  %f ]\n", m[0], m[4], m[8], m[12] );
    printf("[ %f    %f  %f  %f ]\n", m[1], m[5], m[9], m[13] );
    printf("[ %f    %f  %f  %f ]\n", m[2], m[6], m[10], m[14] );
    printf("[ %f    %f  %f  %f ]\n", m[3], m[7], m[11], m[15] );    
}

//perpendicular = lambda Vec, normV :  Vec[:3] - (np.dot(Vec[:3], normV)*normV)
void perpendicular(float *v, float *normV, float *per){
	float v_dot_normV = 0.0;
	vectorDotProduct(v, normV, &v_dot_normV);

	per[0] = v[0] - v_dot_normV*normV[0];
	per[1] = v[1] - v_dot_normV*normV[1];
	per[2] = v[2] - v_dot_normV*normV[2];
}

//cosAngleFromVectors = lambda u, v: np.dot(u[:3], v[:3]) / (np.linalg.norm(u[:3]) * np.linalg.norm(v[:3]))
void cosAngleFromVectors(float* u, float* v, float *cosine){
	//np.dot(u[:3], v[:3])

	float u_dot_v = 0.0;
	vectorDotProduct(u, v, &u_dot_v);
	float norm_u, norm_v;
	norm(u, &norm_u);
	norm(v, &norm_v);

	*cosine = u_dot_v / (norm_u*norm_v);
}
