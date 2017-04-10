#include "xmlSkinLoader.h"

// load skinPose bone structure

//	<bone boneIndex = "0" boneName = "Bip001 Head">
//		<boneMatrix r0c0 = "1.945255E-12" r0c1 = "1.343089E-06" r0c2 = "1" r1c0 = "-1.402693E-06" r1c1 = "-0.9999999" r1c2 = "1.343089E-06" r2c0 = "0.9999999" r2c1 = "-1.402693E-06" r2c2 = "-6.131367E-14" r3c0 = "2.654867" r3c1 = "-28.61358" r3c2 = "167.3172" / >
//		<boneDualQuat dqs = "60.0941976903" dqx = "10.1161455216" dqy = "-58.2139016237" dqz = "-10.1341166725" qs = "0.000211432540736" qx = "0.707106786455" qy = "-2.10732134678e-08" qz = "0.707106744308" / >
//	< / bone>

//<scene>
//	<node nodeName = "Scene Root" nodeObject = "0">
//		...
//			<node nodeName="Bip001 Head" nodeObject="22">

//		"./scene//node[@nodeName='" + str(aBone.attrib['boneName']) + "']"

// getHierarchyBoneIndexForSkinBoneIndex

HRESULT getHierarchyBoneIndexForSkinBoneIndex(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, int skinBoneIndex, int &boneIndex){
	HRESULT hr = S_OK;
	char skinBoneIndexStr[5];
	itoa(skinBoneIndex, skinBoneIndexStr, 10);

	_bstr_t skinBoneElementQuery = _bstr_t(".//bone[@boneIndex='") + _bstr_t(skinBoneIndexStr) + _bstr_t("']");

	IXMLDOMNode* skinBoneElementNode = NULL;
	hr = skinElementNode->selectSingleNode(skinBoneElementQuery, &skinBoneElementNode);

	_bstr_t skinBoneName;
	hr = getBSTRattributeFromElementInNode(skinBoneElementNode, "boneName", skinBoneName);

	_bstr_t boneLinkIndexQuery = _bstr_t("./scene//node[@nodeName='") + skinBoneName + _bstr_t("']");

	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* boneElementNode = NULL;
	hr = theDocument->get_documentElement(&theRoot);
	hr = theRoot->selectSingleNode(boneLinkIndexQuery, &boneElementNode);

	hr = getIntAttributeFromElementInNode(boneElementNode, "nodeObject", boneIndex);

	return hr;
}

HRESULT getSkinPoseBones(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, skinPoseBone* &theSkinBones, int &numSkinBones){
	HRESULT hr = S_OK;
	IXMLDOMNodeList* skinBonesNodesList = NULL;
	hr = skinElementNode->selectNodes(_bstr_t(".//bone"), &skinBonesNodesList);
	long testLength = 0;
	hr = skinBonesNodesList->get_length(&testLength);
	numSkinBones = testLength;

	theSkinBones = new skinPoseBone[numSkinBones];

	for (long i = 0; i < testLength; i++){
		IXMLDOMNode* skinBoneElementNode = NULL;
		hr = skinBonesNodesList->get_item(i, &skinBoneElementNode);

		//	<bone boneIndex = "0" boneName = "Bip001 Head">
		//		<boneMatrix r0c0 = "1.945255E-12" r0c1 = "1.343089E-06" r0c2 = "1" r1c0 = "-1.402693E-06" r1c1 = "-0.9999999" r1c2 = "1.343089E-06" r2c0 = "0.9999999" r2c1 = "-1.402693E-06" r2c2 = "-6.131367E-14" r3c0 = "2.654867" r3c1 = "-28.61358" r3c2 = "167.3172" / >
		//		<boneDualQuat dqs = "60.0941976903" dqx = "10.1161455216" dqy = "-58.2139016237" dqz = "-10.1341166725" qs = "0.000211432540736" qx = "0.707106786455" qy = "-2.10732134678e-08" qz = "0.707106744308" / >
		//	< / bone>

		int skinBoneIndex;
		hr = getIntAttributeFromElementInNode(skinBoneElementNode, "boneIndex", skinBoneIndex);
		int boneIndex;
		hr = getHierarchyBoneIndexForSkinBoneIndex(theDocument, skinElementNode, skinBoneIndex, boneIndex);
		theSkinBones[skinBoneIndex].boneIndex = boneIndex;

		IXMLDOMNode* boneDualQuatElementNode = NULL;
		hr = skinBoneElementNode->selectSingleNode(_bstr_t("./boneDualQuat"), &boneDualQuatElementNode);

		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "qs", theSkinBones[skinBoneIndex].skinPose[0]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "qx", theSkinBones[skinBoneIndex].skinPose[1]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "qy", theSkinBones[skinBoneIndex].skinPose[2]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "qz", theSkinBones[skinBoneIndex].skinPose[3]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "dqs", theSkinBones[skinBoneIndex].skinPose[4]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "dqx", theSkinBones[skinBoneIndex].skinPose[5]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "dqy", theSkinBones[skinBoneIndex].skinPose[6]);
		hr = getFloatAttributeFromElementInNode(boneDualQuatElementNode, "dqz", theSkinBones[skinBoneIndex].skinPose[7]);

		if (skinBoneElementNode != NULL){
			skinBoneElementNode->Release();
		}
	}

	return hr;
}

// load skin structure
HRESULT getMeshNodeForSkin(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, IXMLDOMNode* &meshElementNode){
	HRESULT hr = S_OK;
	_bstr_t meshID;
	hr = getBSTRattributeFromElementInNode(skinElementNode, "mesh", meshID);

	_bstr_t meshElementQuery = _bstr_t("./geometry/mesh[@meshID='") + meshID + _bstr_t("']");

	IXMLDOMElement* theRoot = NULL;
	hr = theDocument->get_documentElement(&theRoot);
	hr = theRoot->selectSingleNode(meshElementQuery, &meshElementNode);

	return hr;
}

HRESULT getSkinVerticesData(IXMLDOMDocument3* theDocument, IXMLDOMNode* skinElementNode, float* &boneOffset, float* &boneNum, float* &boneIndexes, float* &boneWeight, int &vertAttribNum, int& boneUniformNum){
	TCHAR outputString[100];
	
	HRESULT hr = S_OK;
	IXMLDOMNode* meshElementNode = NULL;
	hr = getMeshNodeForSkin(theDocument, skinElementNode, meshElementNode);

	////////////////////////////////////// the uniform arrays for index and weight

	IXMLDOMNodeList* skinVerticesNodesList = NULL;
	hr = skinElementNode->selectNodes(_bstr_t(".//vertex"), &skinVerticesNodesList);
	long numSkinVertices = 0;
	hr = skinVerticesNodesList->get_length(&numSkinVertices);

	int* auxBoneOffset = new int[numSkinVertices];
	int* auxNumBone = new int[numSkinVertices];

	int totalBoneUniforms = 0;
	for (long i = 0; i < numSkinVertices; i++){
		IXMLDOMNode* skinVertexElementNode = NULL;
		hr = skinVerticesNodesList->get_item(i, &skinVertexElementNode);

		//<vertex vertexIndex = "4">
		//	<bonesForVertex numBonesForVertex = "2">
		//		<boneEntry boneIndexForSkin = "7" boneIndexForVertex = "0" weight = "0.9999996" / >
		//		<boneEntry boneIndexForSkin = "13" boneIndexForVertex = "1" weight = "4.570433E-07" / >
		//	< / bonesForVertex>
		//< / vertex>

		IXMLDOMNode* bonesForVertexElementNode = NULL;
		hr = skinVertexElementNode->selectSingleNode(_bstr_t("./bonesForVertex"), &bonesForVertexElementNode);
		int numEntries;
		hr = getIntAttributeFromElementInNode(bonesForVertexElementNode, "numBonesForVertex", numEntries);
		totalBoneUniforms += numEntries;

		if (skinVertexElementNode != NULL){
			skinVertexElementNode->Release();
		}
	}

	boneUniformNum = totalBoneUniforms;
	boneIndexes = new float[totalBoneUniforms];
	boneWeight = new float[totalBoneUniforms];

	int theBoneOffset = 0;
	for (long i = 0; i < numSkinVertices; i++){
		IXMLDOMNode* skinVertexElementNode = NULL;
		hr = skinVerticesNodesList->get_item(i, &skinVertexElementNode);

		int vertexIndex;
		hr = getIntAttributeFromElementInNode(skinVertexElementNode, "vertexIndex", vertexIndex);

		//<vertex vertexIndex = "4">
		//	<bonesForVertex numBonesForVertex = "2">
		
		IXMLDOMNodeList* boneEntryNodesList = NULL;
		hr = skinVertexElementNode->selectNodes(_bstr_t(".//boneEntry"), &boneEntryNodesList);
		long numEntries = 0;
		hr = boneEntryNodesList->get_length(&numEntries);

		auxBoneOffset[vertexIndex] = theBoneOffset;
		auxNumBone[vertexIndex] = numEntries;

		for (long i = 0; i < numEntries; i++){
			IXMLDOMNode* boneEntryElementNode = NULL;
			hr = boneEntryNodesList->get_item(i, &boneEntryElementNode);

		//		<boneEntry boneIndexForSkin = "7" boneIndexForVertex = "0" weight = "0.9999996" / >
		//		<boneEntry boneIndexForSkin = "13" boneIndexForVertex = "1" weight = "4.570433E-07" / >

			int boneIndexForSkin;
			hr = getIntAttributeFromElementInNode(boneEntryElementNode, "boneIndexForSkin", boneIndexForSkin);

			int boneIndex;
			hr = getHierarchyBoneIndexForSkinBoneIndex(theDocument, skinElementNode, boneIndexForSkin, boneIndex);
			boneIndexes[theBoneOffset] = boneIndex;

			float boneWeightValue;
			hr = getFloatAttributeFromElementInNode(boneEntryElementNode, "weight", boneWeightValue);
			boneWeight[theBoneOffset] = boneWeightValue;
			theBoneOffset += 1;

			if (boneEntryElementNode != NULL){
				boneEntryElementNode->Release();
			}
		}

		if (skinVertexElementNode != NULL){
			skinVertexElementNode->Release();
		}
		//	< / bonesForVertex>
		//< / vertex>
	}

	////////////////////////////////////// the attribute arrays for offset and number of bones
	
	IXMLDOMNode* theVerticesElementNode = NULL;
	hr = getNodeElementFromNode(meshElementNode, _bstr_t("./vertices"), &theVerticesElementNode);

	int numVerts = 0;
	hr = getIntAttributeFromElementInNode(theVerticesElementNode, "numVertices", numVerts);

	IXMLDOMNodeList* theVerticesNodesList = NULL;
	hr = theVerticesElementNode->selectNodes(_bstr_t("./vertex"), &theVerticesNodesList);
	long numVertexElements = 0;
	hr = theVerticesNodesList->get_length(&numVertexElements);

	boneOffset = new float[numVertexElements];
	boneNum = new float[numVertexElements];
	vertAttribNum = numVertexElements;

	swprintf(outputString, TEXT("{\n"));
	OutputDebugString(outputString);

	for (long i = 0; i < numVertexElements; i++){
		IXMLDOMNode* theVertexNode = NULL;
		hr = theVerticesNodesList->get_item(i, &theVertexNode);

		int vertexID = 0;
		hr = getIntAttributeFromElementInNode(theVertexNode, "vertexID", vertexID);

		int theVert = 0;
		hr = getIntAttributeFromElementInNode(theVertexNode, "theVert", theVert);
		boneOffset[vertexID] = float(auxBoneOffset[theVert]);
		boneNum[vertexID] = float(auxNumBone[theVert]);

		if (theVertexNode != NULL) {
			theVertexNode->Release();
		}
		//int boneNumforVertexID = int(boneNum[vertexID]);
		//int textOffset = swprintf(outputString, TEXT("{%i, {"), vertexID);
		//for (int j = 0; j < int(boneNum[vertexID]); ++j) {
		//	int boneIndexForVertexID = int(boneIndexes[int(boneOffset[vertexID]) + j]);
		//	float boneWeightForVertexID = boneWeight[int(boneOffset[vertexID]) + j];

		//	textOffset += swprintf(outputString + textOffset, TEXT("{%i, %f}"), boneIndexForVertexID, boneWeightForVertexID);
		//	if (j < (int(boneNum[vertexID]) - 1)) {
		//		textOffset += swprintf(outputString + textOffset, TEXT(","));
		//	}
		//}
		//textOffset += swprintf(outputString + textOffset, TEXT("}}"));
		//if (i < (numVertexElements - 1)) {
		//	textOffset += swprintf(outputString + textOffset, TEXT(",\n"));
		//}
		//else {
		//	textOffset += swprintf(outputString + textOffset, TEXT("\n"));
		//}
		//OutputDebugString(outputString);
	}

	swprintf(outputString, TEXT("}\n"));
	OutputDebugString(outputString);

	return hr;
}

HRESULT getSkinData(IXMLDOMDocument3* theDocument, LPCSTR skinElementNodeName, skinData &theSkinData){
	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* skinElementNode = NULL;
	_bstr_t skinElementQuery = _bstr_t("./objectList/skin[@skinNodeName='") + _bstr_t(skinElementNodeName) + _bstr_t("']");
	hr = theDocument->get_documentElement(&theRoot);

	hr = theRoot->selectSingleNode(skinElementQuery, &skinElementNode);

	hr = getSkinVerticesData(theDocument, skinElementNode,
		theSkinData.boneOffsetVertAttrib, theSkinData.boneNumVertAttrib,
		theSkinData.boneIndexesForSkinVertices, theSkinData.boneWeightForSkinVertices,
		theSkinData.vertAttribNum, theSkinData.boneUniformNum);

	hr = getSkinPoseBones(theDocument, skinElementNode, theSkinData.skinPoseSkeleton, theSkinData.numBones);

	return hr;
}

HRESULT getVertexDataFromSkinDoc(IXMLDOMDocument3* theDocument, LPCSTR theSkinNodeName, int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals){
	TCHAR outputString[100];

	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* theSkinElementNode = NULL;
	_bstr_t skinElementQuery = _bstr_t("./objectList/skin[@skinNodeName='") + _bstr_t(theSkinNodeName) + _bstr_t("']");
	hr = theDocument->get_documentElement(&theRoot);

	hr = theRoot->selectSingleNode(skinElementQuery, &theSkinElementNode);
	IXMLDOMNode* theMeshElementNode = NULL;
	hr = getMeshNodeForSkin(theDocument, theSkinElementNode, theMeshElementNode);
	hr = getVertexDataFromMeshElement(theMeshElementNode, numFaces, numVerts, faces, vertices, texcoord, normals);

	/////////// load mesh matrix

	IXMLDOMNode* meshMatrixNode = NULL;
	hr = theSkinElementNode->selectSingleNode(_bstr_t("./objectMatrix"), &meshMatrixNode);

	//float objectMatrix[16] = { 1.00000000e+00, 0.00000000e+00, 0.00000000e+00, 0.00000000e+00,
	//	0.00000000e+00, -4.37113883e-08, 1.00000000e+00, 0.00000000e+00,
	//	0.00000000e+00, -1.00000000e+00, -4.37113883e-08, 0.00000000e+00,
	//	3.49143505e-01, 1.27211199e+01, 1.99448700e+01, 1.00000000e+00
	//};

	float objectMatrix[16] = {
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	// r0c0="1"			r0c1="0"				r0c2="0" 
	// r1c0="0"			r1c1="-4.371139E-08"	r1c2="1" 
	// r2c0="0"			r2c1="-1"				r2c2="-4.371139E-08"
	// r3c0="0.3491435" r3c1="12.72112"			r3c2="19.94487" />
	
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r0c0", objectMatrix[0]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r0c1", objectMatrix[1]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r0c2", objectMatrix[2]);
	objectMatrix[3] = 0.0;
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r1c0", objectMatrix[4]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r1c1", objectMatrix[5]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r1c2", objectMatrix[6]);
	objectMatrix[7] = 0.0;
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r2c0", objectMatrix[8]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r2c1", objectMatrix[9]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r2c2", objectMatrix[10]);
	objectMatrix[11] = 0.0;
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r3c0", objectMatrix[12]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r3c1", objectMatrix[13]);
	hr = getFloatAttributeFromElementInNode(meshMatrixNode, "r3c2", objectMatrix[14]);
	objectMatrix[15] = 1.0;

	/////////// apply mesh matrix on vertices

	//swprintf(outputString, TEXT("{\n (* {vertexID, {skinpose x, y, z}} *)\n"));
	//OutputDebugString(outputString);

	for (int i = 0; i < numVerts; i++) {
		float myOldVert[4] = { vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0 };
		float myNewVert[4] = { 0.0, 0.0, 0.0, 0.0 };

		multiplyMatrixByVector(objectMatrix, myOldVert, myNewVert);
		vertices[i * 3 + 0] = myNewVert[0];
		vertices[i * 3 + 1] = myNewVert[1];
		vertices[i * 3 + 2] = myNewVert[2];


		//int textOffset = swprintf(outputString, TEXT("{%i, {%f, %f, %f}}"), 
		//	i,
		//	vertices[i * 3 + 0],
		//	vertices[i * 3 + 1],
		//	vertices[i * 3 + 2]
		//);
		//if (i < (numVerts - 1)) {
		//	textOffset += swprintf(outputString + textOffset, TEXT(",\n"));
		//}
		//else {
		//	textOffset += swprintf(outputString + textOffset, TEXT("\n"));
		//}
		//OutputDebugString(outputString);
	}

	//swprintf(outputString, TEXT("}\n"));
	//OutputDebugString(outputString);
	/////

	return hr;

	//int meshID = 0;
	//hr = getIntAttributeFromElementInNode(theSkinElementNode, "mesh", meshID);
	//_variant_t theMeshIDint = _variant_t(meshID);
	//theMeshIDint.ChangeType(VT_BSTR);

	//////////// get mesh by meshID - LPCSTR

	//_bstr_t meshElementQuery = _bstr_t("./geometry/mesh[@meshID='") + _bstr_t(theMeshIDint) + _bstr_t("']");
	//IXMLDOMNode* theMeshElementNode = NULL;
	//hr = theRoot->selectSingleNode(meshElementQuery, &theMeshElementNode);
}

EXTERN_C void xmlLoadSkin(skinData &theSkinData){

	//<document>
	//	<objectList>
	//	<skin ID = "0" mesh = "0" skinNodeName = "Box002">


	IXMLDOMDocument3* theModelDoc = loadModelDocument("processed.OXY");

	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* theSkinElementNode = NULL;
	_bstr_t skinElementQuery = _bstr_t("./objectList/skin");
	hr = theModelDoc->get_documentElement(&theRoot);
	hr = theRoot->selectSingleNode(skinElementQuery, &theSkinElementNode);

	_bstr_t skinNodeName;
	hr = getBSTRattributeFromElementInNode(theSkinElementNode, "skinNodeName", skinNodeName);

	//getSkinData(theModelDoc, "objBeatriceA", theSkinData);
	//getSkinData(theModelDoc, "Box002", theSkinData);
	getSkinData(theModelDoc, skinNodeName, theSkinData);
}

EXTERN_C void xmlLoadModel(int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals){
	// create a XML Document and load XML model file to it

	//testTestIdea();

	IXMLDOMDocument3* theModelDoc = loadModelDocument("processed.OXY");

	HRESULT hr = S_OK;
	IXMLDOMElement* theRoot = NULL;
	IXMLDOMNode* theSkinElementNode = NULL;
	_bstr_t skinElementQuery = _bstr_t("./objectList/skin");
	hr = theModelDoc->get_documentElement(&theRoot);
	hr = theRoot->selectSingleNode(skinElementQuery, &theSkinElementNode);

	_bstr_t skinNodeName;
	hr = getBSTRattributeFromElementInNode(theSkinElementNode, "skinNodeName", skinNodeName);

	//getVertexDataFromModelDoc(theModelDoc, "objBeatriceA", numFaces, numVerts, faces, vertices, texcoord, normals);
	getVertexDataFromSkinDoc(theModelDoc, skinNodeName, numFaces, numVerts, faces, vertices, texcoord, normals);

}
