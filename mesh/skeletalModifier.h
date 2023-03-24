#pragma once
#include <memory>
#include <vector>
#include <array>
#include "dualQuaternionFunctions.h"

namespace oxyde {
	namespace geometry {

		using dualQuat = oxyde::DQ::dualQuat;

		class skeletalModifier {
		public:
			virtual void updateSkinPose(std::vector<dualQuat>&) = 0;
		};

		using skeletalModifierPtr = std::shared_ptr<skeletalModifier>;
	}
}
