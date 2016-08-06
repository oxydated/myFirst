//
//  vertexArray.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#if defined _WIN32
#include "stdafx.h"
#endif

#include "vertexArrays.h"
#include "linearAlg.h"
#include "transformations.h"
#include "modelLoader.h"
#include "xmlModelLoader.h"
#include "skinFunctions.h"
#include "xmlSkinLoader.h"
#include "xmlAnimationLoader.h"
#include "lights.h"
#include "lookAtCamera.h"

//0.5, 0.5, 10.0, 1.0
static GLfloat staticPosition[] = {     1.0f,   1.0f,  0.0f,
                                        0.0f,   1.0f,  0.0f,         
                                        1.0f,   0.0f,  0.0f   };

static int numFaces = 0;
static int numVerts = 0;

static unsigned short *faces = NULL;
static float *vertices = NULL;
static float *texcoord = NULL;
static float *normals = NULL;

static float* blendedVertices = NULL;
static float* blendedNormals = NULL;

static float *vIndices;
static float *vWeights;

static float orientation = -1.0;

static GLint theProgram = 0;

static skeleton theSkeleton;
static sceneTracks theSceneTracks;
static skinData theSkin;

//static float Up[] = { 0., 1., 0., 0. };
//static float *Up = NULL;

static int endTimeForScene = 0;

void createVertexBuffer() {
	xmlLoadModel(numFaces, numVerts, faces, vertices, texcoord, normals);
	blendedVertices = new float[3 * numVerts];
	blendedNormals = new float[3 * numVerts];

	//float objectMatrix[16] = { -4.3711388286737929e-08, 1.0, 0.0, 0.0, -1.0, -4.3711388286737929e-08, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 7.8819050788879395, -28.733169555664062, 75.552009582519531, 1.0 };
	//float objectMatrix[16] = { 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 8.44548035, 1. };
	//float objectMatrix[16] = { 1.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
	//	0.00000000e+00, -4.37113883e-08, 1.00000000e+00, 0.00000000e+00,
	//	0.00000000e+00, -1.00000000e+00, -4.37113883e-08, 0.00000000e+00,
	//	3.49143505e-01, 1.27211199e+01, 1.99448700e+01, 1.00000000e+00
	//};
	////float *testVertices = new float[numVerts];
	//for (int i = 0; i < numVerts; i++){
	//	float myOldVert[4] = { vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2], 1.0 };
	//	float myNewVert[4] = { 0.0, 0.0, 0.0, 0.0 };
	//	multiplyMatrixByVector(objectMatrix, myOldVert, myNewVert);
	//	vertices[3 * i] = myNewVert[0];
	//	vertices[3 * i + 1] = myNewVert[1];
	//	vertices[3 * i + 2] = myNewVert[2];
	//}

	//////////////////////////////////////////////	test skin loading
	xmlLoadSkin(theSkin);

	float theTestNum = theSkin.boneNumVertAttrib[685];
	float theTestOffset = theSkin.boneOffsetVertAttrib[685];
	float boneIndexesTest[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float boneWeightsTest[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (int i = 0; i< int(theTestNum); i++){
		boneIndexesTest[i] = theSkin.boneIndexesForSkinVertices[int(theTestOffset) + i];
		boneWeightsTest[i] = theSkin.boneWeightForSkinVertices[int(theTestOffset) + i];
	}

	//////////////////////////////////////////////  test uniform array

	vIndices = new float[numVerts];
	for (int i = 0; i < numVerts; i++){
		vIndices[i] = float(i % 200);
	}

	vWeights = new float[200];

	////////////////////////////////////////////// test skeleton loading

	xmlLoadSkeleton(theSkeleton);

	////////////////////////////////////////////// test tracks loading

	xmlLoadTracks(theSceneTracks);

	alocateGlobalArraysForTracks(theSceneTracks);
	alocateSkinArraysForTracks(theSceneTracks, theSkin);

	endTimeForScene = getEndTimeForTracks(theSceneTracks);

	//////////////////////////////////////////////// for lookAt Cam

	//Up = new float[4];
	//Up[0] = 1.; Up[1] = 0.; Up[2] = 0.; Up[3] = 0.;
 
    //glVertexAttribPointer( VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)vertices );
	glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedVertices);
    glVertexAttribPointer( VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)texcoord );
    //glVertexAttribPointer( VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)normals );
	glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedNormals);
	//
	//glVertexAttribPointer(VERTEX_WEIGHT_ATT, VERTEX_WEIGHT_SIZE, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLvoid*)vIndices);
	glVertexAttribPointer(VERTEX_WEIGHT_ATT, VERTEX_WEIGHT_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)vIndices);
	//
}


void bindVertexBuffer(){
    
}

void setProgram( GLint program ){
    theProgram = program;
}

void reorient(){
    orientation = -orientation;
}

void drawVertexArray(){    
    glBindTexture( GL_TEXTURE_2D, 1 );
    static float teta = 0.0;
    teta += 0.005*orientation;

	static float theTime = 0.0;
	theTime += 20.0;
	if (theTime > float(endTimeForScene)) theTime = 0.0;
    
    float mat[16];    
    identity(mat);
    
    float rot[16];
    identity(rot);
    
    float trans[16];
    identity(trans);   

    translate( 0.0, 0.0, 300.0, mat,trans );

    rotateY( teta, trans, rot  );  

    translate( 0.0, 0.0, -100.0, rot,trans );

	float normalMat[16];
	identity(normalMat);

	float normalRot[16];
	identity(normalRot);


	rotateY( teta, normalMat, normalRot );
    
    float invWorld[16];

    invertMatrix( normalRot, invWorld);
    
    float tInvWorld[16];
    transposeMatrix( invWorld, tInvWorld );
	//float lightVector[] = { -15.0, 0.0, -1.0, 1.0 };

	//float Camera[] = { -0.0283399541 + 15, 26.2844849 - 20, 12.0541649 + 2.0 };
	float Camera[] = { -4.41113997 + 50, 83.9176025 - 20, 108.0 };
	//float lightVector[] = { Camera[0]+3, Camera[1], Camera[2]+.5 };
	float lightVector[] = { Camera[0]+50, Camera[1]-20, Camera[2]+10 };
 //   GLint location = glGetUniformLocation( theProgram, "World");
 //   printf("location of World: %i\n", location);
 //    //glUniformMatrix4fv( location, 1, GL_FALSE, rot );
	//glUniformMatrix4fv( location, 1, GL_FALSE, trans );   
      

	//////////

	for (int i = 0; i < 200; i++){
		float pulse = 2.0*M_PI*( (float(i) / 100.0) - 1.0);
		vWeights[i] = cos(pulse + teta*50)*0.05;
	}

	GLint weightLocation = glGetUniformLocation(theProgram, "weight");
	glUniform1fv(weightLocation, 200, (GLfloat*) vWeights);

	//////////

    setLightPosition( theProgram, lightVector[0], lightVector[1], lightVector[2] );
    
    
    //rotationMatrix( 0.0, 0.0, teta, theProgram );
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);


	//float currentTime = 400.0;
	getSkeletonForTime(theSkeleton, theSceneTracks, theTime);

	transformFromSkinPoseToCurrentPose(theSkin, theSceneTracks);

	blendDualQuatFromMesh(theSkin, vertices, normals, blendedVertices, blendedNormals, numVerts);
	//
	float ROp[] = { blendedVertices[0], blendedVertices[1], blendedVertices[2]-2.5, 1.0 };
	//float ROp[] = { blendedVertices[0], blendedVertices[1], 75.5, 1.0 };
	//float ROp[] = { -4.41113997, 83.9176025, 11.2481737, 1.0 };
	//float RCp[] = { blendedVertices[0] - 50., blendedVertices[1] + 150., blendedVertices[2] - 50., 1.0 };
	
	float RCp[] = { Camera[0], Camera[1], Camera[2], 1.00000000 };
	//float RCp[] = { 249.827286-20, -9.27583790-400., -11.9338779+50, 1.00000000 };
	//float RCp[] = { -80.000000, -300.000000, 150.000000, 1.00000000 };
	float Up[] = { 1.00000000, 0.000000000, 0.000000000, 0.000000000 };
	float camUp[] = { 1.00000000, 0.000000000, 0.000000000, 0.000000000 };
	float viewerVec[] = { 0., 0., 1., 0. };

	float m[16];
	identity(m);

	float normalM[16];
	identity(normalM);

	float r[16];

	lookAtCameraMatrix(ROp, RCp, Up, camUp, viewerVec, m, normalM, r);

	float Tr[16];

	transposeMatrix(r, Tr);

	GLint location = glGetUniformLocation(theProgram, "World");
	printf("location of World: %i\n", location);
	//glUniformMatrix4fv( location, 1, GL_FALSE, rot );
	glUniformMatrix4fv(location, 1, GL_FALSE, Tr);

	GLint invlocation = glGetUniformLocation(theProgram, "invWorld");
	printf("location of invWorld: %i\n", invlocation);
	// glUniformMatrix4fv( location, 1, GL_FALSE, rot );
	//glUniformMatrix4fv(invlocation, 1, GL_FALSE, tInvWorld);
	glUniformMatrix4fv(invlocation, 1, GL_FALSE, normalM);

	//

    glEnableVertexAttribArray( VERTEX_POSITION_ATT );
    glEnableVertexAttribArray( VERTEX_TEXCOORD_ATT );
    glEnableVertexAttribArray( VERTEX_NORMAL_ATT );

	/////

	glEnableVertexAttribArray(VERTEX_WEIGHT_ATT);

	/////

    //glDrawArrays( GL_TRIANGLES, 0, 3);
    glDrawElements(	GL_TRIANGLES, numFaces*3, GL_UNSIGNED_SHORT, faces);
}
