#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#include "../stdafx.h"
#endif
#include <stdio.h>
#include "xmlModelLoader.h"
#include "dualQuaternionFunctions.h"

HRESULT getFaceDataFromModelDoc(IXMLDOMDocument3* theDocument){
	return S_OK;
}

//HRESULT getVertexDataFromModelDoc(IXMLDOMNode* theMeshElementNode, int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals){
HRESULT getVertexDataFromMeshElement(IXMLDOMNode* theMeshElementNode, int &numFaces, int &numVerts, unsigned short* &faces, float* &vertices, float* &texcoord, float* &normals){
	HRESULT hr = S_OK;

	/////////// load mesh vertices

	IXMLDOMNode* theVerticesElementNode = NULL;
	hr = getNodeElementFromNode(theMeshElementNode, _bstr_t("./vertices"), &theVerticesElementNode);
	
	numVerts = 0;
	hr = getIntAttributeFromElementInNode(theVerticesElementNode, "numVertices", numVerts);

	IXMLDOMNodeList* theVerticesNodesList = NULL;
	hr = theVerticesElementNode->selectNodes(_bstr_t("./vertex"), &theVerticesNodesList);
	long testLength = 0;
	hr = theVerticesNodesList->get_length(&testLength);

	vertices = new float[3 * testLength];
	texcoord = new float[2 * testLength];
	normals = new float[3 * testLength];
	
	for (long i = 0; i < testLength; i++){
		IXMLDOMNode* theVertexNode = NULL;
		hr = theVerticesNodesList->get_item(i, &theVertexNode);
		int vertexID = 0;
		/*<vertex nx = "-0.8184771" ny = "9.991667E-09" nz = "-0.5745391"
			theNormal = "1"
			theTVert = "400"
			theVert = "342"
			u = "1" v = "0"
			vertexID = "1"
			x = "-3.693069" y = "5.247759" z = "18.59679" / >*/

		hr = getIntAttributeFromElementInNode(theVertexNode, "vertexID", vertexID);

		hr = getFloatAttributeFromElementInNode(theVertexNode, "x", vertices[i * 3 + 0]);
		hr = getFloatAttributeFromElementInNode(theVertexNode, "y", vertices[i * 3 + 1]);
		hr = getFloatAttributeFromElementInNode(theVertexNode, "z", vertices[i * 3 + 2]);

		hr = getFloatAttributeFromElementInNode(theVertexNode, "u", texcoord[i * 2 + 0]);
		hr = getFloatAttributeFromElementInNode(theVertexNode, "v", texcoord[i * 2 + 1]);

		hr = getFloatAttributeFromElementInNode(theVertexNode, "nx", normals[i * 3 + 0]);
		hr = getFloatAttributeFromElementInNode(theVertexNode, "ny", normals[i * 3 + 1]);
		hr = getFloatAttributeFromElementInNode(theVertexNode, "nz", normals[i * 3 + 2]);

		if (theVertexNode != NULL){
			theVertexNode->Release();
		}
	}

	/////////// load mesh faces
	//	<faces numFaces = "1450">

	IXMLDOMNode* theFacesElementNode = NULL;
	hr = getNodeElementFromNode(theMeshElementNode, _bstr_t("./faces"), &theFacesElementNode);

	numFaces = 0;
	hr = getIntAttributeFromElementInNode(theFacesElementNode, "numFaces", numFaces);

	IXMLDOMNodeList* theFacesNodesList = NULL;
	hr = theFacesElementNode->selectNodes(_bstr_t("./face"), &theFacesNodesList);

	hr = theFacesNodesList->get_length(&testLength);

	faces = new unsigned short[3 * testLength];

	for (long i = 0; i < testLength; i++){

//		<face faceID = "0" v0 = "0" v1 = "1" v2 = "2" / >

		IXMLDOMNode* theFaceNode = NULL;
		hr = theFacesNodesList->get_item(i, &theFaceNode);
		int faceID = 0;

		hr = getIntAttributeFromElementInNode(theFaceNode, "faceID", faceID);

		hr = getUShortAttributeFromElementInNode(theFaceNode, "v0", faces[i * 3 + 0]);
		hr = getUShortAttributeFromElementInNode(theFaceNode, "v1", faces[i * 3 + 1]);
		hr = getUShortAttributeFromElementInNode(theFaceNode, "v2", faces[i * 3 + 2]);
		unsigned short us0 = faces[i * 3 + 0];
		unsigned short us1 = faces[i * 3 + 1];
		unsigned short us2 = faces[i * 3 + 2];

		if (theFaceNode != NULL){
			theFaceNode->Release();
		}
	}

	return S_OK;
}
