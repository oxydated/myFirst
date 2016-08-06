#include "animationFunctions.h"

//(0)loop interpolates bones local transformations
//		get total time for animation cycle from the Tracks

int getEndTimeForTracks(sceneTracks theTracks){
	int laterIntervalEnd = 0;
	for (int i = 0; i < theTracks.numTracks; i++){
		track &myTrack = theTracks.theTracks[i];
		for (int j = 0; j < myTrack.numKeys; j++){
			keyFrame &myKeyFrame = myTrack.theFrames[j];
			laterIntervalEnd = (myKeyFrame.endTime > laterIntervalEnd) ? myKeyFrame.endTime : laterIntervalEnd;
		}
	}
	return laterIntervalEnd;
}

//		{ getIntervalFromTrackForTime() in oxySourceAnimation.py }

void getIntervalFromTrackForTime(track theTrack, float currentTime, keyFrame* &theKeyFrame, float &step){
	// O(n) search because it is not expected to be there a huge number of keyFrames
	if (theTrack.numKeys == 0){
		theKeyFrame = NULL;
		return;
	}
	int i = 0;
	for (i = 0; (i < theTrack.numKeys) && (float(theTrack.theFrames[i].startTime) < currentTime) && (float(theTrack.theFrames[i].endTime) < currentTime); i++){
		int startTime = theTrack.theFrames[i].startTime;
		int endTime = theTrack.theFrames[i].endTime;
	}
	i = (i < theTrack.numKeys) ? i : theTrack.numKeys - 1;
	theKeyFrame = &theTrack.theFrames[i];
	step = (currentTime - float(theKeyFrame->startTime)) / float(theKeyFrame->endTime - theKeyFrame->startTime);
	step = (step < 0.0) ? 0.0 : step;
	step = (1.0 < step) ? 1.0 : step;
}

//		{getQuatFromTrackForTime() in oxySourceAnimation.py}

void getQuatFromTrackForKeyFrameAndStep(keyFrame* theKeyFrame, float step, DUALQUAARG(q)){
	if (theKeyFrame == NULL){
		dual_quaternion_unity(DUALQUACOMP(q));
		return;
	}
	//(theAngle_motion, theUvector, theSlide_motion, theMvector) = trackIntervalsDic[(lastTime, nextTime)][1][0]

	//######################################################
	//# calculate the weighted motion from the quaternion at the beginning of the interval

	//currentQuat = None

	//currentQuat = dual_quaternion_product(
	//										dual_Versor(theAngle_motion*step, theUvector, theSlide_motion*step, theMvector),

	DUALQUAVAR(r);
	//dual_Versor(float theta, float ux, float uy, float uz, float s, float mx, float my, float mz, DUALQUAARG(r));
	dual_Versor(theKeyFrame->keyFrameData->theAngle * step, 
		theKeyFrame->keyFrameData->theUvector[0], theKeyFrame->keyFrameData->theUvector[1], theKeyFrame->keyFrameData->theUvector[2],
		theKeyFrame->keyFrameData->theSfactor * step,
		theKeyFrame->keyFrameData->theMvector[0], theKeyFrame->keyFrameData->theMvector[1], theKeyFrame->keyFrameData->theMvector[2],
		DUALQUACOMP(r));

	float* curr = theKeyFrame->keyFrameData->startTransform;
	
	dual_quaternion_product(DUALQUACOMP(r),
		DUALQUAARRAY(curr),
		DUALQUACOMP(q));
	//										trackIntervalsDic[(lastTime, nextTime)][0]
	//										)
	//return currentQuat
}

void getQuatFromTrackForTime(track theTrack, float currentTime, DUALQUAARG(q)){
	keyFrame* myKeyFrame = NULL;
	float theStep = 0.0;
	getIntervalFromTrackForTime(theTrack, currentTime, myKeyFrame, theStep);
	getQuatFromTrackForKeyFrameAndStep(myKeyFrame, theStep, DUALQUACOMP(q));
}

//(1)and calculate global transformation for every bone in hierarchy.An array with global transformations for every bone will be calculated
//		**globalTransforms[] will have the same size as sceneTracks.numTracks (X size of dual quaternion - float[8]) and will be indexed by bone Index

void alocateGlobalArraysForTracks(sceneTracks &theTracks){
	theTracks.globalTransforms = new float[8 * theTracks.numTracks];
}

//		{ getSkeletonForTime() in oxyAnimateMesh.py }
//		** will descend by the tree in struct skeleton

void getSkeletonForTime(skeleton theSkeleton, sceneTracks &theTracks, float currentTime){
	boneStack theBoneStack;
	float localTransform[8];
	int boneIndex = theSkeleton.theRoot->boneNodeIndex;
	float* boneGlobalTransform = &theTracks.globalTransforms[boneIndex * 8];
	getQuatFromTrackForTime(theTracks.theTracks[boneIndex], currentTime, DUALQUAARRAY(boneGlobalTransform));
	theBoneStack.push(theSkeleton.theRoot);
	while (!theBoneStack.empty()){
		boneNode* currentNode = theBoneStack.top();
		theBoneStack.pop();
		float* parentGlobalTransform = &theTracks.globalTransforms[currentNode->boneNodeIndex * 8];

		for (int i = 0; i < currentNode->numChildren; i++){
			boneNode &currentChild = currentNode->children[i];
			getQuatFromTrackForTime(theTracks.theTracks[currentChild.boneNodeIndex], currentTime, DUALQUAARRAY(localTransform));
			boneGlobalTransform = &theTracks.globalTransforms[currentChild.boneNodeIndex * 8];

			//globalTransforms[theChild] = dual_quaternion_product(
			//	globalTransforms[theParent],
			//	childInterpolation
			//	)

			dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(localTransform),
				DUALQUAARRAY(boneGlobalTransform));

			theBoneStack.push(&currentNode->children[i]);
		}
	}
}