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
#include "projections.h"
#include "lookAtCamera.h"
#include "interactivity.h"
#include "macroUtilities.h"

static GLfloat staticPosition[] = {     1.0f,   1.0f,  0.0f,
                                        0.0f,   1.0f,  0.0f,         
                                        1.0f,   0.0f,  0.0f   };


//static float Camera[] = { -200.0, 50.0, 0.0 }; 
//static float Camera[] = { 100.0, 0.0, 100.0 };
static float Camera[] = { 50.0, -200.0, 0.0 };
//static float Camera[] = { -189.241, 71.645, 45.0 };
static bool updateCamera = false;
//static float startRotationVector[] = { 0.0, 0.0, 0.0, 0.0 };

static float formerWinX = 0.0;
static float formerWinY = 0.0;

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

static int endTimeForScene = 0;

void createVertexBuffer() {
	xmlLoadModel(numFaces, numVerts, faces, vertices, texcoord, normals);
	blendedVertices = new float[3 * numVerts];
	blendedNormals = new float[3 * numVerts];

	//////////////////////////////////////////////	test skin loading
	xmlLoadSkin(theSkin);

	float theTestNum = theSkin.boneNumVertAttrib[2];
	float theTestOffset = theSkin.boneOffsetVertAttrib[2];
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

	glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedVertices);
    glVertexAttribPointer( VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)texcoord );
	glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedNormals);
	glVertexAttribPointer(VERTEX_WEIGHT_ATT, VERTEX_WEIGHT_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)vIndices);
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
	TCHAR outputString[100];
    glBindTexture( GL_TEXTURE_2D, 1 );
    static float teta = 0.0;
    teta += 0.5*orientation;

	if (teta > 200.0 || -200.0 > teta)	reorient();

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

	//float Camera[] = { 69.311, -116.811, 26.308 };
	//float Camera[] = { 44.238, 128.108, 23.965 };

	//float lightVector[] = { Camera[0]+3, Camera[1], Camera[2]+.5 };
	//float lightVector[] = { Camera[0]+50, Camera[1]-20, Camera[2]+10 };
	float lightVector[] = { Camera[0], -Camera[1], Camera[2] };

	//float lightVector[] = { 0.0, 0.0, 100.0};

	//////////

	for (int i = 0; i < 200; i++){
		float pulse = 2.0*M_PI*( (float(i) / 100.0) - 1.0);
		vWeights[i] = cos(pulse + teta*50)*0.05;
	}

	GLint weightLocation = glGetUniformLocation(theProgram, "weight");
	glUniform1fv(weightLocation, 200, (GLfloat*) vWeights);

	//////////
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getSkeletonForTime(theSkeleton, theSceneTracks, theTime);

	transformFromSkinPoseToCurrentPose(theSkin, theSceneTracks);

	blendDualQuatFromMesh(theSkin, vertices, normals, blendedVertices, blendedNormals, numVerts);
	
	float sumVX = 0.0, sumVY = 0.0, sumVZ = 0.0;
	for (int itv = 0; itv < numVerts; ++itv) {
		int itvX = 3 * itv;
		int itvY = 3 * itv + 1;
		int itvZ = 3 * itv + 2;
		sumVX += blendedVertices[itvX];
		sumVY += blendedVertices[itvY];
		sumVZ += blendedVertices[itvZ];
	}
	float centerX = sumVX / numVerts; 
	float centerY = sumVY / numVerts; 
	float centerZ = sumVZ / numVerts;

	//float ROp[] = { 0, 0, -150, 1.0 };
	float ROp[] = { centerX, centerY, centerZ, 1.0 };
	//float RCp[] = { blendedVertices[0] - 50., blendedVertices[1] + 150., blendedVertices[2] - 50., 1.0 };
	//float ROp[] = { blendedVertices[0], blendedVertices[1], blendedVertices[2], 1.0 };

	
	float RCp[] = { Camera[0], Camera[1], Camera[2], 1.00000000 };
	float Up[] = { 1.00000000, 0.000000000, 0.000000000, 0.000000000 };
	float camUp[] = { 1.00000000, 0.000000000, 0.000000000, 0.000000000 };
	float viewerVec[] = { 0., 0., 1., 0. };

	float m[16];
	identity(m);

	float TnormalM[16];
	float normalM[16];
	identity(TnormalM);

	float r[16]; 
	float Tr[16];

	//lookAtCameraMatrix(ROp, RCp, Up, camUp, viewerVec, m, normalM, r);  

	//transposeMatrix(TnormalM, normalM);

	lookAtCameraMatrix3(ROp, RCp, m, TnormalM, r);

	copyMatrices(TnormalM, normalM);

	//copyMatrices(r, Tr);

	transposeMatrix(r, Tr);

	float cameraBeforeTransform[] = { 0.00000000, 0.000000000, 0.000000000, 1.000000000 };

	float cameraAfterTransform[] = { 1.00000000, 0.000000000, 0.000000000, 1.000000000 };
									
	float invertR[16];

	invertMatrix(r, invertR);  							  
																		   
	multiplyVectorByMatrix(RCp, r, cameraAfterTransform);
	//multiplyMatrixByVector(r, RCp, cameraAfterTransform);
	
	//swprintf(outputString, TEXT("RCp: %f, %f: %f -------------------\n"), RCp[0], RCp[1], RCp[2]);
	//OutputDebugString(outputString);
	//swprintf(outputString, TEXT("cameraAfterTransform: %f, %f, %f, %f |||||||||||||||||||\n"), cameraAfterTransform[0]/ cameraAfterTransform[3], cameraAfterTransform[1] / cameraAfterTransform[3], cameraAfterTransform[2] / cameraAfterTransform[3], cameraAfterTransform[3] / cameraAfterTransform[3]);
	//OutputDebugString(outputString);



	float lightVectorBeforeTransform[] = { 100.0, 50.0, -200.0, 1.0 };
	float lightVectorAfterTransform[] = { 0.0, 0.0, 100.0, 1.0 };

	multiplyVectorByMatrix(lightVectorBeforeTransform, r, lightVectorAfterTransform);
	setLightPosition(theProgram, lightVectorAfterTransform[0], lightVectorAfterTransform[1], lightVectorAfterTransform[2]);


	float invTr[16];
	float inv_r[16];


	invertMatrix(Tr, invTr);
	invertMatrix(r, inv_r);

	float unprojectMatrix[16];
	float tempMatrix[16];

	float transposePerspective[16];
	transposeMatrix(getInvertedPersPectiveMatrix(), transposePerspective);
	float transposeView[16];
	transposeMatrix(getInvertedViewMatrix(), transposeView);

	//multiplyMatrices(m, tm, res);
	//vec4 pos = tempView * Proj * World * tempPos;

	//multiplyMatrices(getInvertedPersPectiveMatrix(), getInvertedViewMatrix(), tempMatrix); 
	//multiplyMatrices(invTr, tempMatrix, unprojectMatrix);



	multiplyMatrices(transposePerspective, invTr, tempMatrix);
	//multiplyMatrices(invTr, getInvertedPersPectiveMatrix(), tempMatrix);
	//multiplyMatrices(transposeView, tempMatrix, unprojectMatrix);

	float winX = float(getWinX());
	float winY = float(getWinY());

	//float NEARFAR[] = { 1.0, 0.0 };
	//glGetFloatv(GL_DEPTH_RANGE, NEARFAR);
	//float winZ = (NEARFAR[1] - NEARFAR[0]) / 2.0;
	//winZ = 1.025;

	//float screenCoord[] = {float(winX),  float(winY), winZ, 1.0 };
	////float screenCoord[] = { 400.0,  0.0 , winZ, 1.0 };
	float normalizedScreenX = 0.0;
	float normalizedScreenY = 0.0;

	float unprojectedPointInCamSpace[] = { 0.0, 0.0, 0.0, 0.0 };
	float unprojectedPoint[] = { 0.0, 0.0, 0.0, 0.0 };


	float identM[16];
	identity(identM);
	float pers[16];
	float persT[16];
	perspectiveMatrix(-1.0, 1.0, -1.5, -400.0, -1.0, 1.0, identM, pers);
	transposeMatrix(pers, persT);

	float gotFromShaders[16];
	GLint persLocation = getPersLocation();
	GLint programNum = getProgram();

	glGetUniformfv(programNum, persLocation, gotFromShaders);



	float sanityCheckForPers[] = { 0.0, 0.0, 0.0, 0.0 };

	float winX_vec[5];
	float winY_vec[5];
	float depth_vec[5];
	int vertexInc_vec[5];

	winX_vec[0] = 1424;	winY_vec[0] = 700;	depth_vec[0] = 400.0;
	winX_vec[1] = winX;	winY_vec[1] = winY;	depth_vec[1] = 200.0;
	winX_vec[2] = 0;	winY_vec[2] = 0;	depth_vec[2] = 400.0;
	winX_vec[3] = 1424;	winY_vec[3] = 0;	depth_vec[3] = 400.0;
	winX_vec[4] = 700;	winY_vec[4] = 700;	depth_vec[4] = 400.0;

	float depth;

	for (int i = 0; i < 3; i++) {
		//winX = 0;
		//winY = 700;
		winX = winX_vec[i];
		winY = winY_vec[i];

		depth = depth_vec[i];

		normalizedScreenCoordFromWindowCoord(winX, winY, normalizedScreenX, normalizedScreenY);


		//	From windows coord to screen coord
		windowCoordToCameraSpace(normalizedScreenX, normalizedScreenY, depth, unprojectedPointInCamSpace);

		//	From screen to camera space
		multiplyMatrixByVector(pers, unprojectedPointInCamSpace, sanityCheckForPers);



		//multiplyVectorByMatrix(unprojectedPointInCamSpace, persT, sanityCheckForPers);
		sanityCheckForPers[0] = sanityCheckForPers[0] / sanityCheckForPers[3];
		sanityCheckForPers[1] = sanityCheckForPers[1] / sanityCheckForPers[3];
		sanityCheckForPers[2] = sanityCheckForPers[2] / sanityCheckForPers[3];
		sanityCheckForPers[3] = sanityCheckForPers[3] / sanityCheckForPers[3];

		// From camera space to world space	
		float sanityCheckForWorldMatrix[4];
		multiplyMatrixByVector(invTr, unprojectedPointInCamSpace, sanityCheckForWorldMatrix);
		//multiplyVectorByMatrix(unprojectedPointInCamSpace, invTr, sanityCheckForWorldMatrix);
		sanityCheckForWorldMatrix[0] = sanityCheckForWorldMatrix[0] / sanityCheckForWorldMatrix[3];
		sanityCheckForWorldMatrix[1] = sanityCheckForWorldMatrix[1] / sanityCheckForWorldMatrix[3];
		sanityCheckForWorldMatrix[2] = sanityCheckForWorldMatrix[2] / sanityCheckForWorldMatrix[3];
		sanityCheckForWorldMatrix[3] = sanityCheckForWorldMatrix[3] / sanityCheckForWorldMatrix[3];

		// Back from world space to camera space (should be the same as 	unprojectedPointInCamSpace)	 
		float sanityCheckForPerspAgain[4];
		multiplyMatrixByVector(r, sanityCheckForWorldMatrix, sanityCheckForPerspAgain);
		//multiplyVectorByMatrix(sanityCheckForWorldMatrix, r, sanityCheckForPerspAgain);
		sanityCheckForPerspAgain[0] = sanityCheckForPerspAgain[0] / sanityCheckForPerspAgain[3];
		sanityCheckForPerspAgain[1] = sanityCheckForPerspAgain[1] / sanityCheckForPerspAgain[3];
		sanityCheckForPerspAgain[2] = sanityCheckForPerspAgain[2] / sanityCheckForPerspAgain[3];
		sanityCheckForPerspAgain[3] = sanityCheckForPerspAgain[3] / sanityCheckForPerspAgain[3];


		float sanityCheckBackToNormalizedWindowsCoord[4];
		multiplyMatrixByVector(pers, sanityCheckForPerspAgain, sanityCheckBackToNormalizedWindowsCoord);
		sanityCheckBackToNormalizedWindowsCoord[0] = sanityCheckBackToNormalizedWindowsCoord[0] / sanityCheckBackToNormalizedWindowsCoord[3];
		sanityCheckBackToNormalizedWindowsCoord[1] = sanityCheckBackToNormalizedWindowsCoord[1] / sanityCheckBackToNormalizedWindowsCoord[3];
		sanityCheckBackToNormalizedWindowsCoord[2] = sanityCheckBackToNormalizedWindowsCoord[2] / sanityCheckBackToNormalizedWindowsCoord[3];
		sanityCheckBackToNormalizedWindowsCoord[3] = sanityCheckBackToNormalizedWindowsCoord[3] / sanityCheckBackToNormalizedWindowsCoord[3];

		//multiplyVectorByMatrix(unprojectedPointInCamSpace, inv_r, unprojectedPoint);
		multiplyMatrixByVector(invTr, unprojectedPointInCamSpace, unprojectedPoint);
		unprojectedPoint[0] = unprojectedPoint[0] / unprojectedPoint[3];
		unprojectedPoint[1] = unprojectedPoint[1] / unprojectedPoint[3];
		unprojectedPoint[2] = unprojectedPoint[2] / unprojectedPoint[3];
		unprojectedPoint[3] = unprojectedPoint[3] / unprojectedPoint[3];

		blendedVertices[(302 + i) * 3 + 0] = unprojectedPoint[0];
		blendedVertices[(302 + i) * 3 + 1] = unprojectedPoint[1];
		blendedVertices[(302 + i) * 3 + 2] = unprojectedPoint[2];
	}

	float camIncamSpace[] = { 0.0, 0.0, 0.0, 1.0 };
	float camInWorldSpace[] = { 0.0, 0.0, 0.0, 1.0 };
	multiplyVectorByMatrix(camIncamSpace, inv_r, camInWorldSpace);		
	
	float targetIncamSpace[] = { 0.0, 0.0, 100.0, 1.0 };
	float targetInWorldSpace[] = { 0.0, 0.0, 0.0, 1.0 };
	multiplyVectorByMatrix(camIncamSpace, inv_r, camInWorldSpace);

	//screenCoord[0] = /*1.025**/((1424.0 / 2.0) - screenCoord[0]);
	//screenCoord[1] = /*1.025**/(screenCoord[1] - (700.0 / 2.0));
	//screenCoord[2] = winZ;

	//multiplyVectorByMatrix(screenCoord, tempMatrix, unprojectedPoint);
	//unprojectedPoint[3] = unprojectedPoint[3] / unprojectedPoint[3];


	float startRotationVectorInCamSpace[] = { 1424.0 / 2.0, 350, 400.0, 1.0 };
	float startRotationVector[] = { 0.0, 0.0, 0.0, 0.0 };
	formerWinX = 1424;
	formerWinY = 0;
	normalizedScreenCoordFromWindowCoord(formerWinX, formerWinY, normalizedScreenX, normalizedScreenY);

	windowCoordToCameraSpace(normalizedScreenX, normalizedScreenY, depth, startRotationVectorInCamSpace);

	//windowCoordToCameraSpace(((1424.0 / 2.0) - formerWinX), (formerWinY - (700.0 / 2.0)), 1.5, unprojectedPointInCamSpace);
	multiplyVectorByMatrix(startRotationVectorInCamSpace, inv_r, startRotationVector);
	startRotationVector[0] = startRotationVector[0] / startRotationVector[3];
	startRotationVector[1] = startRotationVector[1] / startRotationVector[3];
	startRotationVector[2] = startRotationVector[2] / startRotationVector[3];
	startRotationVector[3] = startRotationVector[3] / startRotationVector[3];

	float sanityCheckForMatrixInversion[16];
	multiplyMatrices(r, inv_r, sanityCheckForMatrixInversion);
	multiplyMatrices(inv_r, r, sanityCheckForMatrixInversion);
	multiplyMatrices(invTr, r, sanityCheckForMatrixInversion);
	multiplyMatrices(r, invTr, sanityCheckForMatrixInversion);

	//screenCoord[0] = /*1.025**/((1424.0 / 2.0) - formerWinX);
	//screenCoord[1] = /*1.025**/(formerWinY - (700.0 / 2.0));
	//screenCoord[2] = winZ;

	//multiplyVectorByMatrix(screenCoord, tempMatrix, startRotationVector);
	//startRotationVector[3] = startRotationVector[3] / startRotationVector[3];

	//if (isnan(unprojectedPoint[0])) {
	//	OutputDebugString(TEXT("NAN"));
	//}

	float normalForDebugReasons[] = { 0.0, 0.0, 0.0, 1.0 };

	//////////////////////////////////////////////////////////////////////////
	if (getButtonPressed()) {

		swprintf(outputString, TEXT("getButtonPressed(): %i, updateCamera: %i\n"), getButtonPressed(), updateCamera);
		OutputDebugString(outputString);

		if (!updateCamera) {
			updateCamera = true;
		}
		else {	  			
			//rotateCameraThroughVectors(ROp, RCp, unprojectedPoint, startRotationVector, Camera, normalForDebugReasons);

			if (isnan(Camera[0])) {
				OutputDebugString(TEXT("NAN"));
			}
		}

		swprintf(outputString, TEXT("formerWinX, Y: %f, %f\n"), formerWinX, formerWinY);
		OutputDebugString(outputString);
		swprintf(outputString, TEXT("WinX, Y: %f, %f\n"), winX, winY);
		OutputDebugString(outputString); 
		swprintf(outputString, TEXT("startRotationVector: %f, %f, %f\n"), startRotationVector[0], startRotationVector[1], startRotationVector[2]);
		OutputDebugString(outputString);
		swprintf(outputString, TEXT("unprojectedPoint: %f, %f, %f\n"), unprojectedPoint[0], unprojectedPoint[1], unprojectedPoint[2]);
		OutputDebugString(outputString);
		//startRotationVector[0] = unprojectedPoint[0];
		//startRotationVector[1] = unprojectedPoint[1];
		//startRotationVector[2] = unprojectedPoint[2];
		//startRotationVector[3] = unprojectedPoint[3];
		formerWinX = winX;
		formerWinY = winY;
	}
	else {
		updateCamera = false;		
	}
	//////////////////////////////////////////////////////////////////////////


	//blendedVertices[200 * 3 + 0] = unprojectedPoint[0];
	//blendedVertices[200 * 3 + 1] = unprojectedPoint[1];
	//blendedVertices[200 * 3 + 2] = unprojectedPoint[2];	

	//blendedVertices[201 * 3 + 0] = startRotationVector[0];
	//blendedVertices[201 * 3 + 1] = startRotationVector[1];
	//blendedVertices[201 * 3 + 2] = startRotationVector[2];

	//blendedVertices[202 * 3 + 0] = blendedVertices[200 * 3 + 0];
	//blendedVertices[202 * 3 + 1] = blendedVertices[200 * 3 + 0];
	//blendedVertices[202 * 3 + 2] = blendedVertices[200 * 3 + 0];

	GLint location = glGetUniformLocation(theProgram, "World");
	printf("location of World: %i\n", location);
	glUniformMatrix4fv(location, 1, GL_FALSE, r);

	GLint invlocation = glGetUniformLocation(theProgram, "invWorld");
	printf("location of invWorld: %i\n", invlocation);
	glUniformMatrix4fv(invlocation, 1, GL_FALSE, normalM);

	//

    glEnableVertexAttribArray( VERTEX_POSITION_ATT );
    glEnableVertexAttribArray( VERTEX_TEXCOORD_ATT );
    glEnableVertexAttribArray( VERTEX_NORMAL_ATT );

	/////

	glEnableVertexAttribArray(VERTEX_WEIGHT_ATT);

	/////

    glDrawElements(	GL_TRIANGLES, numFaces*3, GL_UNSIGNED_SHORT, faces);
}
