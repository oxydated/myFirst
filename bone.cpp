#include <string>
#include <algorithm>
#include "bone.h"
#include "debugLog.h"

namespace oxyde {
	namespace scene {
		
		int bone::rootNodeObject = 0;
		std::vector<dualQuat> bone::boneTransformation(0);
		std::vector<oxyde::geometry::skeletalModifierPtr> bone::modifiers;

		bone::bone(const MSXML2::IXMLDOMNodePtr &theElement)
		{
			nodeObject = oxyde::XML::getIntAttributeFromElement(theElement, "nodeObject");
		}

		void bone::setRootNodeObject(int nodeObject)
		{
			rootNodeObject = nodeObject;
		}

		void bone::setTransformationVector(int sizeOfVector)
		{
			boneTransformation.resize(sizeOfVector);
			dualQuat unit = { 1.,0.,0.,0.,0.,0.,0.,0. };
			std::fill(boneTransformation.begin(), boneTransformation.end(), unit);
		}

		const float * bone::getTransformationData()
		{
			return (float*)boneTransformation.data();
		}

		void bone::printBones()
		{
			for (int i = 0; i < boneTransformation.size(); i++) {
				oxyde::log::printDualQuat(std::wstring(L"bone[ ") + std::to_wstring(i) + std::wstring(L" ]"), boneTransformation[i].data());
			}
		}

		void bone::addModifier(oxyde::geometry::skeletalModifierPtr theModifier)
		{
			modifiers.push_back(theModifier);
		}

		void bone::updateFrame()
		{
			for (auto modifier : modifiers) {
				modifier->updateSkinPose(boneTransformation);
			}
		}
		
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
