#include "track.h"
#include "ticker.h"

namespace oxyde {
	namespace scene {
		const keyframePtr track::getKeyFrameForTime(long time)
		{
			if (time < startTime) {
				currentKeyframe = keyframes.cbegin();
			}

			if (time < endTime) {

				for (; (time >= (*currentKeyframe)->getEndTime()) && (currentKeyframe != keyframes.cend()); currentKeyframe++);

				if (currentKeyframe == keyframes.cend()) {
					currentKeyframe = keyframes.cend()-1;
				}
			}
			else {
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