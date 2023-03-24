#ifndef __LOOKATCAMERA_H__
#define __LOOKATCAMERA_H__

#include "linAlg.h"
#include "transformations.h"

enum camera_positions { ON_TOP=1, BY_THE_SIDE=2, ON_BOTTOM=3 };

enum camera_current_and_former_pos {
	FROM_TOP_TO_SIDE = size_t(camera_positions::BY_THE_SIDE) * 10 + size_t(camera_positions::ON_TOP),
	FROM_SIDE_TO_BOTTOM = size_t(camera_positions::ON_BOTTOM) * 10 + size_t(camera_positions::BY_THE_SIDE),
	FROM_BOTTOM_TO_SIDE = size_t(camera_positions::BY_THE_SIDE) * 10 + size_t(camera_positions::ON_BOTTOM),
	FROM_SIDE_TO_TOP = size_t(camera_positions::ON_TOP) * 10 + size_t(camera_positions::BY_THE_SIDE),
};

void rotateCameraThroughVectors(float *RealTargetPos, float *RealCamPos, float* pv, float* pu, float* NewCamPos, float *normalForDebugReasons);

void lookAtCameraMatrix3(float * TargetPos, float * CamPos, float * normalM, float * r);		

void lookAtCameraMatrix4(float * TargetPos, float * CamPos, float * Up, float * normalM, float * r);

void setUpVector(float in_x, float in_y, float in_z);

void updateUpVector(float *targetPos, float *camPos, float *upVector);

#endif