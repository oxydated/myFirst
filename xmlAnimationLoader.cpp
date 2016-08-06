#include "xmlAnimationLoader.h"

// load interpolation structure
HRESULT getInterpolationData(IXMLDOMDocument3* theDocument, IXMLDOMNode* dualQuatKeyElementNode, interpolData &theInterpolData){
	HRESULT hr = S_OK;
	IXMLDOMNode* startingDualQuatNode = NULL;
	IXMLDOMNode* interpolationParamNode = NULL;

	//theInterpolData = new interpolData;

	//		<startingDualQuat dqs = "0.0" dqx = "0.0" dqy = "0.0" dqz = "0.0" qs = "0.495593461227" qx = "-0.504368082999" qy = "-0.504368722031" qz = "-0.495592726214" / >
	hr = getNodeElementFromNode(dualQuatKeyElementNode, _bstr_t("./startingDualQuat"), &startingDualQuatNode);

	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "qs", theInterpolData.startTransform[0]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "qx", theInterpolData.startTransform[1]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "qy", theInterpolData.startTransform[2]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "qz", theInterpolData.startTransform[3]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "dqs", theInterpolData.startTransform[4]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "dqx", theInterpolData.startTransform[5]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "dqy", theInterpolData.startTransform[6]);
	hr = getFloatAttributeFromElementInNode(startingDualQuatNode, "dqz", theInterpolData.startTransform[7]);

	//		<interpolationParam angle = "0.0183527278104" mx = "-0.0" my = "-0.0" mz = "-0.0" slide = "0.0" ux = "0.999999999996" uy = "-2.86176145063e-06" uz = "-7.71845732628e-07" / >
	hr = getNodeElementFromNode(dualQuatKeyElementNode, _bstr_t("./interpolationParam"), &interpolationParamNode);

	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "angle", theInterpolData.theAngle);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "ux", theInterpolData.theUvector[0]);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "uy", theInterpolData.theUvector[1]);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "uz", theInterpolData.theUvector[2]);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "slide", theInterpolData.theSfactor);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "mx", theInterpolData.theMvector[0]);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "my", theInterpolData.theMvector[1]);
	hr = getFloatAttributeFromElementInNode(interpolationParamNode, "mz", theInterpolData.theMvector[2]);

	return hr;
}

// load keyFrame structure
HRESULT getKeyFrameData(IXMLDOMDocument3* theDocument, IXMLDOMNode* dualQuatKeyElementNode, keyFrame &theKeyFrame){
	HRESULT hr = S_OK;

	//theKeyFrame = new keyFrame;
	hr = getIntAttributeFromElementInNode(dualQuatKeyElementNode, "startTime", theKeyFrame.startTime);
	hr = getIntAttributeFromElementInNode(dualQuatKeyElementNode, "endTime", theKeyFrame.endTime);
	theKeyFrame.keyFrameData = new interpolData;
	hr = getInterpolationData(theDocument, dualQuatKeyElementNode, theKeyFrame.keyFrameData[0]);
	return hr;

//	<dualQuatKey endTime = "320" startTime = "160">
//		<startingDualQuat dqs = "0.0" dqx = "0.0" dqy = "0.0" dqz = "0.0" qs = "0.495593461227" qx = "-0.504368082999" qy = "-0.504368722031" qz = "-0.495592726214" / >
//		<interpolationParam angle = "0.0183527278104" mx = "-0.0" my = "-0.0" mz = "-0.0" slide = "0.0" ux = "0.999999999996" uy = "-2.86176145063e-06" uz = "-7.71845732628e-07" / >
//	< / dualQuatKey>
}

bool compareKeyFrame(keyFrame A, keyFrame B){
	return (B.startTime > A.startTime);
}

HRESULT getTrackData(IXMLDOMDocument3* theDocument, IXMLDOMNode* theElementNode , track &theTrack){
	HRESULT hr = S_OK;
	//theTrack = new track;
	//			<dualQuatTrack numKeys = "36">

	IXMLDOMNode* dualQuatTrackElementNode = NULL;
	hr = theElementNode->selectSingleNode(_bstr_t("./animaton/keyFrames/dualQuatTrack"), &dualQuatTrackElementNode);

	IXMLDOMNodeList* dualQuatKeyNodesList = NULL;
	hr = dualQuatTrackElementNode->selectNodes(_bstr_t("./dualQuatKey"), &dualQuatKeyNodesList);
	long numKeys = 0;
	hr = dualQuatKeyNodesList->get_length(&numKeys);
	theTrack.numKeys = numKeys;
	theTrack.theFrames = new keyFrame[numKeys];
	keyFrameVector theKeyFrameVector;

	for (long i = 0; i < numKeys; i++){
		IXMLDOMNode* dualQuatKeyElementNode = NULL;
		hr = dualQuatKeyNodesList->get_item(i, &dualQuatKeyElementNode);
		keyFrame currentKey;
		hr = getKeyFrameData(theDocument, dualQuatKeyElementNode, currentKey);
		theKeyFrameVector.push_back(currentKey);
	}
	std::sort(theKeyFrameVector.begin(), theKeyFrameVector.end(), compareKeyFrame);
	for (long i = 0; i < numKeys; i++){
		theTrack.theFrames[i] = theKeyFrameVector[i];
	}
	theKeyFrameVector.clear();
	return hr;
}

HRESULT getAllSceneTracks(IXMLDOMDocument3* theDocument, sceneTracks &allTracks){
	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;

	_bstr_t nodesElementsQuery = _bstr_t("./scene//node");
	hr = theDocument->get_documentElement(&theRoot);

	IXMLDOMNodeList* sceneNodesList = NULL;
	hr = theRoot->selectNodes(nodesElementsQuery, &sceneNodesList);
	long numNodes = 0;
	hr = sceneNodesList->get_length(&numNodes);
	allTracks.numTracks = numNodes;
	allTracks.theTracks = new track[numNodes];

	for (long i = 0; i < numNodes; i++){
		IXMLDOMNode* currentSceneNode = NULL;
		hr = sceneNodesList->get_item(i, &currentSceneNode);
		if (hasDualQuatTrackNode(currentSceneNode)){
			int theNodeIndex = 0;
			hr = getIntAttributeFromElementInNode(currentSceneNode, "nodeObject", theNodeIndex);
			hr = getTrackData(theDocument, currentSceneNode, allTracks.theTracks[theNodeIndex]);
		}
		else {
			int theNodeIndex = 0;
			hr = getIntAttributeFromElementInNode(currentSceneNode, "nodeObject", theNodeIndex);
			allTracks.theTracks[theNodeIndex].numKeys = 0;
			allTracks.theTracks[theNodeIndex].theFrames = NULL;
		}
	}
	return hr;

}

bool hasDualQuatTrackNode(IXMLDOMNode* theElementNode){
	HRESULT hr = S_OK;
	IXMLDOMNode* theTrackNode = NULL;
	hr = theElementNode->selectSingleNode(_bstr_t("./animaton/keyFrames/dualQuatTrack"), &theTrackNode);
	return hr == S_OK;
}

HRESULT getSkeletonData(IXMLDOMDocument3* theDocument, IXMLDOMNode* theRootElementNode, skeleton &theSkeleton){
	HRESULT hr = S_OK;
	boneElemStack theBoneElemStack;

	theSkeleton.theRoot = NULL;
	int numBones = 0;

	if (hasDualQuatTrackNode(theRootElementNode)){

		theSkeleton.theRoot = new boneNode;
		boneElemPair theCurrentPair = boneElemPair(theRootElementNode, theSkeleton.theRoot);
		theBoneElemStack.push(theCurrentPair);
		int boneCount = 0;

		while (!theBoneElemStack.empty()){
			boneCount++;

			////// get stack top
			theCurrentPair = theBoneElemStack.top();
			theBoneElemStack.pop();

			////// get node Index
			int theNodeIndex = 0;
			hr = getIntAttributeFromElementInNode(theCurrentPair.first, "nodeObject", theNodeIndex);
			theCurrentPair.second->boneNodeIndex = theNodeIndex;
			
			////// get node children
			nodeVector validChildren;
			IXMLDOMNodeList* theCurrentNodeChildren = NULL;
			hr = theCurrentPair.first->selectNodes(_bstr_t("./node"), &theCurrentNodeChildren);
			long numChildren = 0;
			hr = theCurrentNodeChildren->get_length(&numChildren);

			for (long i = 0; i < numChildren; i++){
				IXMLDOMNode* aCurrentChildNode = NULL;
				hr = theCurrentNodeChildren->get_item(i, &aCurrentChildNode);
				if (hasDualQuatTrackNode(aCurrentChildNode)) validChildren.push_back(aCurrentChildNode);
			}		

			////// build pairs and clear the vector
			theCurrentPair.second->numChildren = validChildren.size();
			theCurrentPair.second->children = new boneNode[theCurrentPair.second->numChildren];

			for (int i = 0; i < theCurrentPair.second->numChildren; i++){
			////// push pairs to stack, if any
				theBoneElemStack.push(boneElemPair(validChildren[i], &(theCurrentPair.second->children[i])));
			}
			validChildren.clear();
		}
		theSkeleton.numBones = boneCount;
	}

	return hr;
}

EXTERN_C void xmlLoadSkeleton(skeleton &theSkeleton){
	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* theRootElementNode = NULL;

	IXMLDOMDocument3* theDocument = loadModelDocument("processed.OXY");

	_bstr_t skeletonElementQuery = _bstr_t("./scene//node[@nodeName='Bip001']");
	hr = theDocument->get_documentElement(&theRoot);

	hr = theRoot->selectSingleNode(skeletonElementQuery, &theRootElementNode);


	getSkeletonData(theDocument, theRootElementNode, theSkeleton);
}

EXTERN_C void xmlLoadTracks(sceneTracks &theSceneTracks){
	HRESULT hr = S_OK;

	IXMLDOMDocument3* theDocument = loadModelDocument("processed.OXY");

	hr = getAllSceneTracks(theDocument, theSceneTracks);

}
