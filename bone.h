#pragma once

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "XMLDocument.h"

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

		class bone {
		public:
			virtual void updateTransform() = 0;
			const std::vector<int>& getListOfObservedBones() { return listOfObservedBones; }

		protected:
			std::vector<int> listOfObservedBones;
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
