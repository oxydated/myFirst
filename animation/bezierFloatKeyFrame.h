#pragma once

#include "../xml/XMLDocument.h"
#include "keyframe.h"

namespace oxyde {
	namespace scene {

		class bezierFloatKeyframe: public keyframe {
		public:
			bezierFloatKeyframe(const MSXML2::IXMLDOMElementPtr&);

			float getInterpolatedValueForTime(long time);
			float getCurrentInterpolatedValue();

		protected:
			float coeff_B3, coeff_B2, coeff_B1, coeff_B0;
		};

		using bezierFloatKeyframePtr = std::shared_ptr<bezierFloatKeyframe>;
	}
}
