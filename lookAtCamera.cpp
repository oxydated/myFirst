#include "lookAtCamera.h"
#include <Windows.h>

//def getLookAtMatrix(CamPos, TargetPos, Up)
void getLookAtMatrix(float *CamPos, float *TargetPos, float* Up, float* CamUp, float *viewerVec, float *m, float *r){
	//global viewerVec

	//	############################################################## get the lookAt and Up-For-Cam vectors

	float temp[] = { 0.0, 0.0, 0.0, 0.0 };

	//	lookAtVec = normalize(TargetPos - CamPos)
	temp[0] = TargetPos[0] - CamPos[0];	
	temp[1] = TargetPos[1] - CamPos[1];	
	temp[2] = TargetPos[2] - CamPos[2];	
	temp[3] = TargetPos[3] - CamPos[3];

	float lookAtVec[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, lookAtVec);

	//	upForCam = normalize(Up - (np.dot(Up, lookAtVec) * lookAtVec))
		//np.dot(Up, lookAtVec)
	float Up_dot_lookAtVec = 0.0;
	vectorDotProduct(Up, lookAtVec, &Up_dot_lookAtVec);
		//Up - (np.dot(Up, lookAtVec) * lookAtVec)
	temp[0] = Up[0] - Up_dot_lookAtVec* lookAtVec[0];
	temp[1] = Up[1] - Up_dot_lookAtVec* lookAtVec[1];
	temp[2] = Up[2] - Up_dot_lookAtVec* lookAtVec[2];
	temp[3] = Up[3] - Up_dot_lookAtVec* lookAtVec[3];

	float upForCam[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, upForCam);

	//	############################################################## get the Rotation Axis

	//	lookAtRotationPlaneVec = lookAtVec - viewerVec
	float lookAtRotationPlaneVec[] = { 0.0, 0.0, 0.0, 0.0 };
	lookAtRotationPlaneVec[0] = lookAtVec[0] - viewerVec[0];
	lookAtRotationPlaneVec[1] = lookAtVec[1] - viewerVec[1];
	lookAtRotationPlaneVec[2] = lookAtVec[2] - viewerVec[2];
	lookAtRotationPlaneVec[3] = lookAtVec[3] - viewerVec[3];

	//	upForCamRotationPlaneVec = upForCam - Up
	float upForCamRotationPlaneVec[] = { 0.0, 0.0, 0.0, 0.0 };
	upForCamRotationPlaneVec[0] = upForCam[0] - Up[0];
	upForCamRotationPlaneVec[1] = upForCam[1] - Up[1];
	upForCamRotationPlaneVec[2] = upForCam[2] - Up[2];
	upForCamRotationPlaneVec[3] = upForCam[3] - Up[3];

	//	rotationAxis = normalize(np.cross(upForCamRotationPlaneVec[:3], lookAtRotationPlaneVec[:3]))
		//np.cross(upForCamRotationPlaneVec[:3], lookAtRotationPlaneVec[:3])
	temp[0] = 0.0;
	vectorCrossProduct(upForCamRotationPlaneVec, lookAtRotationPlaneVec, temp);

	float rotationAxis[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, rotationAxis);

	//	if np.dot(Up, lookAtVec) > 0.0:
		//np.dot(Up, lookAtVec)
	if (Up_dot_lookAtVec > 0.0){
		//rotationAxis = -rotationAxis
		rotationAxis[0] = -rotationAxis[0];
		rotationAxis[1] = -rotationAxis[1];
		rotationAxis[2] = -rotationAxis[2];
		rotationAxis[3] = -rotationAxis[3];
	}

	//	############################################################## get the rotation angle

	//	beforeRotationVec = perpendicular(Up, rotationAxis)
	float beforeRotationVec[] = { 0.0, 0.0, 0.0, 0.0 };
	perpendicular(Up, rotationAxis, beforeRotationVec);

	//	afterRotationVec = perpendicular(upForCam, rotationAxis)
	float afterRotationVec[] = { 0.0, 0.0, 0.0, 0.0 };
	perpendicular(upForCam, rotationAxis, afterRotationVec);

	//	rotationCosine = cosAngleFromVectors(beforeRotationVec, afterRotationVec)
	float rotationCosine = 0.0;
	cosAngleFromVectors(beforeRotationVec, afterRotationVec, &rotationCosine);

	//	############################################################## get the rotation quaternion

	//	halfRotationCos = halfAngleCos(rotationCosine)
	float halfRotationCos = 0.0;
	halfAngleCos(rotationCosine, &halfRotationCos);

	//	halfRotationSin = halfAngleSin(rotationCosine)
	float halfRotationSin = 0.0;
	halfAngleSin(rotationCosine, &halfRotationSin);

	if (halfRotationCos < 0.0){
		float test = halfRotationCos;
	}

	//	rotQuatAxis = halfRotationSin * rotationAxis
	float rotQuatAxis[] = { 0.0, 0.0, 0.0, 0.0 };
	rotQuatAxis[0] = halfRotationSin * rotationAxis[0];
	rotQuatAxis[1] = halfRotationSin * rotationAxis[1];
	rotQuatAxis[2] = halfRotationSin * rotationAxis[2];
	rotQuatAxis[3] = halfRotationSin * rotationAxis[3];

	//	rotQuat = halfRotationCos, rotQuatAxis[0], rotQuatAxis[1], rotQuatAxis[2]
	float rotQuat[] = { 0.0, 0.0, 0.0, 0.0 };
	rotQuat[0] = halfRotationCos;
	rotQuat[1] = rotQuatAxis[0];
	rotQuat[2] = rotQuatAxis[1];
	rotQuat[3] = rotQuatAxis[2];

	//	############################################################## get the quaternion matrix

	//	rotQuatMatrix = quaterionRotationMatrix(rotQuat)
	quaternionRotation(rotQuat, m, r);

	//	return rotQuatMatrix, lookAtVec, upForCam, rotationAxis
}

//def getLookAtMatrix2(CamPos, TargetPos, Up, viewerVec) :
void getLookAtMatrix2(float *CamPos, float *TargetPos, float* Up, float* CamUp, float *viewerVec, float *m, float *r){
	//global viewerVec

	//	############################################################## get the lookAt and Up-For-Cam vectors

	float temp[] = { 0.0, 0.0, 0.0, 0.0 };

	//	lookAtVec = normalize(TargetPos - CamPos)
	temp[0] = TargetPos[0] - CamPos[0];
	temp[1] = TargetPos[1] - CamPos[1];
	temp[2] = TargetPos[2] - CamPos[2];
	temp[3] = TargetPos[3] - CamPos[3];

	float lookAtVec[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, lookAtVec);

	//	upForCam = normalize(CamUp - (np.dot(CamUp, lookAtVec) * lookAtVec))
	//np.dot(CamUp, lookAtVec)
	float CamUp_dot_lookAtVec = 0.0;
	vectorDotProduct(CamUp, lookAtVec, &CamUp_dot_lookAtVec);
	//Up - (np.dot(Up, lookAtVec) * lookAtVec)
	temp[0] = CamUp[0] - CamUp_dot_lookAtVec* lookAtVec[0];
	temp[1] = CamUp[1] - CamUp_dot_lookAtVec* lookAtVec[1];
	temp[2] = CamUp[2] - CamUp_dot_lookAtVec* lookAtVec[2];
	temp[3] = CamUp[3] - CamUp_dot_lookAtVec* lookAtVec[3];

	float upForCam[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, upForCam);

	////// debuging matter

	float Up_cos_upForCam = 0.0;
	cosAngleFromVectors(Up, upForCam, &Up_cos_upForCam);

	if (Up_cos_upForCam < 0.05 && Up_cos_upForCam > 0.04){
		float test = Up_cos_upForCam;
	}

	if (Up_cos_upForCam < 0.04){
		float test = Up_cos_upForCam;
	}


	//	############################################################## get the Rotation Axis

//	Up_cross_upForCam = normalize(np.cross(Up[:3], upForCam[:3]))
	temp[0] = 0.0;
	vectorCrossProduct(Up, upForCam, temp);
	float Up_cross_upForCam[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, Up_cross_upForCam);

//	Up_half_upForCam = normalize((Up[:3] + upForCam[:3]) / 2.0)
	float Up_half_upForCam[] = { 0.0, 0.0, 0.0, 0.0 };
	temp[0] = (Up[0] + upForCam[0]) / 2.0;
	temp[1] = (Up[1] + upForCam[1]) / 2.0;
	temp[2] = (Up[2] + upForCam[2]) / 2.0;
	temp[3] = 0.0;
	normalizeVector(temp, Up_half_upForCam);

//	Up_cross_half = normalize(np.cross(Up_cross_upForCam[:3], Up_half_upForCam[:3]))
	temp[0] = 0.0;
	vectorCrossProduct(Up_cross_upForCam, Up_half_upForCam, temp);
	float Up_cross_half[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, Up_cross_half);

//	viewerVec_cross_lookAtVec = normalize(np.cross(viewerVec[:3], lookAtVec[:3]))
	temp[0] = 0.0;
	vectorCrossProduct(viewerVec, lookAtVec, temp);
	float viewerVec_cross_lookAtVec[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, viewerVec_cross_lookAtVec);

//	viewerVec_half_lookAtVec = normalize((viewerVec[:3] + lookAtVec[:3]) / 2.0)
	float viewerVec_half_lookAtVec[] = { 0.0, 0.0, 0.0, 0.0 };
	temp[0] = (viewerVec[0] + lookAtVec[0]) / 2.0;
	temp[1] = (viewerVec[1] + lookAtVec[1]) / 2.0;
	temp[2] = (viewerVec[2] + lookAtVec[2]) / 2.0;
	temp[3] = 0.0;
	normalizeVector(temp, viewerVec_half_lookAtVec);

//	viewerVec_cross_half = normalize(np.cross(viewerVec_cross_lookAtVec[:3], viewerVec_half_lookAtVec[:3]))
	temp[0] = 0.0;
	vectorCrossProduct(viewerVec_cross_lookAtVec, viewerVec_half_lookAtVec, temp);
	float viewerVec_cross_half[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, viewerVec_cross_half);

//	rotationAxis = normalize(np.cross(Up_cross_half[:3], viewerVec_cross_half[:3]))
	temp[0] = 0.0;
	vectorCrossProduct(Up_cross_half, viewerVec_cross_half, temp);
	float rotationAxis[] = { 0.0, 0.0, 0.0, 0.0 };
	normalizeVector(temp, rotationAxis);

//	if np.dot(Up, lookAtVec) > 0.0:
//	rotationAxis = -rotationAxis
	float Up_dot_lookAtVec = 0.0;
	vectorDotProduct(Up, lookAtVec, &Up_dot_lookAtVec);
	if (Up_dot_lookAtVec > 0.0){
		//rotationAxis = -rotationAxis
		rotationAxis[0] = -rotationAxis[0];
		rotationAxis[1] = -rotationAxis[1];
		rotationAxis[2] = -rotationAxis[2];
		rotationAxis[3] = -rotationAxis[3];
	}

	//	############################################################## get the rotation angle

	//	beforeRotationVec = perpendicular(Up, rotationAxis)
	float beforeRotationVec[] = { 0.0, 0.0, 0.0, 0.0 };
	perpendicular(Up, rotationAxis, beforeRotationVec);

	//	afterRotationVec = perpendicular(upForCam, rotationAxis)
	float afterRotationVec[] = { 0.0, 0.0, 0.0, 0.0 };
	perpendicular(upForCam, rotationAxis, afterRotationVec);

	//	rotationCosine = cosAngleFromVectors(beforeRotationVec, afterRotationVec)
	float rotationCosine = 0.0;
	cosAngleFromVectors(beforeRotationVec, afterRotationVec, &rotationCosine);

	//	############################################################## get the rotation quaternion

	//	halfRotationCos = halfAngleCos(rotationCosine)
	float halfRotationCos = 0.0;
	halfAngleCos(rotationCosine, &halfRotationCos);

	//	halfRotationSin = halfAngleSin(rotationCosine)
	float halfRotationSin = 0.0;
	halfAngleSin(rotationCosine, &halfRotationSin);

	if (halfRotationCos < 0.0){
		float test = halfRotationCos;
	}

	//	rotQuatAxis = halfRotationSin * rotationAxis
	float rotQuatAxis[] = { 0.0, 0.0, 0.0, 0.0 };
	rotQuatAxis[0] = halfRotationSin * rotationAxis[0];
	rotQuatAxis[1] = halfRotationSin * rotationAxis[1];
	rotQuatAxis[2] = halfRotationSin * rotationAxis[2];
	rotQuatAxis[3] = halfRotationSin * rotationAxis[3];

	//	rotQuat = halfRotationCos, rotQuatAxis[0], rotQuatAxis[1], rotQuatAxis[2]
	float rotQuat[] = { 0.0, 0.0, 0.0, 0.0 };
	rotQuat[0] = halfRotationCos;
	rotQuat[1] = rotQuatAxis[0];
	rotQuat[2] = rotQuatAxis[1];
	rotQuat[3] = rotQuatAxis[2];

	//	############################################################## get the quaternion matrix

	//	rotQuatMatrix = quaterionRotationMatrix(rotQuat)
	quaternionRotation(rotQuat, m, r);

	//	############################################################## update Up vector

	/*Up[0] = upForCam[0];
	Up[1] = upForCam[1];
	Up[2] = upForCam[2];
	Up[3] = upForCam[3];*/

	//	return rotQuatMatrix, lookAtVec, upForCam, rotationAxis
}

//	############################################################## get the Rotation Axis

//	Up_cross_upForCam = normalize(np.cross(Up[:3], upForCam[:3]))
//	Up_half_upForCam = normalize((Up[:3] + upForCam[:3]) / 2.0)

//	Up_cross_half = normalize(np.cross(Up_cross_upForCam[:3], Up_half_upForCam[:3]))

//	viewerVec_cross_lookAtVec = normalize(np.cross(viewerVec[:3], lookAtVec[:3]))
//	viewerVec_half_lookAtVec = normalize((viewerVec[:3] + lookAtVec[:3]) / 2.0)

//	viewerVec_cross_half = normalize(np.cross(viewerVec_cross_lookAtVec[:3], viewerVec_half_lookAtVec[:3]))

//	rotationAxis = normalize(np.cross(Up_cross_half[:3], viewerVec_cross_half[:3]))

//	if np.dot(Up, lookAtVec) > 0.0:
//	rotationAxis = -rotationAxis

//	############################################################## get the rotation angle

//def lookAtCameraMatrix(RealTargetPos, RealCamPos, Up)
void lookAtCameraMatrix(float *RealTargetPos, float *RealCamPos, float* Up, float* CamUp, float *viewerVec, float *m, float* normalM, float *r){
	float temp[16];
	//trans = translationMatrix(RealCamPos)
	identity(temp);
	float trans[16];
	translate(RealCamPos[0], RealCamPos[1], RealCamPos[2], temp, trans);

	//invTrans = np.linalg.inv(trans)
	float invTrans[16];
	invertMatrix(trans, invTrans);

	//camSpaceCamPos = np.dot(invTrans, RealCamPos)
	float camSpaceCamPos[] = { 0.0, 0.0, 0.0, 0.0 };
	//multiplyMatrixByVector(invTrans, RealCamPos, camSpaceCamPos);
	multiplyVectorByMatrix(RealCamPos, invTrans, camSpaceCamPos);

	//camSpaceTargetPos = np.dot(invTrans, RealTargetPos)
	float camSpaceTargetPos[] = { 0.0, 0.0, 0.0, 0.0 };
	//multiplyMatrixByVector(invTrans, RealTargetPos, camSpaceTargetPos);
	multiplyVectorByMatrix(RealTargetPos, invTrans, camSpaceTargetPos);

	//camSpaceRotQuatMatrix, camSpaceLookAtVec, camSpaceUpForCam, camSpaceRotationAxis = getLookAtMatrix(camSpaceCamPos, camSpaceTargetPos, Up)
	identity(temp);
	//float camSpaceRotQuatMatrix[16];
	float* &camSpaceRotQuatMatrix = normalM;
	getLookAtMatrix2(camSpaceCamPos, camSpaceTargetPos, Up, CamUp, viewerVec, temp, camSpaceRotQuatMatrix);

	//invRotQuatMatrix = np.linalg.inv(camSpaceRotQuatMatrix)
	float invRotQuatMatrix[16];
	invertMatrix(camSpaceRotQuatMatrix, invRotQuatMatrix);

	//lookAtMatrix = np.dot(invRotQuatMatrix, invTrans)
	float lookAtMatrix[16];

	multiplyMatrices(invTrans, invRotQuatMatrix, lookAtMatrix);

	multiplyMatrices(m, lookAtMatrix, r);
	//return lookAtMatrix, camSpaceLookAtVec, camSpaceUpForCam, camSpaceRotationAxis
}

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
				
	//swprintf(outputString, TEXT("v: %f, %f, %f\n"), vx, vy, vz);
	//OutputDebugString(outputString);
	//swprintf(outputString, TEXT("u: %f, %f, %f\n"), ux, uy, uz);
	//OutputDebugString(outputString);
	//swprintf(outputString, TEXT("cosAlpha: %f, sinAlpha: %f\n\n"), cosAlpha, sinAlpha);
	//OutputDebugString(outputString);
}

void lookAtCameraMatrix2(float * TargetPos, float * CamPos, float * m, float * normalM, float * r)
{
	float px = CamPos[0] - TargetPos[0];
	float py = CamPos[1] - TargetPos[1];
	float pz = CamPos[2] - TargetPos[2];

	float ry = sqrt(pow(px, 2) + pow(pz, 2));
	float ra = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));

	float cosAlpha = -pz / ry;
	float sinAlpha = px / ry;

	float cosTheta = -ry / ra;
	float sinTheta = py / ra;

	float normalY[] = { 1.0, 0.0, 0.0 };
	float normalX[] = {0.0, 1.0, 0.0 };

	float identMat[16];
	identity(identMat);

	float TranslatedToTarget[16];
	translate(TargetPos[0], TargetPos[1], TargetPos[2], identMat, TranslatedToTarget);
	////translate(px, py, pz, m, TranslatedToTarget);

	float rotatedByAlpha[16];
	quaternionFromCosSinNormal(cosAlpha, sinAlpha, normalY, identMat, rotatedByAlpha);

	float rotatedByTetha[16];
	quaternionFromCosSinNormal(cosTheta, sinTheta, normalX, identMat, rotatedByTetha);

	float TranslatedToCamera[16];
	translate(0.0, 0.0, ra, identMat, TranslatedToCamera);

	////////////////////

	//float result1[16];
	//multiplyMatrices(TranslatedToCamera, rotatedByTetha, result1);
	//float result2[16];
	//multiplyMatrices(result1, rotatedByAlpha, result2);
	//multiplyMatrices(result2, TranslatedToTarget, r);

	///////////////////

	float result1[16];
	multiplyMatrices(rotatedByAlpha, TranslatedToTarget, result1); 
	float result2[16];
	multiplyMatrices(rotatedByTetha, result1, result2);
	multiplyMatrices(TranslatedToCamera, result2, r);

	float result3[16];
	multiplyMatrices(rotatedByTetha, rotatedByAlpha, result3);
	invertMatrix(result3, normalM);
	
}

void lookAtCameraMatrix3(float * TargetPos, float * CamPos, float * m, float * normalM, float * r)
{
	TCHAR outputString[100];

	float upx = 0.0;
	float upy = 1.0;
	float upz = 0.0;

	float viewx = 0.0;
	float viewy = 0.0;
	float viewz = 1.0;

	//float upx = 1.0;
	//float upy = 0.0;
	//float upz = 0.0;

	//float viewx = 0.0;
	//float viewy = 0.0;
	//float viewz = 1.0;

	//float crossVecx = -(upz*viewy) + upy*viewz;
	//float crossVecy = upz*viewx - upx*viewz;
	//float crossVecz = -(upy*viewx) + upx*viewy;
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

	//multiplyMatrices(translationFromCamToOrigin, rotationsFromTargetToCam, r);
	multiplyMatrices(rotationsFromTargetToCam, translationFromCamToOrigin, r);

	/////////// sanity check

	float transposed_r[16];
	transposeMatrix(r, transposed_r);

	float transformedCam_shouldBeOntheOrigin[4];
	//multiplyVectorByMatrix(CamPos, r, transformedCam_shouldBeOntheOrigin);
	//multiplyVectorByMatrix(CamPos, translationFromCamToOrigin, transformedCam_shouldBeOntheOrigin);
	multiplyVectorByMatrix(CamPos, transposed_r, transformedCam_shouldBeOntheOrigin);


	float upVec[] = { upx, upy, upz, 1.0 };
	float whereIsUpVectorNow[4];
	multiplyVectorByMatrix(upVec, transposed_r, whereIsUpVectorNow);


	swprintf(outputString, TEXT("upVec: %f, %f: %f -------------------\n"), upVec[0], upVec[1], upVec[2]);
	OutputDebugString(outputString);


	float transformedTarget_shouldBeOnZAxis[4];

	multiplyVectorByMatrix(TargetPos, transposed_r, transformedTarget_shouldBeOnZAxis);

	/////////// inverted rotation

	copyMatrices(rotationsFromTargetToCam, normalM);


	//invertMatrix(result3, normalM);

}

