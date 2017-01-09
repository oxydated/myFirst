#include "lookAtCamera.h"
#include <Windows.h>

void rotateCameraThroughVectors(float * RealTargetPos, float * RealCamPos, float * pv, float * pu, float * NewCamPos, float *normalForDebugReasons)
{
	TCHAR outputString[100];
	float vx = pv[0] - RealTargetPos[0];
	float vy = pv[1] - RealTargetPos[1];
	float vz = pv[2] - RealTargetPos[2];

	float nv = sqrt(vx*vx + vy*vy + vz*vz);	
	vx = vx / nv;
	vy = vy / nv;
	vz = vz / nv;

	float ux = pu[0] - RealTargetPos[0];
	float uy = pu[1] - RealTargetPos[1];
	float uz = pu[2] - RealTargetPos[2];

	float nu = sqrt(ux*ux + uy*uy + uz*uz);
	ux = ux / nu;
	uy = uy / nu;
	uz = uz / nu;

	float cosAlpha = ux*vx + uy*vy + uz*vz;
	float sinAlpha = sqrt(pow(uz*vy - uy*vz, 2) + pow(-uz*vx + ux*vz, 2) + pow(uy*vx - ux*vy, 2));

	//swprintf(outputString, TEXT("cosAlpha: %f, sinAlpha: %f,>>>>>>>>>>>		 angle: %f		<<<<<<\n"), cosAlpha, sinAlpha, atan(sinAlpha /cosAlpha));
	//OutputDebugString(outputString);


	float sign = sinAlpha > 0.0 ? 1.0 : -1.0;

	//cosAlpha = np.clip(cosAlpha, -1.0, 1.0)

	float cosTheta = sign * sqrt((1.0 + cosAlpha) / 2.0);
	float sinTheta = sqrt((1.0 - cosAlpha) / 2.0);

	//qW = cosTheta
	//qV = theEigenDict[theOneEigenvalue]
	//qX, qY, qZ = qV*sinTheta 

	float camVecX = RealCamPos[0] - RealTargetPos[0];
	float camVecY = RealCamPos[1] - RealTargetPos[1];
	float camVecZ = RealCamPos[2] - RealTargetPos[2];

	float oldCamVec[] = { camVecX , camVecY , camVecZ , 1.0 };
	float newCamVec[] = { 0.0, 0.0, 0.0, 0.0 };

	float normX = (uz*vy - uy*vz);
	float normY = (-uz*vx + ux*vz);
	float normZ = (uy*vx - ux*vy);

	float qs = cosTheta;
	float qx = (sinTheta / sinAlpha) * (uz*vy - uy*vz);
	float qy = (sinTheta / sinAlpha) * (-uz*vx + ux*vz);
	float qz = (sinTheta / sinAlpha) * (uy*vx - ux*vy);
	normalForDebugReasons[0] = normX / sinAlpha;
	normalForDebugReasons[1] = normY / sinAlpha;
	normalForDebugReasons[2] = normZ / sinAlpha;

	if ((fabs(cosAlpha- 1.0) ) > 0.000001) {

		float m[16];
		identity(m);

		float r[16];

		float rotQuat[] = { qs, qx, qy, qz };
		//float rotQuat[] = { qs, -qx, -qy, -qz };

		quaternionRotation(rotQuat, m, r);
		multiplyVectorByMatrix(oldCamVec, r, newCamVec);
		//multiplyMatrixByVector(r, oldCamVec, newCamVec);
	} else {
		newCamVec[0] = oldCamVec[0];
		newCamVec[1] = oldCamVec[1];
		newCamVec[2] = oldCamVec[2];
		newCamVec[3] = oldCamVec[3]; 
	}

	NewCamPos[0] = newCamVec[0] + RealTargetPos[0];
	NewCamPos[1] = newCamVec[1] + RealTargetPos[1];
	NewCamPos[2] = newCamVec[2] + RealTargetPos[2];
	NewCamPos[3] = 1.0;
						
	if (isnan(NewCamPos[0])) {
		OutputDebugString(TEXT("NAN"));
	}
}
 
void lookAtCameraMatrix3(float * TargetPos, float * CamPos, float * normalM, float * r)
{
	TCHAR outputString[100];

	float upx = 0.0;
	float upy = 1.0;
	float upz = 0.0;

	float viewx = 0.0;
	float viewy = 0.0;
	float viewz = 1.0;

	float crossVecx = upz*viewy - upy*viewz;
	float crossVecy = -(upz*viewx) + upx*viewz;
	float crossVecz = upy*viewx - upx*viewy;

	float fromCamToTargetx = TargetPos[0] - CamPos[0];
	float fromCamToTargety = TargetPos[1] - CamPos[1];
	float fromCamToTargetz = TargetPos[2] - CamPos[2];


	float view[] = { viewx, viewy, viewz };
	float targetFromCam[] = { fromCamToTargetx , fromCamToTargety, fromCamToTargetz };

	float dotTargetFromCamByView = fromCamToTargetx*viewx + fromCamToTargety*viewy + fromCamToTargetz*viewz;
	float dotTargetFromCamBycrossVec = crossVecx*fromCamToTargetx + crossVecy*fromCamToTargety + crossVecz*fromCamToTargetz;

	float targetProjectedOverView[] = { dotTargetFromCamByView * viewx, dotTargetFromCamByView * viewy , dotTargetFromCamByView * viewz };
	float targetProjectedOverCrossVec[] = { dotTargetFromCamBycrossVec * crossVecx, dotTargetFromCamBycrossVec * crossVecy , dotTargetFromCamBycrossVec * crossVecz };

	float sanityCheck[] = { fromCamToTargetx , 0.0, fromCamToTargetz };		
	float targetProjectedToPlaneViewCrossVec[] = {	targetProjectedOverView[0] + targetProjectedOverCrossVec[0] ,
													targetProjectedOverView[1] + targetProjectedOverCrossVec[1],
													targetProjectedOverView[2] + targetProjectedOverCrossVec[2] };

	float identMat[16];
	identity(identMat);

	/////////// rotations from cam to target

	float rotationFromViewToProjectedTarget[16];
	quaternionFromVectorVToVectorU(view, targetProjectedToPlaneViewCrossVec, identMat, rotationFromViewToProjectedTarget);

	float rotationFromProjectedTargetToActualTarget[16];
	quaternionFromVectorVToVectorU(targetProjectedToPlaneViewCrossVec, targetFromCam, identMat, rotationFromProjectedTargetToActualTarget);

	float rotationsFromCamToTarget[16];
	multiplyMatrices(rotationFromProjectedTargetToActualTarget, rotationFromViewToProjectedTarget, rotationsFromCamToTarget);

	/////////// inverted rotations

	float rotationFromActualTargetToProjectedTarget[16];
	quaternionFromVectorVToVectorU(targetFromCam, targetProjectedToPlaneViewCrossVec, identMat, rotationFromActualTargetToProjectedTarget);

	float rotationFromProjectedTargetToView[16];
	quaternionFromVectorVToVectorU(targetProjectedToPlaneViewCrossVec, view, identMat, rotationFromProjectedTargetToView);

	float rotationsFromTargetToCam[16];
	multiplyMatrices(rotationFromProjectedTargetToView, rotationFromActualTargetToProjectedTarget, rotationsFromTargetToCam);

	/////////// translation from Cam to Origin

	float translationFromCamToOrigin[16];
	translate(-CamPos[0], -CamPos[1], -CamPos[2], identMat, translationFromCamToOrigin);

	/////////// camera transform

	multiplyMatrices(rotationsFromTargetToCam, translationFromCamToOrigin, r);

	/////////// sanity check

	float transposed_r[16];
	transposeMatrix(r, transposed_r);

	float transformedCam_shouldBeOntheOrigin[4];
	multiplyMatrixByVector(r, CamPos, transformedCam_shouldBeOntheOrigin);


	float upVec[] = { upx, upy, upz, 1.0 };
	float whereIsUpVectorNow[4];
	multiplyMatrixByVector(r, upVec, whereIsUpVectorNow);


	swprintf(outputString, TEXT("upVec: %f, %f: %f -------------------\n"), upVec[0], upVec[1], upVec[2]);
	OutputDebugString(outputString);


	float transformedTarget_shouldBeOnZAxis[4];	
	multiplyMatrixByVector(r, TargetPos, transformedTarget_shouldBeOnZAxis);

	/////////// inverted rotation

	copyMatrices(rotationsFromTargetToCam, normalM);   
}	

void lookAtCameraMatrix4(float * TargetPos, float * CamPos, float * Up, float * normalM, float * r) { 

	float identMat[16];
	identity(identMat);

	//	Translate from CamPos to Origin

	float translateCamPosToOrigin[16];
	translate(-CamPos[0], -CamPos[1], -CamPos[2], identMat, translateCamPosToOrigin);

	// Translate Target by translateCamPosToOrigin
	float o[4];
	multiplyMatrixByVector(translateCamPosToOrigin, TargetPos, o);
	float normO = sqrt(pow(o[0], 2) + pow(o[1], 2) + pow(o[2], 2));
	o[0] = o[0] / normO;
	o[1] = o[1] / normO;
	o[2] = o[2] / normO;
	o[3] = 1.0;

	// get the front vector f[] for the camera

	float normUp = sqrt(pow(Up[0], 2) + pow(Up[1], 2) + pow(Up[2], 2));

	float u[4];
	u[0] = Up[0] / normUp;
	u[1] = Up[1] / normUp;
	u[2] = Up[2] / normUp;
	u[3] = 1.0;

	float uNormSqr = pow(u[0], 2) + pow(u[1], 2) + pow(u[2], 2);
	float f[4];
	f[0] = o[0] - (u[0] * (o[0] * u[0] + o[1] * u[1] + o[2] * u[2])) / uNormSqr;
	f[1] = o[1] - (u[1] * (o[0] * u[0] + o[1] * u[1] + o[2] * u[2])) / uNormSqr;
	f[2] = o[2] - (u[2] * (o[0] * u[0] + o[1] * u[1] + o[2] * u[2])) / uNormSqr;
	f[3] = 1.0;

	float normF = sqrt(pow(f[0], 2) + pow(f[1], 2) + pow(f[2], 2));
	f[0] = f[0] / normF;
	f[1] = f[1] / normF;
	f[2] = f[2] / normF;
	f[3] = 1.0;

	// Rotate from o[] to f[]

	float fromVectorOtoF[16];
	quaternionFromVectorVToVectorU(o, f, identMat, fromVectorOtoF);

	// Rotate from u to y
	float y[] = { 0.0, 1.0, 0.0, 1.0 };

	float fromVectorUtoAxisY[16];
	quaternionFromVectorVToVectorU(u, y, identMat, fromVectorUtoAxisY);

	// Rotate the vector f[] by fromVectorUtoAxisY

	float nF[4];
	multiplyMatrixByVector(fromVectorUtoAxisY, f, nF);

	// Rotate from nF to z 

	float z[] = { 0.0, 0.0, 1.0, 1.0 };

	float fromVectorNFtoAxisZ[16];
	quaternionFromVectorVToVectorU(nF, z, identMat, fromVectorNFtoAxisZ);

	// Compose the transformation

	float result0[16];
	multiplyMatrices(fromVectorOtoF, translateCamPosToOrigin, result0);

	float result1[16];
	multiplyMatrices(fromVectorUtoAxisY, result0, result1);

	float result2[16];
	multiplyMatrices(fromVectorNFtoAxisZ, result1, r);
}
