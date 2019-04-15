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
			long currentTime = ticker::getCurrentTime();
			if (endTime == startTime)
				return 1.0;
			return float(currentTime - long(startTime)) / float(endTime - startTime);
		}

		float keyframe::getNormalizedTimeForTime(long time)
		{
			if (endTime == startTime)
				return 1.0;
			return float(time - long(startTime)) / float(endTime - startTime);
		}
	}
}