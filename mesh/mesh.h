#pragma once
#include<memory>
#include<vector>
#include<array>
#include "../xml/XMLDocument.h"

/*
class mesh{
	??? //To be built and kept as singleton by a factory
	
	Constructor accepts size of the vectors as argument.
	addVertex to add vertex data to vectors
	
	vertices = new float[3 * testLength];
	texcoord = new float[2 * testLength];
	normals = new float[3 * testLength];
	
	Must add facility to build OpenGL Buffers and Attributes
		provide getData for each vector.
};
*/

namespace oxyde {
	namespace geometry {
		class mesh {
		public:
			mesh(const MSXML2::IXMLDOMElementPtr&);

			const unsigned int* getFacesData(size_t &size);
			const float* getVerticesData(size_t &size);
			const float* getTexCoordData(size_t &size);
			const float* getNormalsData(size_t &size);

			const std::wstring getMapFileName();

			int getNumFaces();

			void applyMatrixtoVertices(std::array<float, 16>&);

		private:

			std::wstring textureFileName;

			std::vector<std::array<unsigned int, 3>> faces;

			std::vector<std::array<float, 3>> vertices;
			std::vector<std::array<float, 2>> texcoord;
			std::vector<std::array<float, 3>> normals;

		};

		using meshPtr = std::shared_ptr<mesh>;
	}
}
