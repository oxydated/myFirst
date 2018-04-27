#pragma once

#include "mesh.h"
#include "skindata.h"

namespace oxyde {
	namespace GL {
		namespace renderer {
			void setMeshBuffers(const oxyde::geometry::meshPtr&);
			void setSkinBuffers(const oxyde::geometry::skindataPtr&);

			void drawSkin(const oxyde::geometry::skindataPtr&);

			void setSkinAndMeshBuffers();
			void draw();
		}
	}
}
