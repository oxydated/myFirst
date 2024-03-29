#include "keyframeBone.h"
#include "debugLog.h"
#include <set>

// Will register this bone class's factory

namespace {
	class executeBeforeMain {
	public:
		explicit executeBeforeMain() {
			oxyde::scene::keyframeBone::registerKeyframeBoneFactory();
		}
	};

	executeBeforeMain instance = executeBeforeMain();
}

namespace oxyde {
	namespace scene {

		keyframeBone::keyframeBone(const MSXML2::IXMLDOMNodePtr& theNode, const MSXML2::IXMLDOMElementPtr& dualQuatTrackElement, const notAccessible&) :
			bone(MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../.."))), 
			localTransformTrack(dualQuatTrackElement)
		{
			// get the parent node element
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);

				// need to set parentBoneID
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");

				//// get animation data
				//std::wstring queryKeyframes = L"./dualQuatTrack/dualQuatKey";
				//MSXML2::IXMLDOMNodeListPtr dualQuatKeyframes = theNode->selectNodes(queryKeyframes.data());
				//for (int i = 0; i < dualQuatKeyframes->length; i++) {
				//	MSXML2::IXMLDOMElementPtr dualQuatKeyframeElement = MSXML2::IXMLDOMElementPtr(dualQuatKeyframes->item[i]);
				//	if (dualQuatKeyframeElement) {
				//		track.push_back(dualQuatKeyframe(dualQuatKeyframeElement));
				//	}
				//}
				//trackSize = track.size();

				//	set observed bones
				std::set<int> setToKeepObservedIDsUnique({ rootNodeObject, parentBoneID });
				listOfObservedBones = std::vector<int>(setToKeepObservedIDsUnique.begin(), setToKeepObservedIDsUnique.end());
				listOfObservedBones.shrink_to_fit();
			}

		}

		void keyframeBone::updateTransform()
		{
			// To transform from local to global coordinates
			dualQuat localTransform;
			dualQuat &boneGlobalTransform = boneTransformation[nodeObject];
			dualQuat &parentGlobalTransform = boneTransformation[parentBoneID];

			// Calculate local transformation

			// int inInterval = -1;
			// while ((inInterval != 0) && (currentKeyframe < trackSize)) {
			// inInterval = track[currentKeyframe].getInterpolatedQuaternion(localTransform);
			// currentKeyframe += inInterval;
			// }

			// if (currentKeyframe >= trackSize) {
			// track[trackSize - 1].getEndTransformation(localTransform);
			// }
			dualQuat currentQuat = localTransformTrack.getCurrentValue();
			localTransform = { currentQuat[0], currentQuat[1], currentQuat[2], currentQuat[3],currentQuat[4], currentQuat[5], currentQuat[6], currentQuat[7] };

			// To keep track of local transformation
			boneLocalTransformation[nodeObject] = localTransform;

			// Transforming from local to global coordinates
			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(localTransform),
				DUALQUAARRAY(boneGlobalTransform));

			//oxyde::log::printText(L"update bone: " + std::to_wstring(this->nodeObject));
		}

		bonePtr keyframeBone::createKeyframeBone(const MSXML2::IXMLDOMNodePtr & theNode)
		{
			// To make sure there is actual animation data

			MSXML2::IXMLDOMElementPtr dualQuatTrackElement = (MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./dualQuatTrack")));

			if (nullptr != dualQuatTrackElement) {
				return std::make_shared<keyframeBone>(theNode, dualQuatTrackElement, notAccessible());
			}

			return nullptr;
		}

		void keyframeBone::registerKeyframeBoneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"keyFrames", createKeyframeBone);
		}

	}
}
