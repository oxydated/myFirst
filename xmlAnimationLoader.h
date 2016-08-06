#ifndef __XMLANIMATIONLOADER_H__
#define __XMLANIMATIONLOADER_H__

#include "animationFunctions.h"
#include "xmlUtilityFunctions.h"
#include <vector>
#include <utility>
#include <stack>
#include <algorithm>

typedef std::vector<keyFrame> keyFrameVector;

typedef std::pair<IXMLDOMNode*, boneNode*> boneElemPair;
typedef std::stack < boneElemPair > boneElemStack;

typedef std::vector<IXMLDOMNode*> nodeVector;
typedef nodeVector::iterator nodeIterator;

		// load interpolation structure
HRESULT getInterpolationData(IXMLDOMDocument3* theDocument, IXMLDOMNode* dualQuatKeyElementNode, interpolData &theInterpolData);

		// load keyFrame structure
HRESULT getKeyFrameData(IXMLDOMDocument3* theDocument, IXMLDOMNode* dualQuatKeyElementNode, keyFrame &theKeyFrame);

bool hasDualQuatTrackNode(IXMLDOMNode* theElementNode);

HRESULT getTrackData(IXMLDOMDocument3* theDocument, IXMLDOMNode* theElementNode, track &theTrack);

HRESULT getAllSceneTracks(IXMLDOMDocument3* theDocument, sceneTracks &allTracks);

		// load boneNode structure
HRESULT getBoneNodeData(IXMLDOMDocument3* theDocument, IXMLDOMNode* treeRootElementNode, boneNode &theRoot, int &boneCount);

		// build the hierarchy tree
HRESULT getSkeletonData(IXMLDOMDocument3* theDocument, IXMLDOMNode* theRootElementNode, skeleton &theSkeleton);

EXTERN_C void xmlLoadSkeleton(skeleton &theSkeleton);

EXTERN_C void xmlLoadTracks(sceneTracks &theSceneTracks);

#endif