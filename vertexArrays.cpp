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

//static GLfloat staticPosition[] = {     1.0f,   1.0f,  0.0f,
//                                        0.0f,   1.0f,  0.0f,         
//                                        1.0f,   0.0f,  0.0f   };

//static float Camera[] = { 50.0, 200.0, 0.0 };
static float Camera[] = { 50.0, -50.0, -100.0, 1.0 };

static float originalCameraPos[] = { 0.0, 0.0, 0.0, 1.0 };
static float originalWorldPos[] = { 0.0, 0.0, 0.0, 1.0 };
static float currentWorldPos[] = { 0.0, 0.0, 0.0, 1.0 };

static bool updateCamera = false;

enum states {
	NOT_PRESSING = size_t(false) * 2 + size_t(false),	//f			f
	START_PRESSING = size_t(false) * 2 + size_t(true), //f			t
	KEEP_PRESSING = size_t(true) * 2 + size_t(true),	//t			t
	STOP_PRESSING = size_t(true) * 2 + size_t(false)	//t			f
};

static float winX = 0.0;
static float winY = 0.0;

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

void debugQuatOutput(float cosAlpha, float sinAlpha, float *Normal) {
	TCHAR outputString[100];
	swprintf(outputString, TEXT("cosAlpha: %f, SinAlpha: %f, Normal: (%f, %f, %f)\n"),
		cosAlpha, sinAlpha, Normal[0], Normal[1], Normal[2]);
	OutputDebugString(outputString);
}

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

	float lightVector[] = { Camera[0], -Camera[1], Camera[2] };

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

	float ROp[] = { centerX, centerY, centerZ, 1.0 };	
	
	float RCp[] = { Camera[0], Camera[1], Camera[2], 1.00000000 };
	float Up[] = { 0.00000000, 0.000000000, 1.000000000 };
	float camUp[] = { 1.00000000, 0.000000000, 0.000000000, 0.000000000 };
	float viewerVec[] = { 0., 0., 1., 0. };

	float r[16];
	float normalM[16];
	
	lookAtCameraMatrix3(ROp, RCp, normalM, r);

	lookAtCameraMatrix4(ROp, RCp, Up, normalM, r);	

	float inv_r[16]; 
	invertMatrix(r, inv_r);

	float cameraBeforeTransform[] = { 0.00000000, 0.000000000, 0.000000000, 1.000000000 };

	float cameraAfterTransform[] = { 1.00000000, 0.000000000, 0.000000000, 1.000000000 };
																		   
	multiplyVectorByMatrix(RCp, r, cameraAfterTransform); 

	float lightVectorBeforeTransform[] = { 100.0, 50.0, -200.0, 1.0 };
	float lightVectorAfterTransform[] = { 0.0, 0.0, 100.0, 1.0 };

	multiplyVectorByMatrix(lightVectorBeforeTransform, r, lightVectorAfterTransform);
	setLightPosition(theProgram, lightVectorAfterTransform[0], lightVectorAfterTransform[1], lightVectorAfterTransform[2]);

	float transposePerspective[16];
	transposeMatrix(getInvertedPersPectiveMatrix(), transposePerspective); 

	// For camera update

	bool IsItBeingPressedRightNow = getButtonPressed();
	size_t currentState = size_t(updateCamera) * 2 + size_t(IsItBeingPressedRightNow);

	if (updateCamera) {
		formerWinX = winX;
		formerWinY = winY;
	}

	if (IsItBeingPressedRightNow) {
		winX = float(getWinX());
		winY = float(getWinY());

		//newCamPos[0] = unprojectedPoint[0];
		//newCamPos[1] = unprojectedPoint[1];
		//newCamPos[2] = unprojectedPoint[2];
		//newCamPos[3] = unprojectedPoint[3];	

		if (!updateCamera) {
			formerWinX = winX;
			formerWinY = winY;
		}
	}

	// For camera update

	if (IsItBeingPressedRightNow && updateCamera) {

		//////////

		float normalizedScreenX = 0.0;
		float normalizedScreenY = 0.0;

		float unprojectedPointInCamSpace[] = { 0.0, 0.0, 0.0, 0.0 };

		float winX_vec[5];
		float winY_vec[5];
		float depth_vec[5];

		winX_vec[0] = formerWinX;	winY_vec[0] = formerWinY;	depth_vec[0] = 1.5;
		winX_vec[1] = winX;			winY_vec[1] = winY;			depth_vec[1] = 1.5;

		float winInCamSpace[4];
		float formerWinInCamSpace[4];

		float* pointsInCamSpace[] = { formerWinInCamSpace , winInCamSpace };

		float depth;

		float targetPosInCamSpace[4];
		multiplyMatrixByVector(r, ROp, targetPosInCamSpace);

		for (int i = 0; i < 2; i++) {

			depth = depth_vec[i];

			normalizedScreenCoordFromWindowCoord(winX_vec[i], winY_vec[i], normalizedScreenX, normalizedScreenY);

			//	From windows coord to screen coord
			windowCoordToCameraSpace(normalizedScreenX, normalizedScreenY, depth, unprojectedPointInCamSpace);

			float intersectionWithSphereInCamSpace[] = { 0.0, 0.0, 0.0, 1.0 };
			intersectViewRayToSphereCameraSpace(targetPosInCamSpace, unprojectedPointInCamSpace, intersectionWithSphereInCamSpace);

			if (!isnan(intersectionWithSphereInCamSpace[0])) {

				float intersectionWithSphereAtTargetPosition[] = { 0.0, 0.0, 0.0, 1.0 };
				multiplyMatrixByVector(inv_r, intersectionWithSphereInCamSpace, intersectionWithSphereAtTargetPosition);

				pointsInCamSpace[i][0] = intersectionWithSphereInCamSpace[0];
				pointsInCamSpace[i][1] = intersectionWithSphereInCamSpace[1];
				pointsInCamSpace[i][2] = intersectionWithSphereInCamSpace[2];

				//pointsInCamSpace[i][0] = unprojectedPointInCamSpace[0];
				//pointsInCamSpace[i][1] = unprojectedPointInCamSpace[1];
				//pointsInCamSpace[i][2] = unprojectedPointInCamSpace[2];
				pointsInCamSpace[i][3] = 1.0;

				swprintf(outputString, TEXT("pointer coordinate: %f, %f, %f\n"),
					intersectionWithSphereAtTargetPosition[0], intersectionWithSphereAtTargetPosition[1], intersectionWithSphereAtTargetPosition[2]);
				OutputDebugString(outputString);

				OutputDebugString(TEXT("is not NAN\n"));
				blendedVertices[(302 + i) * 3 + 0] = intersectionWithSphereAtTargetPosition[0];
				blendedVertices[(302 + i) * 3 + 1] = intersectionWithSphereAtTargetPosition[1];
				blendedVertices[(302 + i) * 3 + 2] = intersectionWithSphereAtTargetPosition[2];

				/////////////////

				float theRadius = sqrt(pow(intersectionWithSphereAtTargetPosition[0] - ROp[0], 2) +
					pow(intersectionWithSphereAtTargetPosition[1] - ROp[1], 2) +
					pow(intersectionWithSphereAtTargetPosition[2] - ROp[2], 2));


				float theDistance = sqrt(pow(RCp[0] - ROp[0], 2) +
					pow(RCp[1] - ROp[1], 2) +
					pow(RCp[2] - ROp[2], 2));

				swprintf(outputString, TEXT("The Radius: %f\n"), theRadius);
				OutputDebugString(outputString);


			}
			else {
				OutputDebugString(TEXT("yes, it is a NAN\n"));
			}

		}

		
		float camPosMinusTargetPos[4];
		camPosMinusTargetPos[0] = -targetPosInCamSpace[0];
		camPosMinusTargetPos[1] = -targetPosInCamSpace[1];
		camPosMinusTargetPos[2] = -targetPosInCamSpace[2];
		camPosMinusTargetPos[3] = 1.0;

		float vectorV[4];
		vectorV[0] = formerWinInCamSpace[0] - targetPosInCamSpace[0];
		vectorV[1] = formerWinInCamSpace[1] - targetPosInCamSpace[1];
		vectorV[2] = formerWinInCamSpace[2] - targetPosInCamSpace[2];
		vectorV[3] = 1.0;

		float vectorU[4];
		vectorU[0] = winInCamSpace[0] - targetPosInCamSpace[0];
		vectorU[1] = winInCamSpace[1] - targetPosInCamSpace[1];
		vectorU[2] = winInCamSpace[2] - targetPosInCamSpace[2];
		vectorU[3] = 1.0;

		float ident[16];
		identity(ident);

		float rotateMat[16];
		quaternionFromVectorVToVectorU(vectorU, vectorV, ident, rotateMat, debugQuatOutput);

		float updateCamPos[4];
		multiplyMatrixByVector(rotateMat, camPosMinusTargetPos, updateCamPos);

		float CameraInCamSpace[4];
		CameraInCamSpace[0] = updateCamPos[0] + targetPosInCamSpace[0];
		CameraInCamSpace[1] = updateCamPos[1] + targetPosInCamSpace[1];
		CameraInCamSpace[2] = updateCamPos[2] + targetPosInCamSpace[2];
		CameraInCamSpace[3] = 1.0;

		multiplyMatrixByVector(inv_r, CameraInCamSpace, Camera);

		if (0) { int nothing = 0; }
			
		
		//float camPosMinusTargetPos[4];
		//camPosMinusTargetPos[0] = RCp[0] - ROp[0];
		//camPosMinusTargetPos[1] = RCp[1] - ROp[1];
		//camPosMinusTargetPos[2] = RCp[2] - ROp[2];
		//camPosMinusTargetPos[3] = 1.0;

		//float formerWinTransformed[4];
		//multiplyMatrixByVector(inv_r, formerWinInCamSpace, formerWinTransformed);

		//float vectorV[4];
		//vectorV[0] = formerWinTransformed[0] - ROp[0];
		//vectorV[1] = formerWinTransformed[1] - ROp[1];
		//vectorV[2] = formerWinTransformed[2] - ROp[2];
		//vectorV[3] = 1.0;

		//float WinTransformed[4];
		//multiplyMatrixByVector(inv_r, winInCamSpace, WinTransformed);

		//float vectorU[4];
		//vectorU[0] = WinTransformed[0] - ROp[0];
		//vectorU[1] = WinTransformed[1] - ROp[1];
		//vectorU[2] = WinTransformed[2] - ROp[2];
		//vectorU[3] = 1.0;

		//float ident[16];
		//identity(ident);

		//float rotateMat[16];
		//quaternionFromVectorVToVectorU(vectorU, vectorV, ident, rotateMat);

		//float updateCamPos[4];
		//multiplyMatrixByVector(rotateMat, camPosMinusTargetPos, updateCamPos);

		////float CameraInCamSpace[4];
		//Camera[0] = updateCamPos[0] + ROp[0];
		//Camera[1] = updateCamPos[1] + ROp[1];
		//Camera[2] = updateCamPos[2] + ROp[2];
		//Camera[3] = 1.0;
	}
	updateCamera = IsItBeingPressedRightNow;


	///////////////////// end of camera update

	
	//////////////////////////////////////////////////////////////////////////

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
