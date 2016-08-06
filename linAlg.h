//
//  linAlg.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 31/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_linAlg_h
#define myFirst_linAlg_h

#include <math.h>
#include <stdio.h>

void multiplyMatrices( float *n, float *m, float *r );

void sumMatrices( float *a, float *b, float *sm );

void multiplyMatrixByScalar( float s, float *m, float *sm );

void multiplyMatrixByVector( float *m, float *v, float *rV);

void multiplyVectorByMatrix( float *v, float *m, float *vm);

void transposeMatrix( float *m, float *mT );

void invertMatrix( float *m, float *inv);

void vectorCrossProduct( float *v, float *u, float *o );

void vectorDotProduct( float *v, float *u, float *s );

void identity( float *i );

void printVector( float *v );

void printMatrix( float *m );


inline void homogenize( float *v ){
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    v[3] /= v[3];
}

inline void sumVectors( float *v, float *u, float *sv ){
    sv[0] = v[0]+u[0];
    sv[1] = v[1]+u[1];
    sv[2] = v[2]+u[2];
}

inline void norm( float *v, float *n ){
    *n = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

inline void normalizeVector( float *v, float *nv ){
    float n;
    norm(v, &n);
    nv[0] = v[0] / n;
    nv[1] = v[1] / n;
    nv[2] = v[2] / n;
}

//perpendicular = lambda Vec, normV :  Vec[:3] - (np.dot(Vec[:3], normV)*normV)
void perpendicular(float *v, float *normV, float *per);

//cosAngleFromVectors = lambda u, v: np.dot(u[:3], v[:3]) / (np.linalg.norm(u[:3]) * np.linalg.norm(v[:3]))
void cosAngleFromVectors(float* u, float* v, float *cosine);

//halfAngleCos = lambda cosine : np.sqrt((1 + cosine) / 2.0)
inline void halfAngleCos(float cosine, float* halfCos){
	*halfCos = sqrtf(((1.0 + cosine) / 2.0));
}

//halfAngleSin = lambda cosine : np.sqrt((1 - cosine) / 2.0)
inline void halfAngleSin(float cosine, float* halfSin){
	*halfSin = sqrtf(((1.0 - cosine) / 2.0));
}

#endif
