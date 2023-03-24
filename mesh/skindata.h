#pragma once
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include "../xml/XMLDocument.h"
#include "mesh.h"
#include "dualQuaternionFunctions.h"
#include "skeletalModifier.h"

/*
class skinData{

	For now only one SkinData will be allowed

	shared_ptr<mesh> theMesh;

	std::vector<int, numSkinPoseBones> boneIndex;
	
	//	skinPose bone structure array pointer
	std::vector<std::array<float, 8>, numSkinPoseBones> skinPoseSkeleton;
	
	//	vertex attribute array for offset as defined in 6.4.2.1.1. (float array)
	int *boneOffsetVertAttrib;	//	in int boneOffsetVertAttrib;
	
	//	vertex attribute array for number of bones as defined in 6.4.2.1.2. (float array)
	int *boneNumVertAttrib;	//	in int boneNumVertAttrib;

	//	uniform array for bone index as defined through 6.4.2.1.and 6.4.2.2. (float array)
	int *boneIndexesForSkinVertices;	//	int boneIndexesForSkinVertices

	//	uniform array for bone weight as defined through 6.4.2.1.and 6.4.2.2. (float array)
	float *boneWeightForSkinVertices;	//	float boneWeightForSkinVertices
	
	// transformations from the bone skin pose to current frame pose. will have the same size as sceneTracks.numTracks (X size of dual quaternion - float[8]) and will be indexed by bone Index
	float *fromSkinPoseToCurrentTransf;
	To be implemented as a std::vector for performance sake.
	
	Must add facility to build OpenGL Buffers and Attributes
		provide getData for each vector.
		
	Must know innerwards of Scene (?)
	
	UpdateSkinPose()
		Callback method
	
};
*/

namespace oxyde {
	namespace geometry {

		using dualQuat = oxyde::DQ::dualQuat;

		class skindata: public skeletalModifier {
		protected:
			class notAccessible { public: explicit notAccessible() = default; };

		public:
			skindata(const MSXML2::IXMLDOMElementPtr&, int ID, const notAccessible&);

			const int* getBoneOffsetVertAttrib(size_t &size);
			const int* getBoneNumVertAttrib(size_t &size);

			const int* getBoneIndexesForSkinVertices(size_t &size);
			const float* getBoneWeightForSkinVertices(size_t &size);

			const float* getFromSkinPoseToCurrentTransf(size_t &size);

			void updateSkinPose(std::vector<dualQuat>&);

			static void buildSkindata(const MSXML2::IXMLDOMElementPtr&, std::function< void(skeletalModifierPtr)> forEachNewSkin);

			//test

			const meshPtr& getMesh();


		protected:

			int skinID;
			meshPtr theMesh;

			std::vector<int> boneOffsetVertAttrib;
			std::vector<int> boneNumVertAttrib;

			//std::vector<int> boneIndex;
			std::vector<int> boneIndexesForSkinVertices;
			std::vector<float> boneWeightForSkinVertices;

			std::vector<int> nodeObjects;

			std::vector<dualQuat> skinPoseSkeleton;
			std::vector<dualQuat> fromSkinPoseToCurrentTransf;

		};

		using skindataPtr = std::shared_ptr<skindata>;

	}
}
