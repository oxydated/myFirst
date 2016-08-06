#ifndef __XMLMODELLOADER_H__
#define __XMLMODELLOADER_H__

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif
//
//#include <ObjBase.h>
//#include <msxml6.h>
//#include <comutil.h>
//#include <tchar.h>
#include "xmlUtilityFunctions.h"

HRESULT getVertexDataFromMeshElement(IXMLDOMNode* theMeshElementNode, int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals);
//EXTERN_C void xmlLoadModel(int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals);

#endif