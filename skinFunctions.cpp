#include "skinFunctions.h"

void alocateSkinArraysForTracks(sceneTracks theTracks, skinData &theSkin){
	theSkin.fromSkinPoseToCurrentTransf = new float[8 * theTracks.numTracks];
}

void transformFromSkinPoseToCurrentPose(skinData &theSkin, sceneTracks theTracks){

	//for aKey in initialSkinPose :
	//	if aKey in theLinks :
	//		fromInitialPoseToCurrentTransf[aKey] = transformFromSourceToDestinationAxis(initialSkinPose[aKey], globalTransforms[aKey])
	for (int i = 0; i < theSkin.numBones; i++){

		float* boneGlobalTransform = &theTracks.globalTransforms[theSkin.skinPoseSkeleton[i].boneIndex * 8];
		float* transformedFromSkin = &theSkin.fromSkinPoseToCurrentTransf[theSkin.skinPoseSkeleton[i].boneIndex * 8];

		transformFromSourceToDestinationAxis(DUALQUAARRAY(theSkin.skinPoseSkeleton[i].skinPose), DUALQUAARRAY(boneGlobalTransform), DUALQUAARRAY(transformedFromSkin));
	}
}

//def blendDualQuatFromMesh( theSkinWeights, theVerticesDic, theChoosenBone = theTreeRoot-1 ): 
void blendDualQuatFromMesh(skinData theSkin, float *vertices, float* normals, float* &blendedVertices, float* &blendedNormals, int numVertices){
	//blendedVertices = new float[3 * numVertices];
	//    for aVertex in theSkinWeights:
	for (int i = 0; i < numVertices; i++){
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
		for (int j = 0; j < int(theSkin.boneNumVertAttrib[i]); j++){
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
			if (theVersor[0] < 0.0){
				dual_quaternion_complement(DUALQUAARRAY(theVersor), DUALQUAARRAY(theComplement));

				theVersor = theComplement;
			}
			

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

		//		Transforming the vertices normals

		float rotationOnlyQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		extractRotationQuatFromDualQuat(DUALQUAARRAY(normalizedBlend), DUALQUAARRAY(rotationOnlyQuat));

		float normalQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		float nx = normals[3 * i]; float ny = normals[3 * i + 1]; float nz = normals[3 * i + 2];
		point_quaternion(nx, ny, nz, DUALQUAARRAY(normalQuat));

		float normalBlendQuat[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		dual_quat_transform_point(DUALQUAARRAY(rotationOnlyQuat),
			DUALQUAARRAY(normalQuat),
			DUALQUAARRAY(normalBlendQuat));

		blendedNormals[3 * i] = normalBlendQuat[5];
		blendedNormals[3 * i + 1] = normalBlendQuat[6];
		blendedNormals[3 * i + 2] = normalBlendQuat[7];
	}
}