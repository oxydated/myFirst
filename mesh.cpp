#include "mesh.h"
#include "linearAlg.h"
#include "transformations.h"

namespace oxyde {
	namespace geometry {

		mesh::mesh(const MSXML2::IXMLDOMElementPtr &meshElement)
		{
			std::wstring queryForAllFaces(L"./faces/face");
			MSXML2::IXMLDOMNodeListPtr allFaces = MSXML2::IXMLDOMNodePtr(meshElement)->selectNodes(queryForAllFaces.data());
			faces.resize(allFaces->length);
			for (int i = 0; i < allFaces->length; i++) {
				MSXML2::IXMLDOMElementPtr faceElement = MSXML2::IXMLDOMElementPtr(allFaces->item[i]);
				if (faceElement) {
					faces[oxyde::XML::getUShortAttributeFromElement(faceElement, L"faceID")] =
					{
						oxyde::XML::getUShortAttributeFromElement(faceElement, L"v0"),
						oxyde::XML::getUShortAttributeFromElement(faceElement, L"v1"),
						oxyde::XML::getUShortAttributeFromElement(faceElement, L"v2")
					};
				}
			}

			std::wstring queryForAllVertices(L"./vertices/vertex");
			MSXML2::IXMLDOMNodeListPtr allVertices = MSXML2::IXMLDOMNodePtr(meshElement)->selectNodes(queryForAllVertices.data());
			
			vertices.resize(allVertices->length);
			texcoord.resize(allVertices->length);
			normals.resize(allVertices->length);

			for (int i = 0; i < allVertices->length; i++) {
				MSXML2::IXMLDOMElementPtr vertexElement = MSXML2::IXMLDOMElementPtr(allVertices->item[i]);
				if (vertexElement) {
					unsigned short vertexID = oxyde::XML::getUShortAttributeFromElement(vertexElement, L"vertexID");
					vertices[vertexID] =
					{
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"x"),
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"y"),
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"z")
					};

					texcoord[vertexID] =
					{
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"u"),
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"v")
					};

					normals[vertexID] =
					{
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"nx"),
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"ny"),
						oxyde::XML::getFloatAttributeFromElement(vertexElement, L"nz")
					};
				}
			}

		}

		void mesh::applyMatrixtoVertices(std::array<float, 16>& objectMatrix)
		{
			for (auto vertex : vertices) {
				float myOldVert[4] = { vertex[0], vertex[1], vertex[2], 1.0 };
				float myNewVert[4] = { 0.0, 0.0, 0.0, 0.0 };


				oxyde::linAlg::multiplyMatrixByVector(objectMatrix.data(), myOldVert, myNewVert);
				vertex[0] = myNewVert[0];
				vertex[1] = myNewVert[1];
				vertex[2] = myNewVert[2];
			}
		}

	}
}
