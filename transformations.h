//
//  transformations.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 31/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_transformations_h
#define myFirst_transformations_h

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include "linAlg.h"

void rotateX( float theta, float* m, float* r );

void rotateY( float theta, float* m, float* r );

void rotateZ( float theta, float* m, float* r );

void translate( float tx, float ty, float tz, float* m, float* r );

void scale( float sx, float sy, float sz, float *m, float* r );

void quaternionRotation(float *quat, float *m, float *r);

#endif
