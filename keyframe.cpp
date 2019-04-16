#include "keyframe.h"
#include "ticker.h"

namespace oxyde {
	namespace scene {
		keyframe::keyframe(const MSXML2::IXMLDOMElementPtr &keyframeElement)
		{
			startTime = oxyde::XML::getFloatAttributeFromElement(keyframeElement, L"startTime");
			endTime = oxyde::XML::getFloatAttributeFromElement(keyframeElement, L"endTime");
		}

		float keyframe::getNormalizedTime()
		{
			if (endTime == startTime)
				return 1.0;
			long currentTime = ticker::getCurrentTime();
			//long currentTime = ticker::getCurrentTime() < endTime ? ( ticker::getCurrentTime() >= startTime ?  ticker::getCurrentTime() : startTime) : endTime;
			float timeNotClamped = float(currentTime - long(startTime)) / float(endTime - startTime);
			float timeClamped = timeNotClamped < 1.0 ? (0.0 <= timeNotClamped ? timeNotClamped : 0.0) : 1.0;
			return timeClamped;
		}

		float keyframe::getNormalizedTimeForTime(long time)
		{
			if (endTime == startTime)
				return 1.0;

			return float(time - long(startTime)) / float(endTime - startTime);
		}
	}
}