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

//static float Camera[] = { 50.0, 200.0, 0.0 };

static float Up[] = { 0.00000000, 0.000000000, 1.000000000 };
static float Camera[] = {1200, -100.0, 100.0, 1.0 };
//static float Camera[] = { -100, -100.0, 100.0, 1.0 };

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

	float theTestNum = theSkin.boneNumVertAttrib[2];
	float theTestOffset = theSkin.boneOffsetVertAttrib[2];
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

	//////////////////////////////////////////////// for lookAt Cam

	glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedVertices);
	glVertexAttribPointer(VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)texcoord);
	glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)blendedNormals);
	glVertexAttribPointer(VERTEX_WEIGHT_ATT, VERTEX_WEIGHT_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)vIndices);

	////////////////////////////////////////////////////////////////////

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
	//startTime = 1910;
	//endTime = 1930;

	static float theTime = startTime;
	theTime += 10.0;
	//theTime += 100.0;
	if (theTime > float(endTime)) {
		theTime = startTime;
		printit = false;
	}

	//theTime = 3672.4765625;
	//theTime = 3702.4765625;
	//theTime = 3702.47734375;
	//theTime = 3709.47734375;


	//theTime = 1920.0;
	//theTime = 1920.01;

	//theTime = 4000.0;
	//theTime = 4000.01;
	//theTime = 0.001;

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

	GLint weightLocation = glGetUniformLocation(theProgram, "weight");
	glUniform1fv(weightLocation, 200, (GLfloat*)vWeights);

	//////////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//if (executeOnce) {
	if (true) {
		executeOnce = false;

		//if (printit) {
		//	swprintf(outputString, TEXT("(* { time,	{ boneIndex,{ localTransformQuat },{ globalTransformQuat } }, ...} *)\n"));
		//	OutputDebugString(outputString);
		//	swprintf(outputString, TEXT("{\n"));
		//	OutputDebugString(outputString);
		//}

		getSkeletonForTime(theSkeleton, theSceneTracks, theTime, false);
		//getSkeletonForTime(theSkeleton, theSceneTracks, 1920, false);

		//if (printit) {
		//	swprintf(outputString, TEXT("}\n"));
		//	OutputDebugString(outputString);
		//}

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
		blendDualQuatFromMesh(theSkin, vertices, normals, blendedVertices, blendedNormals, numVerts, false);

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

	//oxyde::log::printLine();
	//oxyde::log::printText(L"Degenerated vertices:");

	for (int itv = 0; itv < numVerts; ++itv) {
		int itvX = 3 * itv;
		int itvY = 3 * itv + 1;
		int itvZ = 3 * itv + 2;

		//if (blendedVertices[itvX] > 100.0 || blendedVertices[itvY] > 100.0 || blendedVertices[itvZ] > 100.0 ||
		//	blendedVertices[itvX] < -100.0 || blendedVertices[itvY] < -100.0 || blendedVertices[itvZ] < -100.0) {
		//	oxyde::log::printText(std::to_wstring(itv));
		//}

		//blendedVertices[itvX] = blendedVertices[itvX] < 100.0 ? blendedVertices[itvX] : 0.0;
		//blendedVertices[itvY] = blendedVertices[itvY] < 100.0 ? blendedVertices[itvY] : 0.0;
		//blendedVertices[itvZ] = blendedVertices[itvZ] < 100.0 ? blendedVertices[itvZ] : 0.0;

		//blendedVertices[itvX] = blendedVertices[itvX] > -100.0 ? blendedVertices[itvX] : 0.0;
		//blendedVertices[itvY] = blendedVertices[itvY] > -100.0 ? blendedVertices[itvY] : 0.0;
		//blendedVertices[itvZ] = blendedVertices[itvZ] > -100.0 ? blendedVertices[itvZ] : 0.0;

		maxX = blendedVertices[itvX] > maxX ? blendedVertices[itvX] : maxX;
		maxY = blendedVertices[itvY] > maxY ? blendedVertices[itvY] : maxY;
		maxZ = blendedVertices[itvZ] > maxZ ? blendedVertices[itvZ] : maxZ;

		minX = blendedVertices[itvX] < minX ? blendedVertices[itvX] : minX;
		minY = blendedVertices[itvY] < minY ? blendedVertices[itvY] : minY;
		minZ = blendedVertices[itvZ] < minZ ? blendedVertices[itvZ] : minZ;

		sumVX += blendedVertices[itvX];
		sumVY += blendedVertices[itvY];
		sumVZ += blendedVertices[itvZ];
	}

	oxyde::log::printPointInSpace(L"maxCorner", maxX, maxY, maxZ);
	oxyde::log::printPointInSpace(L"minCorner", minX, minY, minZ);
	//{19.2066, 20.4101, 103.101}

	//blendedVertices[100 * 3 + 0] = 19.2066;
	//blendedVertices[100 * 3 + 1] = 20.4101;
	//blendedVertices[100 * 3 + 2] = 103.101;

	//{32.3473, 21.3105, 117.296}

	//blendedVertices[101 * 3 + 0] = 32.3473;
	//blendedVertices[101 * 3 + 1] = 21.3105;
	//blendedVertices[101 * 3 + 2] = 117.296;

	//{32.3473, 21.3105, 117.296}

	//blendedVertices[601 * 3 + 0] = 32.3473;
	//blendedVertices[601 * 3 + 1] = 21.3105;
	//blendedVertices[601 * 3 + 2] = 117.296;

	//{31.842, 21.7759, 123.946}

	//blendedVertices[602 * 3 + 0] = 31.842;
	//blendedVertices[602 * 3 + 1] = 21.7759;
	//blendedVertices[602 * 3 + 2] = 123.946;

	//oxyde::log::printLine();

	float centerX = sumVX / numVerts;
	float centerY = sumVY / numVerts;
	float centerZ = sumVZ / numVerts;

	float ROp[] = { centerX, centerY, centerZ, 1.0 };

	float RCp[] = { Camera[0], Camera[1], Camera[2], 1.00000000 };
	float viewerVec[] = { 0., 0., 1., 0. };

	float r[16];
	float normalM[16];

	//float RUp[] = { 0.0, 0.0, 0.0 };
	//updateUpVector(ROp, RCp, RUp);

	lookAtCameraMatrix3(ROp, RCp, normalM, r);

	//lookAtCameraMatrix4(ROp, RCp, RUp, normalM, r);
	lookAtCameraMatrix4(ROp, RCp, Up, normalM, r);

	float inv_r[16];
	oxyde::linAlg::invertMatrix(r, inv_r);

	float cameraBeforeTransform[] = { 0.00000000, 0.000000000, 0.000000000, 1.000000000 };

	float cameraAfterTransform[] = { 1.00000000, 0.000000000, 0.000000000, 1.000000000 };

	oxyde::linAlg::multiplyVectorByMatrix(RCp, r, cameraAfterTransform);

	//float lightVectorBeforeTransform[] = { 100.0, 50.0, -200.0, 1.0 };
	//float lightVectorBeforeTransform[] = { RCp[0], RCp[1], RCp[2], 1.0 };
	float lightVectorBeforeTransform[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightVectorAfterTransform[] = { 0.0, 0.0, 100.0, 1.0 };

	//multiplyVectorByMatrix(lightVectorBeforeTransform, inv_r, lightVectorAfterTransform);
	oxyde::linAlg::multiplyVectorByMatrix(lightVectorBeforeTransform, inv_r, lightVectorAfterTransform);
	setLightPosition(theProgram, lightVectorAfterTransform[0], lightVectorAfterTransform[1], lightVectorAfterTransform[2]);
	//setLightPosition(theProgram, lightVectorBeforeTransform[0], lightVectorBeforeTransform[1], lightVectorBeforeTransform[2]);
	//setLightPosition(theProgram, 0.0, 0.0, 0.0);

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
			//quaternionFromVectorVToVectorU(vectorV, vectorU, ident, rotateMat, debugQuatOutput);
			quaternionFromVectorVToVectorU(vectorV, vectorU, ident, rotateMat);

			float originalCamPosMinusTargetPos[4];
			originalCamPosMinusTargetPos[0] = originalCameraPos[0] - ROp[0];
			originalCamPosMinusTargetPos[1] = originalCameraPos[1] - ROp[1];
			originalCamPosMinusTargetPos[2] = originalCameraPos[2] - ROp[2];
			originalCamPosMinusTargetPos[3] = 1.0;

			float updateCamPos[4];
			oxyde::linAlg::multiplyMatrixByVector(rotateMat, originalCamPosMinusTargetPos, updateCamPos);

			//debugVectorName(TEXT("winInCamSpace"))(winInCamSpace);

			//debugVectorName(TEXT("ROp"))(ROp);

			//debugVectorName(TEXT("intersectionWithSphereAtTargetPosition"))(intersectionWithSphereAtTargetPosition);

			//debugVectorName(TEXT("originalCamPosMinusTargetPos"))(originalCamPosMinusTargetPos);

			//debugMatrixName(TEXT("rotateMat"))(rotateMat);

			//debugVectorName(TEXT("updateCamPos"))(updateCamPos);

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

	//

	glEnableVertexAttribArray(VERTEX_POSITION_ATT);
	glEnableVertexAttribArray(VERTEX_TEXCOORD_ATT);
	glEnableVertexAttribArray(VERTEX_NORMAL_ATT);

	/////

	glEnableVertexAttribArray(VERTEX_WEIGHT_ATT);

	/////

	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_SHORT, faces);
}

