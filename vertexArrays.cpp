//
//  vertexArray.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#if defined _WIN32
#define NOMINMAX
#include "stdafx.h"
#endif

#include<array>
#include<algorithm>
#include <limits>

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
#include "debugLog.h"

static GLuint vao = 1;
static GLuint element_index_buffer = 1;
static GLuint vertex_position_buffer = 1;
static GLuint vertex_texcoord_buffer = 1;
static GLuint vertex_normal_buffer = 1;

static GLuint vertex_bone_num_buffer = 1;
static GLuint vertex_bone_offset_buffer = 1;

static GLuint store_boneIndexes_buffer = 1;
static GLuint store_boneWeight_buffer = 1;
static GLuint store_fromSkinPose_buffer = 1;


//static float Camera[] = { 50.0, 200.0, 0.0 };

static float Up[] = { 0.00000000, 0.000000000, 1.000000000 };
//static float Camera[] = {1200, 1200.0, 1200.0, 1.0 };
static float Camera[] = { 400, 400.0, 100.0, 1.0 };

static float originalCameraPos[] = { 0.0, 0.0, 0.0, 1.0 };
static float originalWorldPos[] = { 0.0, 0.0, 0.0, 1.0 };
static float currentWorldPos[] = { 0.0, 0.0, 0.0, 1.0 };

// For camera update
static float originalR[16];
static float originalInv_r[16];

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
	swprintf(outputString, TEXT("cosAlpha = %f, SinAlpha = %f, Normal = {%f, %f, %f}\n"),
		cosAlpha, sinAlpha, Normal[0], Normal[1], Normal[2]);
	OutputDebugString(outputString);
}

typedef void(*debugVec)(float *);
static TCHAR vecName[100];

void debugVector(float *vec) {
	TCHAR outputString[100];
	swprintf(outputString, TEXT("%s = {%f, %f, %f, %f}\n"),
		vecName, vec[0], vec[1], vec[2], vec[3]);
	OutputDebugString(outputString);
}

void debugMatrix(float *m) {
	TCHAR outputString[400];
	swprintf(outputString, TEXT("%s = {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}\n"),
		vecName,
		m[0], m[4], m[8], m[12],
		m[1], m[5], m[9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15]);
	OutputDebugString(outputString);
}

debugVec debugVectorName(TCHAR* vecArgName) {
	wcscpy(vecName, vecArgName);
	return debugVector;
}

debugVec debugMatrixName(TCHAR* matArgName) {
	wcscpy(vecName, matArgName);
	return debugMatrix;
}

void createVertexBuffer() {
	xmlLoadModel(numFaces, numVerts, faces, vertices, texcoord, normals);
	blendedVertices = new float[3 * numVerts];
	blendedNormals = new float[3 * numVerts];

	//////////////////////////////////////////////	test skin loading
	xmlLoadSkin(theSkin);

	int theTestNum = theSkin.boneNumVertAttrib[2];
	int theTestOffset = theSkin.boneOffsetVertAttrib[2];
	float boneIndexesTest[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float boneWeightsTest[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	for (int i = 0; i< int(theTestNum); i++) {
		boneIndexesTest[i] = theSkin.boneIndexesForSkinVertices[int(theTestOffset) + i];
		boneWeightsTest[i] = theSkin.boneWeightForSkinVertices[int(theTestOffset) + i];
	}

	//////////////////////////////////////////////  test uniform array

	vIndices = new float[numVerts];
	for (int i = 0; i < numVerts; i++) {
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLenum myMistake = glGetError();

	glGenBuffers(1, &element_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numFaces * 3, faces, GL_STATIC_DRAW);

	myMistake = glGetError();


	//////////////////////////////////////////////// for lookAt Cam

	glGenBuffers(1, &vertex_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_POSITION_SIZE * sizeof(GLfloat)*numVerts, (GLvoid*)vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	myMistake = glGetError();

	glGenBuffers(1, &vertex_texcoord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_texcoord_buffer);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_TEXCOORD_SIZE * sizeof(GLfloat)*numVerts, (GLvoid*)texcoord, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	myMistake = glGetError();

	glGenBuffers(1, &vertex_normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_NORMAL_SIZE * sizeof(GLfloat)*numVerts, (GLvoid*)normals, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	myMistake = glGetError();

	glGenBuffers(1, &vertex_bone_num_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_bone_num_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*numVerts, (GLvoid*)theSkin.boneNumVertAttrib, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BONE_NUM_ATT, 1, GL_INT, GL_FALSE, 0, (GLvoid*)0);
	myMistake = glGetError();

	glGenBuffers(1, &vertex_bone_offset_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_bone_offset_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*numVerts, (GLvoid*)theSkin.boneOffsetVertAttrib, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BONE_OFFSET_ATT, 1, GL_INT, GL_FALSE, 0, (GLvoid*)0);
	myMistake = glGetError();

	glGenBuffers(1, &store_boneIndexes_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_boneIndexes_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint)*theSkin.boneUniformNum, (GLvoid*)theSkin.boneIndexesForSkinVertices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, store_boneIndexes_buffer);
	myMistake = glGetError();

	//layout(binding = 1) buffer boneIndexes {
	//	int boneIndexesForSkinVertices[];
	//};

	glGenBuffers(1, &store_boneWeight_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_boneWeight_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat)*theSkin.boneUniformNum, (GLvoid*)theSkin.boneWeightForSkinVertices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, store_boneWeight_buffer);
	myMistake = glGetError();

	//layout(binding = 2) buffer boneWeight {
	//	float boneWeightForSkinVertices[];
	//};

	glGenBuffers(1, &store_fromSkinPose_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_fromSkinPose_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * 8 * theSceneTracks.numTracks, (GLvoid*)theSkin.fromSkinPoseToCurrentTransf, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, store_fromSkinPose_buffer);
	myMistake = glGetError();

	//layout(binding = 3) buffer fromSkinPose {
	//	mat2x4 fromSkinPoseToCurrentTransf[];
	//};

	//////////////////////////////////////////////////////////////////// 
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	myMistake = glGetError();

	glEnableVertexAttribArray(VERTEX_POSITION_ATT);
	glEnableVertexAttribArray(VERTEX_TEXCOORD_ATT);
	glEnableVertexAttribArray(VERTEX_NORMAL_ATT);
	glEnableVertexAttribArray(VERTEX_BONE_NUM_ATT);
	glEnableVertexAttribArray(VERTEX_BONE_OFFSET_ATT);

	setUpVector(Up[0], Up[1], Up[2]);
}


void bindVertexBuffer() {

}

void setProgram(GLint program) {
	theProgram = program;
}

void reorient() {
	orientation = -orientation;
}

void drawVertexArray() {

	static bool executeOnce = true;

	TCHAR outputString[200];
	glBindTexture(GL_TEXTURE_2D, 1);
	static float teta = 0.0;
	teta += 0.5*orientation;

	if (teta > 200.0 || -200.0 > teta)	reorient();

	static bool printit = true;

	float startTime = 0.0;
	float endTime = endTimeForScene;

	static float theTime = startTime;
	theTime += 10.0;
	if (theTime > float(endTime)) {
		theTime = startTime;
		printit = false;
	}

	//theTime = 100.;

	float mat[16];
	oxyde::linAlg::identity(mat);

	float rot[16];
	oxyde::linAlg::identity(rot);

	float trans[16];
	oxyde::linAlg::identity(trans);

	translate(0.0, 0.0, 300.0, mat, trans);

	rotateY(teta, trans, rot);

	translate(0.0, 0.0, -100.0, rot, trans);

	float normalMat[16];
	oxyde::linAlg::identity(normalMat);

	float normalRot[16];
	oxyde::linAlg::identity(normalRot);


	rotateY(teta, normalMat, normalRot);

	float lightVector[] = { Camera[0], -Camera[1], Camera[2] };

	//////////

	for (int i = 0; i < 200; i++) {
		float pulse = 2.0*M_PI*((float(i) / 100.0) - 1.0);
		vWeights[i] = cos(pulse + teta * 50)*0.05;
	}

	//////////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if (executeOnce) {
	if (true) {
		executeOnce = false;
		
		getSkeletonForTime(theSkeleton, theSceneTracks, theTime, false);

		if (printit) {
			swprintf(outputString, TEXT("{%f, {\n"),
				theTime);
			OutputDebugString(outputString);
		}

		transformFromSkinPoseToCurrentPose(theSkin, theSceneTracks, printit);

		if (printit) {
			swprintf(outputString, TEXT("}},\n"));
			OutputDebugString(outputString);
		}

		if (false) {
			swprintf(outputString, TEXT("{%f, {\n"),
				theTime);
			OutputDebugString(outputString);
		}
		//blendDualQuatFromMesh(theSkin, vertices, normals, blendedVertices, blendedNormals, numVerts, false);

		if (false) {
			swprintf(outputString, TEXT("}},\n"));
			OutputDebugString(outputString);
		}
	}
	float maxX = std::numeric_limits<float>::min();
	float minX = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();

	float sumVX = 0.0, sumVY = 0.0, sumVZ = 0.0;


	for (int itv = 0; itv < numVerts; ++itv) {
		int itvX = 3 * itv;
		int itvY = 3 * itv + 1;
		int itvZ = 3 * itv + 2;

		maxX = vertices[itvX] > maxX ? vertices[itvX] : maxX;
		maxY = vertices[itvY] > maxY ? vertices[itvY] : maxY;
		maxZ = vertices[itvZ] > maxZ ? vertices[itvZ] : maxZ;

		minX = vertices[itvX] < minX ? vertices[itvX] : minX;
		minY = vertices[itvY] < minY ? vertices[itvY] : minY;
		minZ = vertices[itvZ] < minZ ? vertices[itvZ] : minZ;

		sumVX += vertices[itvX];
		sumVY += vertices[itvY];
		sumVZ += vertices[itvZ];
	}

	oxyde::log::printPointInSpace(L"maxCorner", maxX, maxY, maxZ);
	oxyde::log::printPointInSpace(L"minCorner", minX, minY, minZ);

	float centerX = sumVX / numVerts;
	float centerY = sumVY / numVerts;
	float centerZ = sumVZ / numVerts;

	float ROp[] = { centerX, centerY, centerZ, 1.0 };

	float RCp[] = { Camera[0], Camera[1], Camera[2], 1.00000000 };
	float viewerVec[] = { 0., 0., 1., 0. };

	float r[16];
	float normalM[16];
	
	lookAtCameraMatrix3(ROp, RCp, normalM, r);

	lookAtCameraMatrix4(ROp, RCp, Up, normalM, r);

	float inv_r[16];
	oxyde::linAlg::invertMatrix(r, inv_r);

	float cameraBeforeTransform[] = { 0.00000000, 0.000000000, 0.000000000, 1.000000000 };

	float cameraAfterTransform[] = { 1.00000000, 0.000000000, 0.000000000, 1.000000000 };

	oxyde::linAlg::multiplyVectorByMatrix(RCp, r, cameraAfterTransform);

	float lightVectorBeforeTransform[] = { RCp[0], RCp[1], RCp[2], 1.0 };

	setLightPosition(theProgram, lightVectorBeforeTransform[0], lightVectorBeforeTransform[1], lightVectorBeforeTransform[2]);

	float transposePerspective[16];
	oxyde::linAlg::transposeMatrix(getInvertedPersPectiveMatrix(), transposePerspective);

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

		if (!updateCamera) {
			formerWinX = winX;
			formerWinY = winY;
		}
	}

	// For camera update

	if (currentState == states::START_PRESSING) {
		oxyde::linAlg::copyMatrices(r, originalR);
		oxyde::linAlg::copyMatrices(inv_r, originalInv_r);
	}

	if ((currentState == states::START_PRESSING) || (currentState == states::KEEP_PRESSING)) {

		//////////

		float normalizedScreenX = 0.0;
		float normalizedScreenY = 0.0;

		float unprojectedPointInCamSpace[] = { 0.0, 0.0, 0.0, 0.0 };

		float winInCamSpace[4];
		float formerWinInCamSpace[4];

		float depth;

		float targetPosInCamSpace[4];
		oxyde::linAlg::multiplyMatrixByVector(originalR, ROp, targetPosInCamSpace);

		depth = 1.5;

		normalizedScreenCoordFromWindowCoord(winX, winY, normalizedScreenX, normalizedScreenY);

		//	From windows coord to screen coord
		windowCoordToCameraSpace(normalizedScreenX, normalizedScreenY, depth, unprojectedPointInCamSpace);

		float intersectionWithSphereInCamSpace[] = { 0.0, 0.0, 0.0, 1.0 };
		intersectViewRayToSphereCameraSpace(targetPosInCamSpace, unprojectedPointInCamSpace, intersectionWithSphereInCamSpace);

		float intersectionWithSphereAtTargetPosition[] = { 0.0, 0.0, 0.0, 1.0 };

		if (!isnan(intersectionWithSphereInCamSpace[0])) {
			oxyde::linAlg::multiplyMatrixByVector(originalInv_r, intersectionWithSphereInCamSpace, intersectionWithSphereAtTargetPosition);

			winInCamSpace[0] = intersectionWithSphereInCamSpace[0];
			winInCamSpace[1] = intersectionWithSphereInCamSpace[1];
			winInCamSpace[2] = intersectionWithSphereInCamSpace[2];
			winInCamSpace[3] = 1.0;

			swprintf(outputString, TEXT("pointer coordinate: %f, %f, %f\n"),
				intersectionWithSphereAtTargetPosition[0], intersectionWithSphereAtTargetPosition[1], intersectionWithSphereAtTargetPosition[2]);
			OutputDebugString(outputString);

			OutputDebugString(TEXT("is not NAN\n"));

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

		float WinTransformed[4];
		oxyde::linAlg::multiplyMatrixByVector(originalInv_r, winInCamSpace, WinTransformed);
		blendedVertices[(302 + 1) * 3 + 0] = WinTransformed[0];
		blendedVertices[(302 + 1) * 3 + 1] = WinTransformed[1];
		blendedVertices[(302 + 1) * 3 + 2] = WinTransformed[2];

		if (currentState == states::START_PRESSING) {
			originalCameraPos[0] = RCp[0];
			originalCameraPos[1] = RCp[1];
			originalCameraPos[2] = RCp[2];
			originalCameraPos[3] = RCp[3];

			originalWorldPos[0] = WinTransformed[0];
			originalWorldPos[1] = WinTransformed[1];
			originalWorldPos[2] = WinTransformed[2];
			originalWorldPos[3] = WinTransformed[3];
		}

		if (currentState == states::KEEP_PRESSING) {
			currentWorldPos[0] = WinTransformed[0];
			currentWorldPos[1] = WinTransformed[1];
			currentWorldPos[2] = WinTransformed[2];
			currentWorldPos[3] = WinTransformed[3];

			blendedVertices[(302 + 0) * 3 + 0] = originalWorldPos[0];
			blendedVertices[(302 + 0) * 3 + 1] = originalWorldPos[1];
			blendedVertices[(302 + 0) * 3 + 2] = originalWorldPos[2];

			float vectorV[4];
			vectorV[0] = currentWorldPos[0] - ROp[0];
			vectorV[1] = currentWorldPos[1] - ROp[1];
			vectorV[2] = currentWorldPos[2] - ROp[2];
			vectorV[3] = 1.0;

			//debugVectorName(TEXT("vectorV"))(vectorV);

			float vectorU[4];
			vectorU[0] = originalWorldPos[0] - ROp[0];
			vectorU[1] = originalWorldPos[1] - ROp[1];
			vectorU[2] = originalWorldPos[2] - ROp[2];
			vectorU[3] = 1.0;

			//debugVectorName(TEXT("vectorU"))(vectorU);

			float ident[16];
			oxyde::linAlg::identity(ident);

			float rotateMat[16];
			quaternionFromVectorVToVectorU(vectorV, vectorU, ident, rotateMat);

			float originalCamPosMinusTargetPos[4];
			originalCamPosMinusTargetPos[0] = originalCameraPos[0] - ROp[0];
			originalCamPosMinusTargetPos[1] = originalCameraPos[1] - ROp[1];
			originalCamPosMinusTargetPos[2] = originalCameraPos[2] - ROp[2];
			originalCamPosMinusTargetPos[3] = 1.0;

			float updateCamPos[4];
			oxyde::linAlg::multiplyMatrixByVector(rotateMat, originalCamPosMinusTargetPos, updateCamPos);

			Camera[0] = updateCamPos[0] + ROp[0];
			Camera[1] = updateCamPos[1] + ROp[1];
			Camera[2] = updateCamPos[2] + ROp[2];
			Camera[3] = 1.0;
		}
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

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_fromSkinPose_buffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLfloat) * 8 * theSceneTracks.numTracks, (GLvoid*)theSkin.fromSkinPoseToCurrentTransf);
	GLenum myMistake = glGetError();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	myMistake = glGetError();
	
	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_SHORT, 0);
}

