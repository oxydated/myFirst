#include "skinFunctions.h"
#include "debugLog.h"
#include <string>
#include <sstream>
#include <Windows.h>

void alocateSkinArraysForTracks(sceneTracks theTracks, skinData &theSkin) {
	theSkin.fromSkinPoseToCurrentTransf = new float[8 * theTracks.numTracks];
}

void transformFromSkinPoseToCurrentPose(skinData &theSkin, sceneTracks theTracks, bool shouldIPrintIt) {

	for (int i = 0; i < theSkin.numBones; i++) {

		float* boneGlobalTransform = &theTracks.globalTransforms[theSkin.skinPoseSkeleton[i].boneIndex * 8];
		float* transformedFromSkin = &theSkin.fromSkinPoseToCurrentTransf[theSkin.skinPoseSkeleton[i].boneIndex * 8];

		oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(theSkin.skinPoseSkeleton[i].skinPose), DUALQUAARRAY(boneGlobalTransform), DUALQUAARRAY(transformedFromSkin));

		float theComplement[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		bool complementIt = false;
		if(false){
			complementIt = true;
			oxyde::DQ::dual_quaternion_complement(DUALQUAARRAY(transformedFromSkin), DUALQUAARRAY(theComplement));

			transformedFromSkin[0] = theComplement[0];
			transformedFromSkin[1] = theComplement[1];
			transformedFromSkin[2] = theComplement[2];
			transformedFromSkin[3] = theComplement[3];
			transformedFromSkin[4] = theComplement[4];
			transformedFromSkin[5] = theComplement[5];
			transformedFromSkin[6] = theComplement[6];
			transformedFromSkin[7] = theComplement[7];
		}
	}
}

//def blendDualQuatFromMesh( theSkinWeights, theVerticesDic, theChoosenBone = theTreeRoot-1 ): 
void blendDualQuatFromMesh2(skinData theSkin, float * vertices, float * normals, float *& blendedVertices, float *& blendedNormals, int numVertices, bool shouldIPrintIt) {
	//blendedVertices = new float[3 * numVertices];
	//    for aVertex in theSkinWeights:

	//oxyde::log::printText(__FUNCTION__);
	oxyde::log::printText(L"vertexBlending = {");
	for (int i = 0; i < numVertices; i++) {
		//        (theWeight, aVertexPos) = theSkinWeights[aVertex]
		//        
		//        vertexBone, vertexWeight = theWeight[0]
		//        
		//        ###################################################
		//        
		//        theBlendedQuat = ((0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0))
		float theBlendedQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		//        
		//        ###################################################
		//
		//        theTotalWeight = 0.0
		float theTotalWeight = 0.0;

		//        for (vertexBone, vertexWeight) in theWeight:

		//std::wstring vertexOutput(L"vertex: ");
		//vertexOutput += std::to_wstring(i) += L"\n";

		float neatTrick0[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		for (int j = 0; j < int(theSkin.boneNumVertAttrib[i]); j++) {
			//            ##############################################################
			//            
			//            (theCos, theSin), theUvector, theSfactor, theMvector = extractDualVersorParameters( fromInitialPoseToCurrentTransf[boneDic[vertexBone][0]] )
			//            isInverted = (theCos < 0.0)
			int boneIndex = int(theSkin.boneIndexesForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])]);

			//(vertexOutput += L"		boneIndex: " )+= std::to_wstring(boneIndex);

			//float weight = theSkin.boneWeightForSkinVertices[8 * (j + int(theSkin.boneOffsetVertAttrib[i]))];
			float weight = theSkin.boneWeightForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])];

			//(vertexOutput += L"  weight: ") += std::to_wstring(weight) += L"\n";

			float *theVersor = &theSkin.fromSkinPoseToCurrentTransf[8 * boneIndex];
			if (j==0) {
				neatTrick0[0] = theVersor[0];
				neatTrick0[1] = theVersor[1];
				neatTrick0[2] = theVersor[2];
				neatTrick0[3] = theVersor[3];
				neatTrick0[4] = theVersor[4];
				neatTrick0[5] = theVersor[5];
				neatTrick0[6] = theVersor[6];
				neatTrick0[7] = theVersor[7];
			}

			float theComplement[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
			//            theVersor = None
			//            if isInverted:
			//                theVersor = dual_quaternion_complement(fromInitialPoseToCurrentTransf[boneDic[vertexBone][0]])
			//            else:

			//if (theVersor[4] < 0.0) {
			//	dual_quaternion_complement(DUALQUAARRAY(theVersor), DUALQUAARRAY(theComplement));

			//	theVersor = theComplement;
			//}


			//                theVersor = fromInitialPoseToCurrentTransf[boneDic[vertexBone][0]]
			//            
			//                                                     dual_quaternion_product_by_scalar(theVersor,vertexWeight)
			float o[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
			oxyde::DQ::dual_quaternion_product_by_scalar(DUALQUAARRAY(theVersor), weight, DUALQUAARRAY(o));

			//            theBlendedQuat = dual_quaternion_sum(    theBlendedQuat, 
			//                                                     dual_quaternion_product_by_scalar(theVersor,vertexWeight)
			//                                                )
			float theSum[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
			oxyde::DQ::dual_quaternion_sum(DUALQUAARRAY(theBlendedQuat),
				DUALQUAARRAY(o),
				DUALQUAARRAY(theSum));

			theBlendedQuat[0] = theSum[0]; theBlendedQuat[4] = theSum[4];
			theBlendedQuat[1] = theSum[1]; theBlendedQuat[5] = theSum[5];
			theBlendedQuat[2] = theSum[2]; theBlendedQuat[6] = theSum[6];
			theBlendedQuat[3] = theSum[3]; theBlendedQuat[7] = theSum[7];
			//            ##############################################################
			//                                                
			//            theTotalWeight = theTotalWeight + vertexWeight
			theTotalWeight = theTotalWeight + weight;
		}

		//oxyde::log::printText(vertexOutput);

		//        theNorm = dual_quaternion_norm(theBlendedQuat)
		float theNorm = oxyde::DQ::dual_quaternion_norm(DUALQUAARRAY(theBlendedQuat));

		//       
		float normalizedBlend[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		oxyde::DQ::dual_quaternion_product_by_scalar(DUALQUAARRAY(theBlendedQuat), 1.0 / theNorm, DUALQUAARRAY(normalizedBlend));

		//		Transforming the vertices

		float vertQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		float px = vertices[3 * i]; float py = vertices[3 * i + 1]; float pz = vertices[3 * i + 2];
		oxyde::DQ::point_quaternion(px, py, pz, DUALQUAARRAY(vertQuat));

		float vertBlendQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		//        DLBVerticesDic[aVertex] = dual_quat_transform_point( dual_quaternion_product_by_scalar(theBlendedQuat,1.0/theNorm) , theVerticesDic[aVertex] )[1][1:]
		oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(normalizedBlend),
			//dual_quat_transform_point(DUALQUAARRAY(theBlendedQuat),
			DUALQUAARRAY(vertQuat),
			DUALQUAARRAY(vertBlendQuat));

		//this is a neat trick
		//oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(neatTrick0),
		//	DUALQUAARRAY(vertQuat),
		//	DUALQUAARRAY(vertBlendQuat));

		// neat trick indeed

		blendedVertices[3 * i] = vertBlendQuat[5];
		blendedVertices[3 * i + 1] = vertBlendQuat[6];
		blendedVertices[3 * i + 2] = vertBlendQuat[7];

		//oxyde::log::printText(L"{");
		//oxyde::log::printNamedParameter(L"vertex", i);
		//oxyde::log::printText(L",");
		//oxyde::log::printPointInSpace(L"skin", px, py, pz);
		//oxyde::log::printText(L",");
		//oxyde::log::printDualQuat(L"neatTrick0", neatTrick0);
		//oxyde::log::printText(L",");
		//oxyde::log::printPointInSpace(L"blendedVertices", blendedVertices[3 * i], blendedVertices[3 * i + 1], blendedVertices[3 * i + 2]);
		//oxyde::log::printText(L"},");

		//if (blendedVertices[3 * i] > 100.0 || blendedVertices[3 * i + 1] > 100.0 || blendedVertices[3 * i + 2] > 100.0 ||
		//	blendedVertices[3 * i] < -100.0 || blendedVertices[3 * i + 1] < -100.0 || blendedVertices[3 * i + 2] < -100.0) {
		//	//oxyde::log::printText(resultString);
		//}

		//		Transforming the vertices normals

		float rotationOnlyQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		oxyde::DQ::extractRotationQuatFromDualQuat(DUALQUAARRAY(normalizedBlend), DUALQUAARRAY(rotationOnlyQuat));

		float normalQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		float nx = normals[3 * i]; float ny = normals[3 * i + 1]; float nz = normals[3 * i + 2];
		oxyde::DQ::vector_quaternion(nx, ny, nz, DUALQUAARRAY(normalQuat));

		float normalBlendQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		oxyde::DQ::dual_quat_transform_point(DUALQUAARRAY(normalizedBlend),
			DUALQUAARRAY(normalQuat),
			DUALQUAARRAY(normalBlendQuat));

		blendedNormals[3 * i] = normalBlendQuat[5];
		blendedNormals[3 * i + 1] = normalBlendQuat[6];
		blendedNormals[3 * i + 2] = normalBlendQuat[7];

	}

	oxyde::log::printText(L"}");
}

void dual_quat_transform_point(float q[], float p[], float pt[]) {
	pt[0] = -2 * q[5] * q[0] + 2 * q[4] * q[1] - 2 * q[7] * q[2] + 2 * q[6] * q[3] + p[1] * (2 * q[1] * q[2] - 2 * q[0] * q[3]) + p[2] * (2 * q[0] * q[2] + 2 * q[1] * q[3]) + p[0] * (q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
	pt[1] = -2 * q[6] * q[0] + 2 * q[7] * q[1] + 2 * q[4] * q[2] - 2 * q[5] * q[3] + p[0] * (2 * q[1] * q[2] + 2 * q[0] * q[3]) + p[2] * (-2 * q[0] * q[1] + 2 * q[2] * q[3]) + p[1] * (q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3]);
	pt[2] = -2 * q[7] * q[0] - 2 * q[6] * q[1] + 2 * q[5] * q[2] + 2 * q[4] * q[3] + p[0] * (-2 * q[0] * q[2] + 2 * q[1] * q[3]) + p[1] * (2 * q[0] * q[1] + 2 * q[2] * q[3]) + p[2] * (q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
}

void blendDualQuatFromMesh(skinData theSkin, float * vertices, float * normals, float *& blendedVertices, float *& blendedNormals, int numVertices, bool shouldIPrintIt) {

	for (int i = 0; i < numVertices; i++) {

		//for (int j = 0; j < int(theSkin.boneNumVertAttrib[i]); j++) {

		//	int boneIndex = int(theSkin.boneIndexesForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])]);

		//	float weight = theSkin.boneWeightForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])];

		//	float *theVersor = &theSkin.fromSkinPoseToCurrentTransf[8 * boneIndex];
		//	if (j == 0) {
		//		neatTrick0[0] = theVersor[0];
		//		neatTrick0[1] = theVersor[1];
		//		neatTrick0[2] = theVersor[2];
		//		neatTrick0[3] = theVersor[3];
		//		neatTrick0[4] = theVersor[4];
		//		neatTrick0[5] = theVersor[5];
		//		neatTrick0[6] = theVersor[6];
		//		neatTrick0[7] = theVersor[7];
		//	}

		//	float theComplement[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		//	float o[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		//	oxyde::DQ::dual_quaternion_product_by_scalar(DUALQUAARRAY(theVersor), weight, DUALQUAARRAY(o));

		//	float theSum[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		//	oxyde::DQ::dual_quaternion_sum(DUALQUAARRAY(theBlendedQuat),
		//		DUALQUAARRAY(o),
		//		DUALQUAARRAY(theSum));

		//	theBlendedQuat[0] = theSum[0]; theBlendedQuat[4] = theSum[4];
		//	theBlendedQuat[1] = theSum[1]; theBlendedQuat[5] = theSum[5];
		//	theBlendedQuat[2] = theSum[2]; theBlendedQuat[6] = theSum[6];
		//	theBlendedQuat[3] = theSum[3]; theBlendedQuat[7] = theSum[7];

		//	theTotalWeight = theTotalWeight + weight;
		//}

		int boneIndex = int(theSkin.boneIndexesForSkinVertices[ int(theSkin.boneOffsetVertAttrib[i])]);

		float weight = theSkin.boneWeightForSkinVertices[int(theSkin.boneOffsetVertAttrib[i])];

		float *theVersor = &theSkin.fromSkinPoseToCurrentTransf[8 * boneIndex];

		float p[] = { vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2] };
		float pt[3];

		dual_quat_transform_point(theVersor, p, pt);

		blendedVertices[3 * i] = pt[0];
		blendedVertices[3 * i + 1] = pt[1];
		blendedVertices[3 * i + 2] = pt[2];

	}
}