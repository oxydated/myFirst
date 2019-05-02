#include <set>
#include "debugLog.h"
#include "lookAtBone.h"

// Will register this bone class's factory

namespace {
	class executeBeforeMain {
	public:
		explicit executeBeforeMain() {
			oxyde::scene::lookAtbone::registerlookAtboneFactory();
		}
	};

	executeBeforeMain instance = executeBeforeMain();
}

namespace oxyde {
	namespace scene {

		lookAtbone::lookAtbone(const MSXML2::IXMLDOMNodePtr &theNode, 
			MSXML2::IXMLDOMElementPtr in_RollAngle, 
			MSXML2::IXMLDOMElementPtr in_XPosition, 
			MSXML2::IXMLDOMElementPtr in_YPosition, 
			MSXML2::IXMLDOMElementPtr in_ZPosition, 
			const notAccessible &) :
			bone(MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../.."))),
			RollAngle(in_RollAngle),
			XPosition(in_XPosition),
			YPosition(in_YPosition),
			ZPosition(in_ZPosition)
		{
			// get the parent node element
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);

				// need to set parentBoneID
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");

				// get animation data (done in the track class)

				/*std::wstring queryKeyframes = L"./dualQuatTrack/dualQuatKey";
				MSXML2::IXMLDOMNodeListPtr dualQuatKeyframes = theNode->selectNodes(queryKeyframes.data());
				for (int i = 0; i < dualQuatKeyframes->length; i++) {
				MSXML2::IXMLDOMElementPtr dualQuatKeyframeElement = MSXML2::IXMLDOMElementPtr(dualQuatKeyframes->item[i]);
				if (dualQuatKeyframeElement) {
				track.push_back(dualQuatKeyframe(dualQuatKeyframeElement));
				}
				}
				trackSize = track.size();*/

				//	set observed bones
				std::set<int> setToKeepObservedIDsUnique({ rootNodeObject, parentBoneID });
				listOfObservedBones = std::vector<int>(setToKeepObservedIDsUnique.begin(), setToKeepObservedIDsUnique.end());
				listOfObservedBones.shrink_to_fit();
			}

		}

		void lookAtbone::updateTransform()
		{
			// To transform from local to global coordinates
			//dualQuat localTransform;
			dualQuat &boneGlobalTransform = boneTransformation[nodeObject];
			dualQuat &parentGlobalTransform = boneTransformation[parentBoneID];

			// Calculate Roll Angle

			currentRollAngle = RollAngle.getCurrentValue();

			// Calculate local transformation

			float XPos = XPosition.getCurrentValue();
			float YPos = YPosition.getCurrentValue();
			float ZPos = ZPosition.getCurrentValue();

			//float XRot = XRotation.getCurrentValue();
			//float YRot = YRotation.getCurrentValue();
			//float ZRot = ZRotation.getCurrentValue();

			// Calculate Rotation
			//dualQuat rotationQuat;
			//quatFromEuler(XRot, YRot, ZRot, DUALQUAARRAY(rotationQuat));

			// Calculate Position
			dualQuat translationQuat;
			oxyde::DQ::translation_quaternion(XPos, YPos, ZPos, DUALQUAARRAY(translationQuat));

			// Multiply Rotation by Position to find local transformation
			// MultiQuat[oxyTranslationQuat, oxyQuat]
			//oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(translationQuat),
			//	DUALQUAARRAY(rotationQuat),
			//	DUALQUAARRAY(localTransform));


			// To keep track of local transformation
			//boneLocalTransformation[nodeObject] = localTransform;
			boneLocalTransformation[nodeObject] = translationQuat;

			// Transforming from local to global coordinates
			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(translationQuat),
				DUALQUAARRAY(boneGlobalTransform));

			//oxyde::log::printText(L"update bone: " + std::to_wstring(this->nodeObject));
		}

		bonePtr lookAtbone::createlookAtbone(const MSXML2::IXMLDOMNodePtr &theNode)
		{
			// To make sure there is actual animation data

			MSXML2::IXMLDOMElementPtr XPosition = (MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./Position/XPosition/BezierTrack")));
			MSXML2::IXMLDOMElementPtr YPosition = (MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./Position/YPosition/BezierTrack")));
			MSXML2::IXMLDOMElementPtr ZPosition = (MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./Position/ZPosition/BezierTrack")));

			MSXML2::IXMLDOMElementPtr RollAngle = (MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./RollAngle/BezierTrack")));

			if ((nullptr != XPosition) &&
				(nullptr != YPosition) &&
				(nullptr != ZPosition) &&
				(nullptr != RollAngle)) {
				return std::make_shared<lookAtbone>(theNode,
					RollAngle,
					XPosition, YPosition, ZPosition,
					notAccessible());
			}
			return nullptr;
		}

		void lookAtbone::registerlookAtboneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"LookAt", createlookAtbone);
		}

	}
}