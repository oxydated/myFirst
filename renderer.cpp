#include <memory>

#ifdef _WIN32
#include <glew.h>
#endif

#include "renderer.h"
#include "bone.h"

////////////////////////////////////////

#define VERTEX_POSITION_ATT 0
#define VERTEX_POSITION_SIZE 3

#define VERTEX_TEXCOORD_ATT 1
#define VERTEX_TEXCOORD_SIZE 2

#define VERTEX_NORMAL_ATT 2
#define VERTEX_NORMAL_SIZE 3

#define VERTEX_BONE_NUM_ATT	3	// int boneNumVertAttrib;

#define VERTEX_BONE_OFFSET_ATT 4 // int boneOffsetVertAttrib;

////////////////////////////////////////

static GLuint vao = 1;
static GLuint element_index_buffer = 1;
static GLuint vertex_position_buffer = 1;
static GLuint vertex_texcoord_buffer = 1;
static GLuint vertex_normal_buffer = 1;

static GLuint vertex_bone_num_buffer = 1;
static GLuint vertex_bone_offset_buffer = 1;

static GLuint store_boneIndexes_buffer = 1;
static GLuint store_boneWeight_buffer = 1;
static GLuint store_fromSkinPose_buffer = 1;

////////////////////////////////////////


namespace oxyde {
	namespace GL {
		namespace renderer {

			void setMeshBuffers(const oxyde::geometry::meshPtr &theMesh)
			{

				GLenum myMistake = glGetError();

				size_t sizeFaces;
				const unsigned short* faces = theMesh->getFacesData(sizeFaces);
				glGenBuffers(1, &element_index_buffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_index_buffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*sizeFaces, faces, GL_STATIC_DRAW);

				myMistake = glGetError();

				size_t sizeVertices;
				const float* vertices = theMesh->getVerticesData(sizeVertices);
				glGenBuffers(1, &vertex_position_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeVertices, (GLvoid*)vertices, GL_STATIC_DRAW);
				glVertexAttribPointer(VERTEX_POSITION_ATT, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				myMistake = glGetError();

				size_t sizeTexcoord;
				const float* texcoord = theMesh->getTexCoordData(sizeTexcoord);
				glGenBuffers(1, &vertex_texcoord_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_texcoord_buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeTexcoord, (GLvoid*)texcoord, GL_STATIC_DRAW);
				glVertexAttribPointer(VERTEX_TEXCOORD_ATT, VERTEX_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				myMistake = glGetError();

				size_t sizeNormals;
				const float* normals = theMesh->getNormalsData(sizeNormals);
				glGenBuffers(1, &vertex_normal_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*sizeNormals, (GLvoid*)normals, GL_STATIC_DRAW);
				glVertexAttribPointer(VERTEX_NORMAL_ATT, VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
				myMistake = glGetError();

				glEnableVertexAttribArray(VERTEX_POSITION_ATT);
				glEnableVertexAttribArray(VERTEX_TEXCOORD_ATT);
				glEnableVertexAttribArray(VERTEX_NORMAL_ATT);

			}

			void setSkinBuffers(const oxyde::geometry::skindataPtr &theSkin)
			{
				size_t sizeboneNumVert;
				const int* boneNumVertAtt = theSkin->getBoneNumVertAttrib(sizeboneNumVert);
				glGenBuffers(1, &vertex_bone_num_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_bone_num_buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*sizeboneNumVert, (GLvoid*)boneNumVertAtt, GL_STATIC_DRAW);
				glVertexAttribIPointer(VERTEX_BONE_NUM_ATT, 1, GL_INT, 0, (GLvoid*)0);
				GLenum myMistake = glGetError();

				size_t sizeBoneOffsetVert;
				const int* boneOffsetVertAtt = theSkin->getBoneOffsetVertAttrib(sizeBoneOffsetVert);
				glGenBuffers(1, &vertex_bone_offset_buffer);
				glBindBuffer(GL_ARRAY_BUFFER, vertex_bone_offset_buffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*sizeBoneOffsetVert, (GLvoid*)boneOffsetVertAtt, GL_STATIC_DRAW);
				glVertexAttribIPointer(VERTEX_BONE_OFFSET_ATT, 1, GL_INT, 0, (GLvoid*)0);
				myMistake = glGetError();

				size_t sizeBoneIndexesForSkin;
				const int* boneIndexesForSkin = theSkin->getBoneIndexesForSkinVertices(sizeBoneIndexesForSkin);
				glGenBuffers(1, &store_boneIndexes_buffer);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_boneIndexes_buffer);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint)*sizeBoneIndexesForSkin, (GLvoid*)boneIndexesForSkin, GL_STATIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, store_boneIndexes_buffer);
				myMistake = glGetError();

				//layout(binding = 1) buffer boneIndexes {
				//	int boneIndexesForSkinVertices[];
				//};

				size_t sizeBoneWeightForSkin;
				const float* boneWeightForSkin = theSkin->getBoneWeightForSkinVertices(sizeBoneWeightForSkin);
				glGenBuffers(1, &store_boneWeight_buffer);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_boneWeight_buffer);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat)*sizeBoneWeightForSkin, (GLvoid*)boneWeightForSkin, GL_STATIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, store_boneWeight_buffer);
				myMistake = glGetError();

				//layout(binding = 2) buffer boneWeight {
				//	float boneWeightForSkinVertices[];
				//};

				size_t sizeFromSkinposeToCurrent;
				const float* fromSkinPoseToCurrent = theSkin->getFromSkinPoseToCurrentTransf(sizeFromSkinposeToCurrent);
				glGenBuffers(1, &store_fromSkinPose_buffer);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_fromSkinPose_buffer);
				glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * sizeFromSkinposeToCurrent, (GLvoid*)fromSkinPoseToCurrent, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, store_fromSkinPose_buffer);
				myMistake = glGetError();

				//layout(binding = 3) buffer fromSkinPose {
				//	mat2x4 fromSkinPoseToCurrentTransf[];
				//};

				//////////////////////////////////////////////////////////////////// 
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				myMistake = glGetError();

				glEnableVertexAttribArray(VERTEX_BONE_NUM_ATT);
				glEnableVertexAttribArray(VERTEX_BONE_OFFSET_ATT);
			}

			void drawSkin(const oxyde::geometry::skindataPtr &theSkin)
			{
				oxyde::geometry::meshPtr theMesh = theSkin->getMesh();

				/// draw skin

				glBindVertexArray(vao);

				size_t sizeFromSkinposeToCurrent;
				const float* fromSkinPoseToCurrent = theSkin->getFromSkinPoseToCurrentTransf(sizeFromSkinposeToCurrent);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, store_fromSkinPose_buffer);
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLfloat) * sizeFromSkinposeToCurrent, (GLvoid*)fromSkinPoseToCurrent);
				GLenum myMistake = glGetError();

				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				myMistake = glGetError();

				glDrawElements(GL_TRIANGLES, theMesh->getNumFaces(), GL_UNSIGNED_SHORT, 0);
				///
			}

			void setSkinAndMeshBuffers()
			{
				oxyde::geometry::skindataPtr theSkin = std::dynamic_pointer_cast<oxyde::geometry::skindata>(oxyde::scene::bone::getModifierAtIndex(0));
				if (theSkin) {
					oxyde::geometry::meshPtr theMesh = theSkin->getMesh();

					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);

					setMeshBuffers(theMesh);
					setSkinBuffers(theSkin);
				}
			}

			void draw()
			{
				if (oxyde::scene::bone::getModifierAtIndex(0)) {
					oxyde::geometry::skeletalModifierPtr modifier = oxyde::scene::bone::getModifierAtIndex(0);
					oxyde::geometry::skindataPtr theSkin = std::dynamic_pointer_cast<oxyde::geometry::skindata>(modifier);
					if (theSkin) {
						drawSkin(theSkin);
					}
				}
			}
		}
	}
}
