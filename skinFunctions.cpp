#include "skinFunctions.h"
#include <Windows.h>

void alocateSkinArraysForTracks(sceneTracks theTracks, skinData &theSkin) {
	theSkin.fromSkinPoseToCurrentTransf = new float[8 * theTracks.numTracks];
}

void transformFromSkinPoseToCurrentPose(skinData &theSkin, sceneTracks theTracks, bool shouldIPrintIt) {
	TCHAR outputString[800];
	TCHAR boolString[10];

	//for aKey in initialSkinPose :
	//	if aKey in theLinks :
	//		fromInitialPoseToCurrentTransf[aKey] = transformFromSourceToDestinationAxis(initialSkinPose[aKey], globalTransforms[aKey])
	if (shouldIPrintIt) {
		swprintf(outputString, TEXT("(* {boneIndex , Boolean, {transformedFromSkin quat}, {boneGlobalTransform quat}, {skinPoseSkeleton quat}} *)\n"));
		OutputDebugString(outputString);
	}

	for (int i = 0; i < theSkin.numBones; i++) {

		float* boneGlobalTransform = &theTracks.globalTransforms[theSkin.skinPoseSkeleton[i].boneIndex * 8];
		float* transformedFromSkin = &theSkin.fromSkinPoseToCurrentTransf[theSkin.skinPoseSkeleton[i].boneIndex * 8];

		transformFromSourceToDestinationAxis(DUALQUAARRAY(theSkin.skinPoseSkeleton[i].skinPose), DUALQUAARRAY(boneGlobalTransform), DUALQUAARRAY(transformedFromSkin));


		float theComplement[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		bool complementIt = false;
		//if(false){
		//if (transformedFromSkin[0] < 0.0 || transformedFromSkin[4] > 2.0) {
		if (transformedFromSkin[0] < 0.0 ) {
		//if (transformedFromSkin[4] > 0.0001) {
			complementIt = true;
			dual_quaternion_complement(DUALQUAARRAY(transformedFromSkin), DUALQUAARRAY(theComplement));

			transformedFromSkin[0] = theComplement[0];
			transformedFromSkin[1] = theComplement[1];
			transformedFromSkin[2] = theComplement[2];
			transformedFromSkin[3] = theComplement[3];
			transformedFromSkin[4] = theComplement[4];
			transformedFromSkin[5] = theComplement[5];
			transformedFromSkin[6] = theComplement[6];
			transformedFromSkin[7] = theComplement[7];
		}
		if (shouldIPrintIt) {

			if (complementIt) {
				swprintf(boolString, TEXT("True\0"));
			}
			else {
				swprintf(boolString, TEXT("False\0"));
			}

			int textOffset = swprintf(outputString, TEXT("{%i, %s, {%f, %f, %f, %f, %f, %f, %f, %f}, {%f, %f, %f, %f, %f, %f, %f, %f}, {%f, %f, %f, %f, %f, %f, %f, %f}}"),
				theSkin.skinPoseSkeleton[i].boneIndex,
				boolString,
				transformedFromSkin[0],
				transformedFromSkin[1],
				transformedFromSkin[2],
				transformedFromSkin[3],
				transformedFromSkin[4],
				transformedFromSkin[5],
				transformedFromSkin[6],
				transformedFromSkin[7],

				boneGlobalTransform[0],
				boneGlobalTransform[1],
				boneGlobalTransform[2],
				boneGlobalTransform[3],
				boneGlobalTransform[4],
				boneGlobalTransform[5],
				boneGlobalTransform[6],
				boneGlobalTransform[7],

				theSkin.skinPoseSkeleton[i].skinPose[0],
				theSkin.skinPoseSkeleton[i].skinPose[1],
				theSkin.skinPoseSkeleton[i].skinPose[2],
				theSkin.skinPoseSkeleton[i].skinPose[3],
				theSkin.skinPoseSkeleton[i].skinPose[4],
				theSkin.skinPoseSkeleton[i].skinPose[5],
				theSkin.skinPoseSkeleton[i].skinPose[6],
				theSkin.skinPoseSkeleton[i].skinPose[7]
			);
			if (i < (theSkin.numBones - 1)) {
				swprintf(outputString + textOffset, TEXT(",\n"));
			}
			else {
				swprintf(outputString + textOffset, TEXT("\n"));
			}
			OutputDebugString(outputString);
		}
	}
}

//def blendDualQuatFromMesh( theSkinWeights, theVerticesDic, theChoosenBone = theTreeRoot-1 ): 
void blendDualQuatFromMesh(skinData theSkin, float * vertices, float * normals, float *& blendedVertices, float *& blendedNormals, int numVertices, bool shouldIPrintIt) {
	//blendedVertices = new float[3 * numVertices];
	//    for aVertex in theSkinWeights:
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
		for (int j = 0; j < int(theSkin.boneNumVertAttrib[i]); j++) {
			//            ##############################################################
			//            
			//            (theCos, theSin), theUvector, theSfactor, theMvector = extractDualVersorParameters( fromInitialPoseToCurrentTransf[boneDic[vertexBone][0]] )
			//            isInverted = (theCos < 0.0)
			int boneIndex = int(theSkin.boneIndexesForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])]);
			//float weight = theSkin.boneWeightForSkinVertices[8 * (j + int(theSkin.boneOffsetVertAttrib[i]))];
			float weight = theSkin.boneWeightForSkinVertices[j + int(theSkin.boneOffsetVertAttrib[i])];

			float *theVersor = &theSkin.fromSkinPoseToCurrentTransf[8 * boneIndex];
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
			dual_quaternion_product_by_scalar(DUALQUAARRAY(theVersor), weight, DUALQUAARRAY(o));

			//            theBlendedQuat = dual_quaternion_sum(    theBlendedQuat, 
			//                                                     dual_quaternion_product_by_scalar(theVersor,vertexWeight)
			//                                                )
			float theSum[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
			dual_quaternion_sum(DUALQUAARRAY(theBlendedQuat),
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
		//        theNorm = dual_quaternion_norm(theBlendedQuat)
		float theNorm = dual_quaternion_norm(DUALQUAARRAY(theBlendedQuat));
		//        print "theNorm:", theNorm

		//       
		float normalizedBlend[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		dual_quaternion_product_by_scalar(DUALQUAARRAY(theBlendedQuat), 1.0 / theNorm, DUALQUAARRAY(normalizedBlend));

		//		Transforming the vertices

		float vertQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		float px = vertices[3 * i]; float py = vertices[3 * i + 1]; float pz = vertices[3 * i + 2];
		point_quaternion(px, py, pz, DUALQUAARRAY(vertQuat));

		float vertBlendQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		//        DLBVerticesDic[aVertex] = dual_quat_transform_point( dual_quaternion_product_by_scalar(theBlendedQuat,1.0/theNorm) , theVerticesDic[aVertex] )[1][1:]
		dual_quat_transform_point(DUALQUAARRAY(normalizedBlend),
			//dual_quat_transform_point(DUALQUAARRAY(theBlendedQuat),
			DUALQUAARRAY(vertQuat),
			DUALQUAARRAY(vertBlendQuat));

		blendedVertices[3 * i] = vertBlendQuat[5];
		blendedVertices[3 * i + 1] = vertBlendQuat[6];
		blendedVertices[3 * i + 2] = vertBlendQuat[7];


		TCHAR outputString[400];
		if (shouldIPrintIt) {
			int textOffset = swprintf(outputString, TEXT("{%i, {%f, %f, %f}}"),
				i,
				blendedVertices[3 * i],
				blendedVertices[3 * i + 1],
				blendedVertices[3 * i + 2]);
			if (i < (numVertices - 1)) {
				swprintf(outputString + textOffset, TEXT(",\n"));
			}
			else {
				swprintf(outputString + textOffset, TEXT("\n"));
			}
			OutputDebugString(outputString);
		}

		//		Transforming the vertices normals

		float rotationOnlyQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		extractRotationQuatFromDualQuat(DUALQUAARRAY(normalizedBlend), DUALQUAARRAY(rotationOnlyQuat));

		float normalQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		float nx = normals[3 * i]; float ny = normals[3 * i + 1]; float nz = normals[3 * i + 2];
		vector_quaternion(nx, ny, nz, DUALQUAARRAY(normalQuat));

		float normalBlendQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		dual_quat_transform_point(DUALQUAARRAY(normalizedBlend),
			DUALQUAARRAY(normalQuat),
			DUALQUAARRAY(normalBlendQuat));

		blendedNormals[3 * i] = normalBlendQuat[5];
		blendedNormals[3 * i + 1] = normalBlendQuat[6];
		blendedNormals[3 * i + 2] = normalBlendQuat[7];

		/// just for test:

		//blendedVertices[3 * i] = vertices[3 * i];
		//blendedVertices[3 * i + 1] = vertices[3 * i + 1];
		//blendedVertices[3 * i + 2] = vertices[3 * i + 2];

		//blendedNormals[3 * i] = normals[3 * i];
		//blendedNormals[3 * i + 1] = normals[3 * i + 1];
		//blendedNormals[3 * i + 2] = normals[3 * i + 2];
	}
}