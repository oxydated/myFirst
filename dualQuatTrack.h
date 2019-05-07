#pragma once
#include "XMLDocument.h"
#include "track.h"
#include "dualQuatKeyframe.h"

namespace oxyde {
	namespace scene {

		class dualQuatTrack : track {
		public:
			dualQuatTrack(const MSXML2::IXMLDOMElementPtr&);
			// populate track with bezierFloatKeyframe
			// initialize startTime;
			// initialize endTime;
			// initialize currentKeyframe;

			const dualQuat& getValueForTime(long time);
			const dualQuat& getCurrentValue();
		};
	}
}