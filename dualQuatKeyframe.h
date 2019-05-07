#pragma once

#include <array>
#include "XMLDocument.h"
#include "keyframe.h"
#include "dualQuaternionMath.h"
#include "dualQuaternionFunctions.h"


/*
class quatKeyFrame extends keyFrame{
	float startTransform[8];
	float theAngle;
	float theUvector[3];
	float theSfactor;
	float theMvector[3];
	int getInterpolatedQuaternion( out float[8] );
};
*/

namespace oxyde {
	namespace scene {

		using dualQuat = oxyde::DQ::dualQuat;

		class dualQuatKeyframe : public keyframe {
		protected:

			dualQuat startTransform;
			float theAngle;
			std::array<float, 3> theUvector;
			float theSfactor;
			std::array<float, 3> theMvector;

		public:

			dualQuatKeyframe(const MSXML2::IXMLDOMElementPtr&);

			const dualQuat& getInterpolatedQuaternion();
			const dualQuat&  getInterpolatedQuaternionForTime(long time);
			const dualQuat& getStartTransformation();
			const dualQuat& dualQuatKeyframe::getEndTransformation();
		};

		using dualQuatKeyframePtr = std::shared_ptr<dualQuatKeyframe>;
	}
}