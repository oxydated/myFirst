#ifndef __LOOKATCAMERA_H__
#define __LOOKATCAMERA_H__

#include "linAlg.h"
#include "transformations.h"

void rotateCameraThroughVectors(float *RealTargetPos, float *RealCamPos, float* pv, float* pu, float* NewCamPos, float *normalForDebugReasons);

void lookAtCameraMatrix3(float * TargetPos, float * CamPos, float * normalM, float * r);		

void lookAtCameraMatrix4(float * TargetPos, float * CamPos, float * Up, float * normalM, float * r);

#endif