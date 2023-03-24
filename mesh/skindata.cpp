#include <map>
#include "skindata.h"
#include "debugLog.h"
#include "../rendering/renderer.h"

namespace oxyde {
	namespace geometry {

		skindata::skindata(const MSXML2::IXMLDOMElementPtr &documentElement, int ID, const notAccessible&):skinID(ID)
		{
			MSXML2::IXMLDOMElementPtr skinElement = MSXML2::IXMLDOMElementPtr(MSXML2::IXMLDOMNodePtr(documentElement)->selectSingleNode((L"./objectList/skin[@ID=" + std::to_wstring(ID) + L"]").data()));

			/// load the mesh object

			std::wstring theMeshID = oxyde::XML::getWStringAttributeFromElement(skinElement, L"mesh");
			std::wstring queryForTheMesh = L"./geometry/mesh[@meshID = '" + theMeshID + L"']";
			MSXML2::IXMLDOMElementPtr meshElement = MSXML2::IXMLDOMElementPtr(MSXML2::IXMLDOMNodePtr(documentElement)->selectSingleNode(queryForTheMesh.data()));
			theMesh = std::make_shared<oxyde::geometry::mesh>(meshElement);

			/// build a map of nodeObject by boneIndex and load skin pose

			std::map<int, int> nodeObjecByBoneIndex;
			std::map<int, dualQuat> skinPoseByNodeObject;

			MSXML2::IXMLDOMNodeListPtr allBonesInSkin = MSXML2::IXMLDOMNodePtr(skinElement)->selectNodes(L"./bones//bone");
			for (int i = 0; i < allBonesInSkin->length; i++) {
				MSXML2::IXMLDOMElementPtr boneElement = MSXML2::IXMLDOMElementPtr(allBonesInSkin->item[i]);
				if (boneElement) {
					std::wstring boneName = oxyde::XML::getWStringAttributeFromElement(boneElement, L"boneName");
					int boneIndex = oxyde::XML::getIntAttributeFromElement(boneElement, L"boneIndex");
					std::wstring queryByNodeName = L"./scene//node[@nodeName='" + boneName + L"']";

					MSXML2::IXMLDOMElementPtr nodeElement = MSXML2::IXMLDOMElementPtr(MSXML2::IXMLDOMNodePtr(documentElement)->selectSingleNode(queryByNodeName.data()));
					int nodeObject = oxyde::XML::getIntAttributeFromElement(nodeElement, L"nodeObject");

					nodeObjects.push_back(nodeObject);
					nodeObjecByBoneIndex[boneIndex] = nodeObject;

					MSXML2::IXMLDOMElementPtr boneDualQuatElement = MSXML2::IXMLDOMElementPtr(MSXML2::IXMLDOMNodePtr(boneElement)->selectSingleNode(L"./boneDualQuat"));

					skinPoseByNodeObject[nodeObject] = {
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"qs"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"qx"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"qy"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"qz"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"dqs"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"dqx"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"dqy"),
						oxyde::XML::getFloatAttributeFromElement(boneDualQuatElement, L"dqz")
					};
				}
			}

			int sizeOfBoneVectors = skinPoseByNodeObject.rbegin()->first + 1;
			skinPoseSkeleton.resize(sizeOfBoneVectors);
			fromSkinPoseToCurrentTransf.resize(sizeOfBoneVectors);

			for (auto dualQuatPair : skinPoseByNodeObject) {
				skinPoseSkeleton[dualQuatPair.first] = dualQuatPair.second;
			}

			/// apply the object matrix to the mesh's vertices

			MSXML2::IXMLDOMElementPtr matrixElement = MSXML2::IXMLDOMElementPtr(MSXML2::IXMLDOMNodePtr(skinElement)->selectSingleNode(L"./objectMatrix"));
			if (matrixElement) {
				std::array<float, 16> objectMatrix = {
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r0c0"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r0c1"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r0c2"),
					0.0,
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r1c0"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r1c1"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r1c2"),
					0.0,
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r2c0"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r2c1"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r2c2"),
					0.0,
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r3c0"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r3c1"),
					oxyde::XML::getFloatAttributeFromElement(matrixElement, L"r3c2"),
					1.0
				};

				theMesh->applyMatrixtoVertices(objectMatrix);
			}


			/// Loading skin vertex attributes and uniforms

			std::vector<int> skinBoneOffsetVertAttrib;
			std::vector<int> skinBoneNumVertAttrib;

			boneOffsetVertAttrib.clear();
			boneNumVertAttrib.clear();
			boneIndexesForSkinVertices.clear();
			boneWeightForSkinVertices.clear();

			MSXML2::IXMLDOMNodeListPtr verticesInSkin = MSXML2::IXMLDOMNodePtr(skinElement)->selectNodes(L"./vertices/vertex");
			for (int i = 0; i < verticesInSkin->length; i++) {
				MSXML2::IXMLDOMElementPtr skinVertexElement = MSXML2::IXMLDOMElementPtr(verticesInSkin->item[i]);
				if (skinVertexElement) {
					MSXML2::IXMLDOMNodeListPtr boneEntriesForVertex = MSXML2::IXMLDOMNodePtr(skinVertexElement)->selectNodes(L"./bonesForVertex/boneEntry");

					skinBoneOffsetVertAttrib.push_back(boneWeightForSkinVertices.size());
					skinBoneNumVertAttrib.push_back(boneEntriesForVertex->length);

					for (int i = 0; i < boneEntriesForVertex->length; i++) {
						MSXML2::IXMLDOMElementPtr boneEntryElement = MSXML2::IXMLDOMElementPtr(boneEntriesForVertex->item[i]);
						if (boneEntryElement) {
							boneIndexesForSkinVertices.push_back(nodeObjecByBoneIndex[oxyde::XML::getIntAttributeFromElement(boneEntryElement, L"boneIndexForSkin")]);
							boneWeightForSkinVertices.push_back(oxyde::XML::getFloatAttributeFromElement(boneEntryElement, L"weight"));
						}
					}
				}
			}
			
			MSXML2::IXMLDOMNodeListPtr verticesInMesh = MSXML2::IXMLDOMNodePtr(meshElement)->selectNodes(L"./vertices/vertex");
			for (int i = 0; i < verticesInMesh->length; i++) {
				MSXML2::IXMLDOMElementPtr vertexElement = MSXML2::IXMLDOMElementPtr(verticesInMesh->item[i]);
				if (vertexElement) {
					int theVert = oxyde::XML::getIntAttributeFromElement(vertexElement, L"theVert");

					boneOffsetVertAttrib.push_back(skinBoneOffsetVertAttrib[theVert]);
					boneNumVertAttrib.push_back(skinBoneNumVertAttrib[theVert]);
				}
			}

			boneOffsetVertAttrib.shrink_to_fit();
			boneNumVertAttrib.shrink_to_fit();
			boneIndexesForSkinVertices.shrink_to_fit();
			boneWeightForSkinVertices.shrink_to_fit();

			nodeObjects.shrink_to_fit();

			/// testing

			oxyde::log::printText(L"skindata constructor");
			oxyde::log::printLine();

			for (int i = 0; i < boneOffsetVertAttrib.size(); i++) {
				std::wstring tocheck = L"[" + std::to_wstring(i) + L"]  " +
					L"boneOffsetVertAttrib: " +
					std::to_wstring(boneOffsetVertAttrib.data()[i]) +
					L"		boneNumVertAttrib: " +
					std::to_wstring(boneNumVertAttrib.data()[i]);
				oxyde::log::printText(tocheck);
			}

			oxyde::log::printLine();
			oxyde::log::printLine();

			for (int i = 0; i < boneIndexesForSkinVertices.size(); i++) {
				std::wstring tocheck = L"[" + std::to_wstring(i) + L"]  " +
					L"boneIndexesForSkinVertices: " +
					std::to_wstring(boneIndexesForSkinVertices.data()[i]) +
					L"		boneWeightForSkinVertices: " +
					std::to_wstring(boneWeightForSkinVertices.data()[i]);
				oxyde::log::printText(tocheck);
			}

			/// end of test
		}

		const int * skindata::getBoneOffsetVertAttrib(size_t &size)
		{
			size = boneOffsetVertAttrib.size();
			return boneOffsetVertAttrib.data();
		}

		const int * skindata::getBoneNumVertAttrib(size_t &size)
		{
			size = boneNumVertAttrib.size();
			return boneNumVertAttrib.data();
		}

		const int * skindata::getBoneIndexesForSkinVertices(size_t &size)
		{
			size = boneIndexesForSkinVertices.size();
			return boneIndexesForSkinVertices.data();
		}

		const float * skindata::getBoneWeightForSkinVertices(size_t &size)
		{
			size = boneWeightForSkinVertices.size();
			return boneWeightForSkinVertices.data();
		}

		const float * skindata::getFromSkinPoseToCurrentTransf(size_t &size)
		{
			size = fromSkinPoseToCurrentTransf.size() * 8;
			return (float*) fromSkinPoseToCurrentTransf.data();
		}

		void skindata::updateSkinPose(std::vector<dualQuat>& boneGlobalTransform)
		{
			for (int nodeObject : nodeObjects) {

				oxyde::DQ::transformFromSourceToDestinationAxis(DUALQUAARRAY(skinPoseSkeleton[nodeObject]), DUALQUAARRAY(boneGlobalTransform[nodeObject]), DUALQUAARRAY(fromSkinPoseToCurrentTransf[nodeObject]));
			}
		}

		void skindata::buildSkindata(const MSXML2::IXMLDOMElementPtr &documentElement, std::function<void(skeletalModifierPtr)> forEachNewSkin)
		{
			MSXML2::IXMLDOMNodeListPtr allSkinIDinFile = MSXML2::IXMLDOMNodePtr(documentElement)->selectNodes(L"./objectList/skin/@ID");
			//int testLenght = allSkinIDinFile->length;
			for (int i = 0; i < allSkinIDinFile->length; i++) {
				MSXML2::IXMLDOMAttributePtr skinIDatt = MSXML2::IXMLDOMAttributePtr(allSkinIDinFile->item[i]);
				if (skinIDatt) {
					_variant_t&& intVariant = skinIDatt->GetnodeTypedValue();
					intVariant.ChangeType(VT_I4);
					skindataPtr newSkin = std::make_shared<skindata>(documentElement, intVariant.intVal, notAccessible());
					oxyde::GL::renderer::skinRenderer::addSkiToRender(newSkin);
					forEachNewSkin(newSkin);
				}
			}
		}

		const meshPtr & skindata::getMesh()
		{
			return theMesh;
		}

	}
}
