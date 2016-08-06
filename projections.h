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

void perspectiveMatrix( float l, float r, float n, float f, float b, float t, float *m, float *res );

void viewportMatrix( float x0, float y0, float w, float h, float *m, float *res );

#endif
