#ifndef __LOOKATCAMERA_H__
#define __LOOKATCAMERA_H__

#include "linAlg.h"
#include "transformations.h"

//def getLookAtMatrix(CamPos, TargetPos, Up)
//	global viewerVec
void getLookAtMatrix(float *CamPos, float *TargetPos, float* Up, float* CamUp, float *viewerVec, float *m, float *r);

void getLookAtMatrix2(float *CamPos, float *TargetPos, float* Up, float* CamUp, float *viewerVec, float *m, float *r);

//def lookAtCameraMatrix(RealTargetPos, RealCamPos, Up)
void lookAtCameraMatrix(float *RealTargetPos, float *RealCamPos, float* Up, float* CamUp, float *viewerVec, float *m, float* normalM, float *r);

void rotateCameraThroughVectors(float *RealTargetPos, float *RealCamPos, float* pv, float* pu, float* NewCamPos, float *normalForDebugReasons);

void lookAtCameraMatrix2(float *TargetPos, float *CamPos, float *m, float *normalM, float *r);

void lookAtCameraMatrix3(float * TargetPos, float * CamPos, float * m, float * normalM, float * r);

#endif