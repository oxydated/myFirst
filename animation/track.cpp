#include "track.h"
#include "../scene/ticker.h"
#include "debugLog.h"

namespace oxyde {
	namespace scene {
		const keyframePtr track::getKeyFrameForTime(long time)
		{
			if (time < (*currentKeyframe)->getStartTime()) {
				currentKeyframe = keyframes.cbegin();
			}

			for (; ((currentKeyframe != keyframes.cend()) && time >= (*currentKeyframe)->getEndTime()); currentKeyframe++);

			if (currentKeyframe == keyframes.cend()) {
				currentKeyframe = keyframes.cend() - 1;
			}

			return (*currentKeyframe);
		}

		const keyframePtr track::getCurrentKeyFrame()
		{
			long currentTime = ticker::getCurrentTime();

			return getKeyFrameForTime(currentTime);
		}
	}
}