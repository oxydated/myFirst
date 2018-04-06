#include "bone.h"
#include <string>

namespace oxyde {
	namespace scene {
		namespace boneFactory {

			std::map<std::wstring, boneConcreteFactoryFunction> boneFactory::factoryMap;

			void boneFactory::registerBoneConcreteFactory(const std::wstring &key, boneConcreteFactoryFunction theFactory)
			{
				factoryMap[key] = theFactory;
			}

			bonePtr boneFactory::createBone(const MSXML2::IXMLDOMNodePtr & key)
			{
				std::wstring test(key->xml);

				bonePtr returnBone = nullptr;
				std::wstring retrieveAnimationType = L"./animaton/*[1]";
				MSXML2::IXMLDOMNodePtr animationTypeNode = key->selectSingleNode(retrieveAnimationType.data());
				MSXML2::IXMLDOMElementPtr animationTypeElement = MSXML2::IXMLDOMElementPtr(animationTypeNode);
				if (animationTypeElement) {
					std::wstring tagName(animationTypeElement->GettagName());
					returnBone = (factoryMap.at(tagName))(animationTypeNode);
				}
				return returnBone;
			}

		}
	}
}
