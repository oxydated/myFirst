#pragma once

#include "mesh.h"
#include "skindata.h"

namespace oxyde {
	namespace GL {
		namespace renderer {

			void printMeshBuffers(const oxyde::geometry::meshPtr &theMesh);
			void printSkinBuffers(const oxyde::geometry::skindataPtr &theSkin);
			void printSkinAndMeshBuffers();

			void setMeshBuffers(const oxyde::geometry::meshPtr&);
			void setSkinBuffers(const oxyde::geometry::skindataPtr&);

			void drawSkin(const oxyde::geometry::skindataPtr&, GLuint fromSkinPoseBuffer);

			void setSkinAndMeshBuffers();
			bool draw(GLuint fromSkinPoseBuffer);

			GLuint getFromSkinPoseBuffer();
		}
	}
}
