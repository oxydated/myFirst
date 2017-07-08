//
//  linearAlg.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 14/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "linearAlg.h"

static float persStatic[] = {	1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0	};

static float viewStatic[] = {	1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0 };

static float invPersStatic[] = {	1.0, 0.0, 0.0, 0.0,
									0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 1.0, 0.0,
									0.0, 0.0, 0.0, 1.0 };

static float invViewStatic[] = {	1.0, 0.0, 0.0, 0.0,
									0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 1.0, 0.0,
									0.0, 0.0, 0.0, 1.0 };

static GLint persLoc = 0;
static GLint programNum = 0;

void rotationMatrix( float cx, float cy, float teta, int program ){
    GLfloat mat[4] = { cos(teta), sin(teta), -sin(teta), cos(teta) };
    GLint location = glGetUniformLocation( program, "mRotation");
    printf("location of mRotation: %i\n", location);
    glUniformMatrix2fv( location, 1, GL_FALSE, mat );
}

void setPerspectiveMatrix( float *pers, int program ){
    GLint persLocation = glGetUniformLocation( program, "Proj");
    printf("location of Proj: %i\n", persLocation);
    glUniformMatrix4fv( persLocation, 1, GL_FALSE, pers );
	persLoc = persLocation;
	programNum = program;
	oxyde::linAlg::copyMatrices(pers, persStatic);
	oxyde::linAlg::invertMatrix(persStatic, invPersStatic);
}

GLint getPersLocation() {
	return persLoc;
}

GLint getProgram() {
	return programNum;
}

void setInvertedPerspectiveMatrix(float * invPers)
{
	oxyde::linAlg::copyMatrices(invPers, invPersStatic);
}

void setViewportMatrix( float *view, int program ){
    GLint viewLocation = glGetUniformLocation( program, "View" );
    printf("location of View: %i\n", viewLocation);
    if( viewLocation >= 0 ){
		glUniformMatrix4fv( viewLocation, 1, GL_FALSE, view );
	}
	oxyde::linAlg::copyMatrices(view, viewStatic);
	oxyde::linAlg::invertMatrix(viewStatic, invViewStatic);
}

float* getInvertedPersPectiveMatrix() {
	return 	invPersStatic;
}

float* getInvertedViewMatrix() {
	return 	invViewStatic;
}
