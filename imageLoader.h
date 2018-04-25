#pragma once
#include <string>
#include <vector>

namespace oxyde {
	namespace utility {
		std::vector<unsigned char> loadImageFromFile(const std::wstring &fileName, unsigned int &Width, unsigned int &Height);
	}
}
