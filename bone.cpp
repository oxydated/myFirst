#include <string>
#include <algorithm>
#include <cmath>
#include "bone.h"
#include "debugLog.h"

namespace oxyde {
	namespace scene {
		
		int bone::rootNodeObject = 0;
		std::vector<dualQuat> bone::boneTransformation(0);
		std::vector<std::array<float, 3>> bone::boneJoints(0);
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

			boneJoints.resize(sizeOfVector + 1);
		}

		void bone::transformPointByBoneID(int nodeObject, const std::array<float, 3> &pointToTransform, std::array<float, 3> &pointTransformed) {
			dualQuat pointQuatOriginal = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };
			dualQuat pointQuatTransformed = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

			oxyde::DQ::point_quaternion(pointToTransform[0], pointToTransform[1], pointToTransform[2], DUALQUAARRAY(pointQuatOriginal));

			oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(boneTransformation[nodeObject]),
				DUALQUAARRAY(pointQuatOriginal),
				DUALQUAARRAY(pointQuatTransformed));

			pointTransformed[0] = pointQuatTransformed[5];
			pointTransformed[1] = pointQuatTransformed[6];
			pointTransformed[2] = pointQuatTransformed[7];
		}

		void bone::setSkeletonJointsPosition() {
			std::array<float, 3> pointTransformed;
			dualQuat zeroQuat = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };
			dualQuat jointQuat = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

			std::array<float, 3> sum = { 0., 0., 0. };

			float count = 0.;

			for (int i = 0; i < boneTransformation.size(); i++) {
				if (i != rootNodeObject) {
					count += 1.;

					oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(boneTransformation[i]),
						DUALQUAARRAY(zeroQuat),
						DUALQUAARRAY(jointQuat));

					boneJoints[i] = { jointQuat[5], jointQuat[6], jointQuat[7] };

					sum[0] += jointQuat[5];
					sum[1] += jointQuat[6];
					sum[2] += jointQuat[7];
				}
			}

			pointTransformed[0] = sum[0] / count;
			pointTransformed[1] = sum[1] / count;
			pointTransformed[2] = sum[2] / count;

			boneJoints[boneTransformation.size()] = pointTransformed;

		}

		float bone::getSkeletonCenterAndSize(std::array<float, 3> &pointTransformed) {
			//dualQuat zeroQuat = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };
			//dualQuat jointQuat = { 1., 0. , 0. , 0. , 0. , 0. , 0. , 0. };

			//std::array<float, 3> sum = { 0., 0., 0. };

			//float count = 0.;

			//for (int i = 0; i < boneTransformation.size(); i++) {
			//	if (i != rootNodeObject) {
			//		count += 1.;

			//		oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(boneTransformation[i]),
			//			DUALQUAARRAY(zeroQuat),
			//			DUALQUAARRAY(jointQuat));

			//		boneJoints[i] = { jointQuat[5], jointQuat[6], jointQuat[7] };

			//		sum[0] += jointQuat[5];
			//		sum[1] += jointQuat[6];
			//		sum[2] += jointQuat[7];
			//	}
			//}

			//pointTransformed[0] = sum[0] / count;
			//pointTransformed[1] = sum[1] / count;
			//pointTransformed[2] = sum[2] / count;

			pointTransformed = boneJoints[boneTransformation.size()];

			float radius = 0.;
			for (int i = 0; i < boneJoints.size(); i++) {
				if (i != rootNodeObject) {
					float dist = std::sqrt(
						std::pow(boneJoints[i][0] - pointTransformed[0], 2) + 
						std::pow(boneJoints[i][1] - pointTransformed[1], 2) +
						std::pow(boneJoints[i][2] - pointTransformed[2], 2)
					);
					radius = radius < dist ? dist : radius;
				}
			}

			return radius;
		}

		int bone::getNumJoints() {
			return boneJoints.size();
		}

		std::array<float, 3> bone::getJointPositionAtIndex(int i)
		{
			return (i < boneJoints.size() && 0 <= i) ? boneJoints[i] : std::array<float, 3>();
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
			setSkeletonJointsPosition();
		}

		void bone::reset()
		{
			boneTransformation.clear();
			boneTransformation.shrink_to_fit();

			modifiers.clear();
			modifiers.shrink_to_fit();
		}

		 oxyde::geometry::skeletalModifierPtr  bone::getModifierAtIndex(int index)
		{
			if (index < 0 || modifiers.size() <= index) {
				return oxyde::geometry::skeletalModifierPtr(nullptr);
			}
			return modifiers[index];
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
