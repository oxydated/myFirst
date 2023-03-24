#pragma once

#include <vector>
#include "../xml/XMLDocument.h"
#include "keyframe.h"

namespace oxyde {
	namespace scene {

		class track {
		public:
			const keyframePtr getKeyFrameForTime(long time);
			const keyframePtr getCurrentKeyFrame();

		protected:
			long startTime;
			long endTime;
			std::vector<keyframePtr>::const_iterator currentKeyframe;
			std::vector<keyframePtr> keyframes;
		};
	}
}
