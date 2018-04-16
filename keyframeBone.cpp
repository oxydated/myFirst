#include "keyframeBone.h"
#include "debugLog.h"
#include <set>

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

		keyframeBone::keyframeBone(const MSXML2::IXMLDOMNodePtr & theNode, const notAccessible&) :bone(MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../.."))), currentKeyframe(0)
		{
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");
				std::wstring queryKeyframes = L"./dualQuatTrack/dualQuatKey";
				MSXML2::IXMLDOMNodeListPtr dualQuatKeyframes = theNode->selectNodes(queryKeyframes.data());
				for (int i = 0; i < dualQuatKeyframes->length; i++) {
					MSXML2::IXMLDOMElementPtr dualQuatKeyframeElement = MSXML2::IXMLDOMElementPtr(dualQuatKeyframes->item[i]);
					if (dualQuatKeyframeElement) {
						track.push_back(dualQuatKeyframe(dualQuatKeyframeElement));
					}
				}
				std::set<int> setToKeepObservedIDsUnique({ rootNodeObject, parentBoneID });
				listOfObservedBones = std::vector<int>(setToKeepObservedIDsUnique.begin(), setToKeepObservedIDsUnique.end());
				listOfObservedBones.shrink_to_fit();
			}

		}

		void keyframeBone::updateTransform()
		{			
			// being implemented
			dualQuat localTransform;
			dualQuat &boneGlobalTransform = boneTransformation[nodeObject];
			dualQuat &parentGlobalTransform = boneTransformation[parentBoneID];

			int inInterval = -1;
			while (inInterval != 0) {
				inInterval = track[currentKeyframe].getInterpolatedQuaternion(localTransform);
				currentKeyframe += inInterval;
			}

			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(localTransform),
				DUALQUAARRAY(boneGlobalTransform));

			oxyde::log::printText(L"update bone: " + std::to_wstring(this->nodeObject));
		}

		bonePtr keyframeBone::createKeyframeBone(const MSXML2::IXMLDOMNodePtr & theNode)
		{
			std::wstring queryForAnyChild(L"./*");
			MSXML2::IXMLDOMNodeListPtr theChildList = theNode->selectNodes(queryForAnyChild.data());

			if (theChildList->length > 0) {
				return std::make_shared<keyframeBone>(theNode, notAccessible());
			}

			return nullptr;
		}

		void keyframeBone::registerKeyframeBoneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"keyFrames", createKeyframeBone);
		}

	}
}
