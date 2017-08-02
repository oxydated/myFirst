#include "animationFunctions.h"
#include <string>
#include <sstream>
#include <Windows.h>
#include "linAlg.h"
#include "debugLog.h"

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
		oxyde::DQ::dual_quaternion_unity(DUALQUACOMP(q));
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
	oxyde::DQ::dual_Versor(theKeyFrame->keyFrameData->theAngle * step,
		theKeyFrame->keyFrameData->theUvector[0], theKeyFrame->keyFrameData->theUvector[1], theKeyFrame->keyFrameData->theUvector[2],
		theKeyFrame->keyFrameData->theSfactor * step,
		theKeyFrame->keyFrameData->theMvector[0], theKeyFrame->keyFrameData->theMvector[1], theKeyFrame->keyFrameData->theMvector[2],
		DUALQUACOMP(r));

	float* curr = theKeyFrame->keyFrameData->startTransform;
	
	oxyde::DQ::dual_quaternion_product(DUALQUACOMP(r),
		DUALQUAARRAY(curr),
		DUALQUACOMP(q));
	//										trackIntervalsDic[(lastTime, nextTime)][0]
	//										)
	//return currentQuat
	std::wstring outStr;
	std::wostringstream outStream(outStr);

	oxyde::log::printText(__FUNCTION__);
	oxyde::log::printLine();
	oxyde::log::printDualQuat(L"curr", curr);
	oxyde::log::printText("");
	oxyde::log::printDualQuat(L"rTrans", DUALQUACOMP(r));
	oxyde::log::printText("");
	oxyde::log::printDualQuat(L"qEnd", DUALQUACOMP(q));
	oxyde::log::printLine();

	std::wstring resultString(outStream.str());
	OutputDebugString(resultString.c_str());
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

void getSkeletonForTime(skeleton theSkeleton, sceneTracks &theTracks, float currentTime, bool shouldIPrintIt){
	boneStack theBoneStack;
	float localTransform[8];
	int boneIndex = theSkeleton.theRoot->boneNodeIndex;
	float* boneGlobalTransform = &theTracks.globalTransforms[boneIndex * 8];
	getQuatFromTrackForTime(theTracks.theTracks[boneIndex], currentTime, DUALQUAARRAY(boneGlobalTransform));
	theBoneStack.push(theSkeleton.theRoot);
	////////////////////////////// printing it
	TCHAR outputString[400];
	if (true) {
		swprintf(outputString, TEXT("{%f, {\n"), currentTime);
		OutputDebugString(outputString);

		swprintf(outputString, TEXT("{%i, {%f, %f, %f, %f, %f, %f, %f, %f}, {%f, %f, %f, %f, %f, %f, %f, %f}}"),
			boneIndex,
			DUALQUAARRAY(boneGlobalTransform),
			DUALQUAARRAY(boneGlobalTransform)
			);
		OutputDebugString(outputString);
	}
	//////////////////////////////

	oxyde::log::printText(__FUNCTION__);

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

			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(localTransform),
				DUALQUAARRAY(boneGlobalTransform));
			////////////////////////////// printing it
			//if (true) {
			//	swprintf(outputString, TEXT(",\n{%i, {%f, %f, %f, %f, %f, %f, %f, %f}, {%f, %f, %f, %f, %f, %f, %f, %f}}"),
			//		currentChild.boneNodeIndex,
			//		DUALQUAARRAY(localTransform),
			//		DUALQUAARRAY(boneGlobalTransform)
			//	);
			//	OutputDebugString(outputString);
			//}
			oxyde::log::printLine();
			oxyde::log::printNamedParameter(L"currentChild.boneNodeIndex", std::to_wstring(currentChild.boneNodeIndex));
			oxyde::log::printDualQuat(L"parentGlobalTransform", parentGlobalTransform);
			oxyde::log::printDualQuat(L"localTransform", localTransform);
			oxyde::log::printDualQuat(L"boneGlobalTransform", boneGlobalTransform);

			/////////////////// checking bones

			float zeroInLocalSpace[8] = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

			float zeroInGlobalParentTransform[8];
			oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(zeroInLocalSpace),
				DUALQUAARRAY(zeroInGlobalParentTransform));

			float zeroInChildParentTransform[8];
			oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(boneGlobalTransform),
				DUALQUAARRAY(zeroInLocalSpace),
				DUALQUAARRAY(zeroInChildParentTransform));

			float fromChildToParent[3] = { zeroInChildParentTransform[5] - zeroInGlobalParentTransform[5],
				zeroInChildParentTransform[6] - zeroInGlobalParentTransform[6],
				zeroInChildParentTransform[7] - zeroInGlobalParentTransform[7] };

			float distanceFromChildToParent = 0.0;
			oxyde::linAlg::norm(fromChildToParent, &distanceFromChildToParent);
			oxyde::log::printText(std::wstring(L"distance = ") += std::to_wstring(distanceFromChildToParent));

			//////////////////////////////

			theBoneStack.push(&currentNode->children[i]);
		}
	}

	oxyde::log::printLine();

	////////////////////////////// printing it
	if (shouldIPrintIt) {
		swprintf(outputString, TEXT("\n}}, \n"));
		OutputDebugString(outputString);
	}
	//////////////////////////////
}