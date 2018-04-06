#pragma once
#include <memory>
#include <vector>
#include <array>
#include "XMLDocument.h"
#include "mesh.h"

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

		using dualQuat = std::array<float, 8>;

		class skindata {

		public:
			skindata(const MSXML2::IXMLDOMElementPtr&);

			const int* getBoneOffsetVertAttrib();

			const int* getBoneNumVertAttrib();

			const int* getBoneIndexesForSkinVertices();

			const float* getBoneWeightForSkinVertices();

			const float* getFromSkinPoseToCurrentTransf();

			void updateSkinPose();


		protected:
			std::vector<dualQuat> skinPoseSkeleton;

			std::vector<dualQuat> fromSkinPoseToCurrentTransf;

			meshPtr theMesh;

			std::vector<int> boneIndex;

			std::vector<dualQuat> skinPoseSkeleton;

			std::vector<int> boneOffsetVertAttrib;

			std::vector<int> boneNumVertAttrib;

			std::vector<int> boneIndexesForSkinVertices;

			std::vector<float> boneWeightForSkinVertices;

			std::vector<dualQuat> fromSkinPoseToCurrentTransf;

		};

		using skindataPtr = std::shared_ptr<skindata>;

	}
}
