#pragma once

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "XMLDocument.h"
#include "dualQuaternionMath.h"
#include "dualQuaternionFunctions.h"
#include "skeletalModifier.h"

/*
Bone class (abstract)
	virtual updateTransform();
		Called by the Observer
		
	virtual std::vector<int> getListOfObservedBoneID();
		return bones that are observed by this bone.
		int nodeObject used as offset of transformation vector;
		Order depends on the concrete algorithm
		
NOTES:

Bone global transformation data is accessible to its observers's bones. Once its observers receive an UPDATED message, 
	their respective bones will update their transformations reading this data directly. So every Bone object keeps its subjects
	
Bone objects should be constructed by an Abstract Factory. Concrete Factory should be selected by XML data.
*/

namespace oxyde {
	namespace scene {

		using dualQuat = oxyde::DQ::dualQuat;

		class bone {
		public:
			virtual void updateTransform() = 0;
			const std::vector<int>& getListOfObservedBones() { return listOfObservedBones; }
			bone(const MSXML2::IXMLDOMNodePtr&);
			bone() = delete;

			static void setRootNodeObject(int nodeObject);
			static void setTransformationVector(int sizeOfVector);
			static const float* getTransformationData();
			static void printBones();
			static void addModifier(oxyde::geometry::skeletalModifierPtr);
			static void updateFrame();
			static void reset();

			static void transformPointByBoneID(int nodeObject, const std::array<float, 3>& pointToTransform, std::array<float, 3>& pointTransformed);
			static void getSkeletonCenter(std::array<float, 3>& pointTransformed);

			//test
			static oxyde::geometry::skeletalModifierPtr getModifierAtIndex(int index);

		protected:
			std::vector<int> listOfObservedBones;
			int nodeObject;

			static int rootNodeObject;
			static std::vector<dualQuat> boneTransformation;
			static std::vector<oxyde::geometry::skeletalModifierPtr> modifiers;
		};

		using bonePtr = std::shared_ptr<bone>;

		namespace boneFactory {

			using boneConcreteFactoryFunction = std::function<bonePtr(const MSXML2::IXMLDOMNodePtr&)>;

			class boneFactory {
			private:
				boneFactory() = delete;
				static std::map<std::wstring, boneConcreteFactoryFunction> factoryMap;

			public:
				static void registerBoneConcreteFactory(const std::wstring&, boneConcreteFactoryFunction);
				static bonePtr createBone(const MSXML2::IXMLDOMNodePtr& key);
			};

		}

	}
}
