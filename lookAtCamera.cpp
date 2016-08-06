#include "lookAtCamera.h"

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