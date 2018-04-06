#include "keyframeBone.h"

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

		keyframeBone::keyframeBone(const MSXML2::IXMLDOMNodePtr & theNode, const notAccessible&):currentKeyframe(0)
		{
			MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");
			}

		}

		void keyframeBone::updateTransform()
		{
			// to implement
		}

		bonePtr keyframeBone::createKeyframeBone(const MSXML2::IXMLDOMNodePtr & theNode)
		{
			return std::make_shared<keyframeBone>(theNode, notAccessible());
		}

		void keyframeBone::registerKeyframeBoneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"keyFrames", createKeyframeBone);
		}

	}
}
