#ifndef __XMLSKINLOADER_H__
#define __XMLSKINLOADER_H__

#include "skinFunctions.h"
#include "xmlUtilityFunctions.h"
#include "xmlModelLoader.h"
#include "linearAlg.h"
#include "transformations.h"

		// load skinPose bone structure
HRESULT getHierarchyBoneIndexForSkinBoneIndex(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, int skinBoneIndex, int &boneIndex);

HRESULT getSkinPoseBones(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, skinPoseBone* &theSkinBones, int &numSkinBones);

		// load skin structure
HRESULT getMeshNodeForSkin(IXMLDOMDocument3* theDocument, IXMLDOMNode *skinElementNode, IXMLDOMNode* &meshElementNode);

//HRESULT countVertAttribNum(IXMLDOMNode *skinElementNode, IXMLDOMNode* meshElementNode, int &count);

HRESULT getSkinVerticesData(IXMLDOMDocument3* theDocument, IXMLDOMNode* skinElementNode, int* &boneOffset, int* &boneNum, int* &boneIndexes, float* &boneWeight, int &vertAttribNum, int& boneUniformNum);

HRESULT getSkinData(IXMLDOMDocument3* theDocument, LPCSTR skinElementNodeID, skinData &theSkinData);

HRESULT getVertexDataFromSkinDoc(IXMLDOMDocument3* theDocument, LPCSTR theSkinNodeName, int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals);

EXTERN_C void xmlLoadSkin(skinData &theSkinData);

EXTERN_C void xmlLoadModel(int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals);

#endif