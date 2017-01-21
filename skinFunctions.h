#ifndef __SKINFUNCTIONS_H__
#define __SKINFUNCTIONS_H__

#include "animationFunctions.h"

//skinPose bone structure
struct skinPoseBone{
	//	bone index(int)
	int boneIndex;
	
	//	skin pose dual quaternion(as an 8 floats array)
	float skinPose[8];
};
		
		// implement {transfFromInitialToCurrentPose() in oxyAnimateMesh.py}

//////////////////////////////////////////////////////////////////////////////////////////

//skin structure
struct skinData{
	//	skinPose bone structure array pointer
	skinPoseBone *skinPoseSkeleton;

	//	number of skin bones(integer)
	int numBones;

	//	vertex attribute array for offset as defined in 6.4.2.1.1. (float array)
	float *boneOffsetVertAttrib;

	//	vertex attribute array for number of bones as defined in 6.4.2.1.2. (float array)
	float *boneNumVertAttrib;

	//	vertex attribute length(integer)
	int vertAttribNum;

	//	uniform array for bone index as defined through 6.4.2.1.and 6.4.2.2. (float array)
	float *boneIndexesForSkinVertices;

	//	uniform array for bone weight as defined through 6.4.2.1.and 6.4.2.2. (float array)
	float *boneWeightForSkinVertices;

	//	uniform array length(integer)
	int boneUniformNum;

	// tranformations from the bone skin pose to current frame pose. will have the same size as sceneTracks.numTracks (X size of dual quaternion - float[8]) and will be indexed by bone Index
	float*fromSkinPoseToCurrentTransf;
};

void alocateSkinArraysForTracks(sceneTracks theTracks, skinData &theSkin);

//(2)a new array will be produced by transforming each bone in hierarchy from skin pose to current global transformation(this is the array to be sourced to the shader)
//		for every entry in skinData.skinPoseBone[]
//			transforms every skin pose bone to transformation indexed by bone Index in globalTransforms[]
//			{transfFromInitialToCurrentPose() in oxyAnimateMesh.py}
//		produce array fromInitialPoseToCurrentTransf[]

void transformFromSkinPoseToCurrentPose(skinData &theSkin, sceneTracks theTracks);

//void blendDualQuatFromMesh(skinData theSkin, float *vertices, float* &blendedVertices, int numVertices);
void blendDualQuatFromMesh(skinData theSkin, float *vertices, float* normals, float* &blendedVertices, float* &blendedNormals, int numVertices);

#endif