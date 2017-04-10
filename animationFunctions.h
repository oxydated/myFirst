#ifndef	__ANIMATIONFUNCTIONS_H__
#define	__ANIMATIONFUNCTIONS_H__

#include "dualQuaternionFunctions.h"
#include <map>
#include <stack>

//Interpolation structure
struct interpolData{
	//	starting dual quaternion(as an 8 floats array)
	float startTransform[8];
	//	theAngle (float)
	float theAngle;
	//	theUvector(float[3])
	float theUvector[3];
	//	theSfactor(float)
	float theSfactor;
	//	theMvector(float[3])
	float theMvector[3];
};

/////////////////////////////////////////////////////////////////////////////////////////////////

//KeyFrame structure
struct keyFrame{
	//	starting time, ending time of the interval(integers)
	int startTime;
	int endTime;
	//	pointer to the key - frame data(Interpolation structure)
	interpolData* keyFrameData;
};

//track structure
struct track{
//	theKeyFrames(keyFrame pointer)
	keyFrame* theFrames;
//	numKeys(integer)
	int numKeys;
};

//sceneTracks structure
struct sceneTracks{
//	theTracks(track structure pointer) (not to be feature in skeleton structure anymore)
	track* theTracks;
//	numTracks
	int numTracks;
//	global transformations for current frame
	float *globalTransforms;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

//boneNode structure
struct boneNode{
	//	subTreeRootIndex(integer)
	int boneNodeIndex;
	//			the index itself points to the boneTrack array itself
	//	children(pointers to boneNode structures)
	boneNode* children;
	//	numChildren(integer)
	int numChildren;
};

typedef std::map<int, track> trackMapByInt;

//skeleton structure
struct skeleton{
//	theRoot(boneNode)
	boneNode *theRoot;
//	numBones(integer)
	int numBones;
};

//(0)loop interpolates bones local transformations
//		get total time for animation cycle from the Tracks

int getEndTimeForTracks(sceneTracks theTracks);

//		{ getIntervalFromTrackForTime() in oxySourceAnimation.py }

void getIntervalFromTrackForTime(track theTrack, float currentTime, keyFrame* &theKeyFrame, float &step);

//		{getQuatFromTrackForTime() in oxySourceAnimation.py}

void getQuatFromTrackForKeyFrameAndStep(keyFrame* theKeyFrame, float step, DUALQUAARG(q));

void getQuatFromTrackForTime(track theTrack, float currentTime, DUALQUAARG(q));

//(1)and calculate global transformation for every bone in hierarchy.An array with global transformations for every bone will be calculated
//		**globalTransforms[] will have the same size as sceneTracks.numTracks and will be indexed by bone Index

void alocateGlobalArraysForTracks(sceneTracks &theTracks);

//		{ getSkeletonForTime() in oxyAnimateMesh.py }
//		** will descend by the tree in struct skeleton

typedef std::stack< boneNode*> boneStack;

void getSkeletonForTime(skeleton theSkeleton, sceneTracks &theTracks, float currentTime, bool shouldIPrintIt = false);

//(2)a new array will be produced by transforming each bone in hierarchy from skin pose to current global transformation(this is the array to be sourced to the shader)
//		for every entry in skinData.skinPoseBone[]
//			transforms every skin pose bone to transformation indexed by bone Index in globalTransforms[]
//			{transfFromInitialToCurrentPose() in oxyAnimateMesh.py}
//		produce array fromInitialPoseToCurrentTransf[]

#endif