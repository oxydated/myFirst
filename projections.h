//
//  projections.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_projections_h
#define myFirst_projections_h

#include "linAlg.h"

void perspectiveMatrix(float in_l, float in_r, float in_n, float in_f, float in_b, float in_t, float *m, float *res );

void windowCoordToCameraSpace(float xw, float yw, float z, float *pointInCamSpace);

void invertedPerspectiveMatrix(float l, float r, float n, float f, float b, float t, float *m, float *res);

void viewportMatrix(float x0, float y0, float w, float h, float n, float f, float * m, float * res);

void normalizedScreenCoordFromWindowCoord(float sx, float sy, float &x, float &y);

void intersectViewRayToSphere(float * camPos, float * targetPos, float * pointInSpace, float * intersection);

void intersectViewRayToSphereCameraSpace(float * targetPosCamSpace, float * pointInCamSpace, float * intersection);

#endif
