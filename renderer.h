#pragma once

#ifdef _WIN32
#include <glew.h>
#endif

#include "mesh.h"
#include "skindata.h"

namespace oxyde {
	namespace GL {
		namespace renderer {

			class textureRenderer {
			public:
				textureRenderer(const oxyde::geometry::meshPtr &theMesh);
				void makeTextureActive();
				//unsigned int getTextureUnit();
				~textureRenderer();

			private:
				std::wstring mapFileName;
				GLuint textureUnit;
				unsigned int textureName;

				static GLuint textureUnitUsed;
			};

			using textureRendererPtr = std::shared_ptr<textureRenderer>;

			class meshRenderer {
			public:
				meshRenderer(const oxyde::geometry::meshPtr &theMesh);
				void drawMesh();
				~meshRenderer();

			private:
				oxyde::geometry::meshPtr theMeshToRender;
				textureRendererPtr theTextureMap;

				GLuint elementIndexBuffer;
				GLuint vertexPositionBuffer;
				GLuint vertexTexcoordBuffer;
				GLuint vertexNormalBuffer;
			};

			using meshRendererPtr = std::shared_ptr<meshRenderer>;

			class skinRenderer;
			using skinRendererPtr = std::shared_ptr<skinRenderer>;

			class skinRenderer {
			public:
				skinRenderer(const oxyde::geometry::skindataPtr &theSkin);
				void drawSkin();
				~skinRenderer();

				static void addSkiToRender(const oxyde::geometry::skindataPtr &theSkin);
				static void drawAllSkins();

			private:
				oxyde::geometry::skindataPtr theSkinToRender;
				meshRendererPtr theMeshRenderer;

				GLuint vao;

				GLuint vertexBoneNumBuffer;
				GLuint vertexBoneOffsetBuffer;
				
				GLuint storeBoneIndexesBuffer;
				GLuint storeBoneWeightBuffer;
				GLuint storeFromSkinPoseBuffer;

				static std::vector<skinRendererPtr> skinsToRender;
			};

			////////////////////

			void printMeshBuffers(const oxyde::geometry::meshPtr &theMesh);
			void printSkinBuffers(const oxyde::geometry::skindataPtr &theSkin);
			void printSkinAndMeshBuffers();

			void setMeshBuffers(const oxyde::geometry::meshPtr&);
			void setSkinBuffers(const oxyde::geometry::skindataPtr&);

			//void drawSkin(const oxyde::geometry::skindataPtr&, GLuint fromSkinPoseBuffer);

			void setSkinAndMeshBuffers();
			bool draw(GLuint fromSkinPoseBuffer);

			GLuint getFromSkinPoseBuffer();
		}
	}
}
