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
	copyMatrices(pers, persStatic);
	//persStatic[0] = pers[0];	persStatic[4] = pers[4];	persStatic[8] = pers[8];	persStatic[12] = pers[12];
	//persStatic[1] = pers[1];	persStatic[5] = pers[5];	persStatic[9] = pers[9];	persStatic[13] = pers[13];
	//persStatic[2] = pers[2];	persStatic[6] = pers[6];	persStatic[10] = pers[10];	persStatic[14] = pers[14];
	//persStatic[3] = pers[3];	persStatic[7] = pers[7];	persStatic[11] = pers[11];	persStatic[15] = pers[15];
	invertMatrix(persStatic, invPersStatic);
}

GLint getPersLocation() {
	return persLoc;
}

GLint getProgram() {
	return programNum;
}

void setInvertedPerspectiveMatrix(float * invPers)
{
	copyMatrices(invPers, invPersStatic);
}

void setViewportMatrix( float *view, int program ){
    GLint viewLocation = glGetUniformLocation( program, "View" );
    printf("location of View: %i\n", viewLocation);
    if( viewLocation >= 0 ){
		glUniformMatrix4fv( viewLocation, 1, GL_FALSE, view );
	}
	copyMatrices(view, viewStatic);
	//viewStatic[0] = view[0];	viewStatic[4] = view[4];	viewStatic[8] = view[8];	viewStatic[12] = view[12];
	//viewStatic[1] = view[1];	viewStatic[5] = view[5];	viewStatic[9] = view[9];	viewStatic[13] = view[13];
	//viewStatic[2] = view[2];	viewStatic[6] = view[6];	viewStatic[10] = view[10];	viewStatic[14] = view[14];
	//viewStatic[3] = view[3];	viewStatic[7] = view[7];	viewStatic[11] = view[11];	viewStatic[15] = view[15];
	invertMatrix(viewStatic, invViewStatic);
}

float* getInvertedPersPectiveMatrix() {
	return 	invPersStatic;
}

float* getInvertedViewMatrix() {
	return 	invViewStatic;
}
