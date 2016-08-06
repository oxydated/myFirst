//
//  initScene.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "initScene.h"
#include "vertexArrays.h"
#include "shaders.h"
#include "transformations.h"
#include "projections.h"
#include "linearAlg.h"
#include "setTexture.h"

#define SPIT_ERROR     if(glGetError()!= GL_NO_ERROR) printf("error up to line %i\n", __LINE__ ); else printf("ok in %i\n", __LINE__ );

void initGLES(int x, int y, int width, int height){
    
//    /////////////////// matrix tests
//    
//    float mat[16];
//    printf("----------\n");
//
//    
//    identity(mat);
//    
//    float rotatedX[16];
//    
//    rotateX( M_PI/4, mat, rotatedX );
//    
//    printMatrix(mat);
//    printf("----------\n");
//    printMatrix(rotatedX);
//    
//    float inverted[16];
//    
//    invertMatrix( rotatedX, inverted );
//    printf("----------\n");
//    printMatrix(inverted);
//    
//    float invRotX[16];
//    multiplyMatrices( rotatedX, inverted, invRotX );
//    printf("----------\n");
//    printMatrix(invRotX);
//    printf("----------\n");
//
//    
//    
//    ///////////////////
    glClearDepthf(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glDepthRangef( 0.0, 1.0 );
    glViewport(0, 0, width, height);
    glClearColor(0.25, 0.0, 0.5, 1.0);
    
    float mat[16];
    identity(mat);
    
    float pers[16];

	//perspectiveMatrix( -1.0, 1.0, -2.0, -10.0, -1.0, 1.0, mat, pers );
	perspectiveMatrix( -1.0, 1.0, -1.5, -400.0, -1.0, 1.0, mat, pers );
    printf("perspective Matrix:\n");
    printMatrix(pers);
    
    float view[16];
    
    viewportMatrix(x, y, width, height, mat, view); 
    printf("\nview Matrix:\n");
    printMatrix(view);
    
    float poin[4] = { 0.5, 0.5, 10.0, 1.0 };
    float retPoinvm[4];
    float retPoinmv[4];
    
    multiplyVectorByMatrix( poin, pers, retPoinvm);
    multiplyMatrixByVector( poin, pers, retPoinmv);
    printf("Point by pers\n");
    printVector( poin );
    printf("retPoinvm by pers\n");
    printVector( retPoinvm );
    homogenize( retPoinvm );
    printVector( retPoinvm );
    printf("retPoinmv by pers\n");
    printVector( retPoinmv );
    homogenize( retPoinmv );
    printVector( retPoinmv );
    
    createVertexBuffer();
    GLint program = loadAndCompileShader();
    setProgram( program );
    //setSampler( 0, program );
	SPIT_ERROR
	setTexture(program);
    SPIT_ERROR
    setPerspectiveMatrix( pers, program );    
    setViewportMatrix( view, program );
    drawVertexArray();
    
}