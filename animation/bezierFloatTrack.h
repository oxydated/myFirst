#pragma once
#include "../xml/XMLDocument.h"
#include "track.h"
#include "bezierFloatKeyFrame.h"

namespace oxyde {
	namespace scene {

		class bezierFloatTrack : track {
		public:
			bezierFloatTrack(const MSXML2::IXMLDOMElementPtr&);
			// populate track with bezierFloatKeyframe
			// initialize startTime;
			// initialize endTime;
			// initialize currentKeyframe;

			float getValueForTime(long time);
			float getCurrentValue();
		};
	}
}
