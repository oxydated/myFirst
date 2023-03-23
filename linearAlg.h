//
//  linearAlg.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 14/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_linearAlg_h
#define myFirst_linearAlg_h

#include<math.h>
#include<stdio.h>
#include "linAlg.h"

#ifdef _WIN32
//#include <GL/glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

#define MYGL_2D_ROTATION_MATRIX 0

void rotationMatrix( float cx, float cy, float teta, int program );

void setPerspectiveMatrix( float *pers, int program );

//GLint getPersLocation();
//
//GLint getProgram();

void setInvertedPerspectiveMatrix(float *invPers);

void setViewportMatrix( float *view, int program );

float* getInvertedPersPectiveMatrix();

float* getInvertedViewMatrix();

#endif
