#include <memory>

#ifdef _WIN32
//#include <GL/glew.h>
#endif

#include "renderer.h"
#include "../scene/bone.h"
#include "debugLog.h"
#include "../files/imageLoader.h"

////////////////////////////////////////

#define VERTEX_POSITION_ATT 0
#define VERTEX_POSITION_SIZE 3

#define VERTEX_TEXCOORD_ATT 1
#define VERTEX_TEXCOORD_SIZE 2

#define VERTEX_NORMAL_ATT 2
#define VERTEX_NORMAL_SIZE 3

#define VERTEX_BONE_NUM_ATT	3	// int boneNumVertAttrib;

#define VERTEX_BONE_OFFSET_ATT 4 // int boneOffsetVertAttrib;

namespace oxyde {
	namespace GL {
		namespace renderer {

			void printMeshBuffers(const oxyde::geometry::meshPtr &theMesh)
			{
				oxyde::log::printText(L"New code");

				size_t sizeFaces;
				const unsigned int* faces = theMesh->getFacesData(sizeFaces);

				oxyde::log::printText(L"New faces");
				for (int i = 0; i < sizeFaces; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(faces[i]));
				}
				oxyde::log::printText(L"END New faces \n");

				size_t sizeVertices;
				const float* vertices = theMesh->getVerticesData(sizeVertices);

				oxyde::log::printText(L"New vertices");
				for (int i = 0; i < sizeVertices; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(vertices[i]));
				}
				oxyde::log::printText(L"END New vertices \n");

				size_t sizeTexcoord;
				const float* texcoord = theMesh->getTexCoordData(sizeTexcoord);

				oxyde::log::printText(L"New texcoord");
				for (int i = 0; i < sizeTexcoord; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(texcoord[i]));
				}
				oxyde::log::printText(L"END New texcoord \n");

				size_t sizeNormals;
				const float* normals = theMesh->getNormalsData(sizeNormals);

				oxyde::log::printText(L"New normals");
				for (int i = 0; i < sizeNormals; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(normals[i]));
				}
				oxyde::log::printText(L"END New normals \n");

			}

			void printSkinBuffers(const oxyde::geometry::skindataPtr &theSkin)
			{
				oxyde::log::printText(L"New code");

				size_t sizeboneNumVert;
				const int* boneNumVertAtt = theSkin->getBoneNumVertAttrib(sizeboneNumVert);

				oxyde::log::printText(L"New boneNumVertAtt");
				for (int i = 0; i < sizeboneNumVert; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(boneNumVertAtt[i]));
				}
				oxyde::log::printText(L"END New boneNumVertAtt \n");
				

				size_t sizeBoneOffsetVert;
				const int* boneOffsetVertAtt = theSkin->getBoneOffsetVertAttrib(sizeBoneOffsetVert);

				oxyde::log::printText(L"New boneOffsetVertAtt");
				for (int i = 0; i < sizeBoneOffsetVert; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(boneOffsetVertAtt[i]));
				}
				oxyde::log::printText(L"END New boneOffsetVertAtt \n");
				

				size_t sizeBoneIndexesForSkin;
				const int* boneIndexesForSkin = theSkin->getBoneIndexesForSkinVertices(sizeBoneIndexesForSkin);

				oxyde::log::printText(L"New boneIndexesForSkin");
				for (int i = 0; i < sizeBoneIndexesForSkin; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(boneIndexesForSkin[i]));
				}
				oxyde::log::printText(L"END New boneIndexesForSkin \n");
				

				size_t sizeBoneWeightForSkin;
				const float* boneWeightForSkin = theSkin->getBoneWeightForSkinVertices(sizeBoneWeightForSkin);

				oxyde::log::printText(L"New boneWeightForSkin");
				for (int i = 0; i < sizeBoneWeightForSkin; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(boneWeightForSkin[i]));
				}
				oxyde::log::printText(L"END New boneWeightForSkin \n");
				

				size_t sizeFromSkinposeToCurrent;
				const float* fromSkinPoseToCurrent = theSkin->getFromSkinPoseToCurrentTransf(sizeFromSkinposeToCurrent);

				oxyde::log::printText(L"New boneWeightForSkin");
				for (int i = 0; i < sizeFromSkinposeToCurrent; i++) {
					oxyde::log::printText(L"[" + std::to_wstring(i) + L"] " + std::to_wstring(fromSkinPoseToCurrent[i]));
				}
				oxyde::log::printText(L"END New boneWeightForSkin \n");
				
			}

			void printSkinAndMeshBuffers()
			{
				oxyde::geometry::skindataPtr theSkin = std::dynamic_pointer_cast<oxyde::geometry::skindata>(oxyde::scene::bone::getModifierAtIndex(0));
				if (theSkin) {
					oxyde::geometry::meshPtr theMesh = theSkin->getMesh();

					printMeshBuffers(theMesh);
					printSkinBuffers(theSkin);
				}
			}

			std::vector<skinRendererPtr> skinRenderer::skinsToRender;

			skinRenderer::skinRenderer(const oxyde::geometry::skindataPtr &theSkin):theSkinToRender(theSkin)
			{
				if (theSkinToRender) {
					oxyde::geometry::meshPtr theMesh = theSkinToRender->getMesh();

					//glGenVertexArrays(1, &vao);
					//glBindVertexArray(vao);

					theMeshRenderer = std::make_shared<meshRenderer>(theMesh);

						size_t sizeboneNumVert;
						const int* boneNumVertAtt = theSkinToRender->getBoneNumVertAttrib(sizeboneNumVert);
						//glGenBuffers(1, &vertexBoneNumBuffer);
						//glBindBuffer(GL_ARRAY_BUFFER, vertexBoneNumBuffer);
						//glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*sizeboneNumVert, (GLvoid*)boneNumVertAtt, GL_STATIC_DRAW);
						//glVertexAttribIPointer(VERTEX_BONE_NUM_ATT, 1, GL_INT, 0, (GLvoid*)0);
						//GLenum myMistake = glGetError();

						size_t sizeBoneOffsetVert;
						const int* boneOffsetVertAtt = theSkinToRender->getBoneOffsetVertAttrib(sizeBoneOffsetVert);
						//glGenBuffers(1, &vertexBoneOffsetBuffer);
						//glBindBuffer(GL_ARRAY_BUFFER, vertexBoneOffsetBuffer);
						//glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*sizeBoneOffsetVert, (GLvoid*)boneOffsetVertAtt, GL_STATIC_DRAW);
						//glVertexAttribIPointer(VERTEX_BONE_OFFSET_ATT, 1, GL_INT, 0, (GLvoid*)0);
						//myMistake = glGetError();

						size_t sizeBoneIndexesForSkin;
						const int* boneIndexesForSkin = theSkinToRender->getBoneIndexesForSkinVertices(sizeBoneIndexesForSkin);
						//glGenBuffers(1, &storeBoneIndexesBuffer);
						//glBindBuffer(GL_SHADER_STORAGE_BUFFER, storeBoneIndexesBuffer);
						//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint)*sizeBoneIndexesForSkin, (GLvoid*)boneIndexesForSkin, GL_STATIC_DRAW);
						//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, storeBoneIndexesBuffer);
						//myMistake = glGetError();

						size_t sizeBoneWeightForSkin;
						const float* boneWeightForSkin = theSkinToRender->getBoneWeightForSkinVertices(sizeBoneWeightForSkin);
						//glGenBuffers(1, &storeBoneWeightBuffer);
						//glBindBuffer(GL_SHADER_STORAGE_BUFFER, storeBoneWeightBuffer);
						//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat)*sizeBoneWeightForSkin, (GLvoid*)boneWeightForSkin, GL_STATIC_DRAW);
						//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, storeBoneWeightBuffer);
						//myMistake = glGetError();

						size_t sizeFromSkinposeToCurrent;
						const float* fromSkinPoseToCurrent = theSkinToRender->getFromSkinPoseToCurrentTransf(sizeFromSkinposeToCurrent);
						//glGenBuffers(1, &storeFromSkinPoseBuffer);
						//glBindBuffer(GL_SHADER_STORAGE_BUFFER, storeFromSkinPoseBuffer);
						//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * sizeFromSkinposeToCurrent, (GLvoid*)fromSkinPoseToCurrent, GL_DYNAMIC_DRAW);
						//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, storeFromSkinPoseBuffer);
						//myMistake = glGetError();


						//////////////////////////////////////////////////////////////////// 
						//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
						//myMistake = glGetError();

						//glEnableVertexAttribArray(VERTEX_BONE_NUM_ATT);
						//glEnableVertexAttribArray(VERTEX_BONE_OFFSET_ATT);
					//}
				}
			}

			void skinRenderer::drawSkin()
			{
					//glBindVertexArray(vao);

					size_t sizeFromSkinposeToCurrent;
					const float* fromSkinPoseToCurrent = theSkinToRender->getFromSkinPoseToCurrentTransf(sizeFromSkinposeToCurrent);
					//glBindBuffer(GL_SHADER_STORAGE_BUFFER, storeFromSkinPoseBuffer);
					//glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLfloat) * sizeFromSkinposeToCurrent, (GLvoid*)fromSkinPoseToCurrent);
					//GLenum myMistake = glGetError();

					//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
					//myMistake = glGetError();

					theMeshRenderer->drawMesh();
			}

			skinRenderer::~skinRenderer()
			{
				//glDeleteBuffers(1, &vertexBoneNumBuffer);				
				//glDeleteBuffers(1, &vertexBoneOffsetBuffer);				
				//glDeleteBuffers(1, &storeBoneIndexesBuffer);				
				//glDeleteBuffers(1, &storeBoneWeightBuffer);
				//glDeleteBuffers(1, &storeFromSkinPoseBuffer);
				theMeshRenderer.reset();
				//glDeleteVertexArrays(1, &vao);
			}

			void skinRenderer::addSkiToRender(const oxyde::geometry::skindataPtr &theSkin)
			{
				skinsToRender.push_back(std::make_shared<skinRenderer>(theSkin));
			}

			void skinRenderer::drawAllSkins()
			{
				for (auto skin : skinsToRender) {
					skin->drawSkin();
				}
			}

			void skinRenderer::reset()
			{
				skinsToRender.clear();
				textureRenderer::reset();
			}

			meshRenderer::meshRenderer(const oxyde::geometry::meshPtr & theMesh):
				theTextureMap(std::make_shared<textureRenderer>(theMesh)), theMeshToRender(theMesh)
			{

				//GLenum myMistake = glGetError();

				size_t sizeFaces;
				const unsigned int* faces = theMeshToRender->getFacesData(sizeFaces);
				//glGenBuffers(1, &elementIndexBuffer);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementIndexBuffer);
				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*sizeFaces, faces, GL_STATIC_DRAW);

				//myMistake = glGetError();

				size_t sizeVertices;
				const float* vertices = theMeshToRender->getVerticesData(sizeVertices);
				//glGenBuffers(1, &vertexPositionBuffer);
				//glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
				//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeVertices, (GLvoid*)vertices, GL_STATIC_DRAW);
				//glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				//myMistake = glGetError();

				size_t sizeTexcoord;
				const float* texcoord = theMeshToRender->getTexCoordData(sizeTexcoord);
				//glGenBuffers(1, &vertexTexcoordBuffer);
				//glBindBuffer(GL_ARRAY_BUFFER, vertexTexcoordBuffer);
				//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeTexcoord, (GLvoid*)texcoord, GL_STATIC_DRAW);
				//glVertexAttribPointer(VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				//myMistake = glGetError();

				size_t sizeNormals;
				const float* normals = theMeshToRender->getNormalsData(sizeNormals);
				//glGenBuffers(1, &vertexNormalBuffer);
				//glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBuffer);
				//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeNormals, (GLvoid*)normals, GL_STATIC_DRAW);
				//glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				//myMistake = glGetError();

				//glEnableVertexAttribArray(VERTEX_POSITION_ATT);
				//glEnableVertexAttribArray(VERTEX_TEXCOORD_ATT);
				//glEnableVertexAttribArray(VERTEX_NORMAL_ATT);

			}

			void meshRenderer::drawMesh()
			{
				theTextureMap->makeTextureActive();
				//glDrawElements(GL_TRIANGLES, theMeshToRender->getNumFaces(), GL_UNSIGNED_INT, 0);
			}

			meshRenderer::~meshRenderer()
			{
				//glDeleteBuffers(1, &elementIndexBuffer);
				//glDeleteBuffers(1, &vertexPositionBuffer);
				//glDeleteBuffers(1, &vertexTexcoordBuffer);
				//glDeleteBuffers(1, &vertexNormalBuffer);
			}

			//GLuint textureRenderer::textureUnitUsed = 0;

			textureRenderer::textureRenderer(const oxyde::geometry::meshPtr & theMesh):mapFileName(theMesh->getMapFileName())
			{
				//GLint program = -1;
				//glGetIntegerv(GL_CURRENT_PROGRAM, &program);
				//if (program) {
				//	textureUnit = textureUnitUsed++;

				//	glActiveTexture(textureUnit);
				//	
				//	//////////////////////
				//	
				//	unsigned int width = 512;
				//	unsigned int height = 512;
				//	int theGLerror = 0;

				//	std::vector<unsigned char> &&imageData = oxyde::utility::loadImageFromFile(mapFileName, width, height);

				//	glGenTextures(1, &textureName);

				//	printf("textureName: %i\n", textureName);

				//	glBindTexture(GL_TEXTURE_2D, textureName);

				//	glActiveTexture(GL_TEXTURE0);

				//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

				//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

				//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)imageData.data());

				//	if (glGetError() != GL_NO_ERROR) {
				//		printf("error up to line %i\n", __LINE__);
				//	}
				//	else {
				//		printf("ok in %i\n", __LINE__);
				//	}

				//	glGenerateMipmap(GL_TEXTURE_2D);

				//	//////////////////////

				//	GLint samplerLocation = glGetUniformLocation(program, "theSampler");

				//	glUniform1i(samplerLocation, textureUnit);
				//}
			}

			void textureRenderer::makeTextureActive()
			{
				//glActiveTexture(textureUnit);
			}

			textureRenderer::~textureRenderer()
			{
				//glDeleteTextures(1, &textureName);
			}

			void textureRenderer::reset()
			{
				//textureUnitUsed = 0;
			}

}
	}
}
