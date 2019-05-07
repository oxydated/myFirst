#include "dualQuatTrack.h"

namespace oxyde {
	namespace scene {

		dualQuatTrack::dualQuatTrack(const MSXML2::IXMLDOMElementPtr &dualQuatTrackElement)
		{
			// populate track with bezierFloatKeyframe

			MSXML2::IXMLDOMNodePtr theNode = MSXML2::IXMLDOMNodePtr(dualQuatTrackElement);
			MSXML2::IXMLDOMNodeListPtr dualQuatKeyframes = theNode->selectNodes(L"./dualQuatKey");
			for (int i = 0; i < dualQuatKeyframes->length; i++) {
				MSXML2::IXMLDOMElementPtr dualQuatKeyframeElement = MSXML2::IXMLDOMElementPtr(dualQuatKeyframes->item[i]);
				if (dualQuatKeyframeElement) {

					dualQuatKeyframePtr thedualQuatKeyframe = std::make_shared<dualQuatKeyframe>(dualQuatKeyframeElement);
					keyframes.push_back(thedualQuatKeyframe);
					//track.push_back(dualQuatKeyframe(dualQuatKeyframeElement));
				}
			}

			// initialize startTime;
			// initialize endTime;

			startTime = (*keyframes.cbegin())->getStartTime();
			endTime = (*keyframes.crbegin())->getEndTime();

			// initialize currentKeyframe;

			currentKeyframe = keyframes.cbegin();
		}

		const dualQuat& dualQuatTrack::getValueForTime(long time)
		{
			//long animationTime = time < endTime ? (time >= startTime ? time : startTime) : endTime;

			//return (std::static_pointer_cast<bezierFloatKeyframe>(getKeyFrameForTime(time)))->getInterpolatedValueForTime(animationTime);

			return (std::static_pointer_cast<dualQuatKeyframe>(getKeyFrameForTime(time)))->getInterpolatedQuaternionForTime(time);
		}

		const dualQuat& dualQuatTrack::getCurrentValue()
		{
			dualQuat mid = (std::static_pointer_cast<dualQuatKeyframe>(getCurrentKeyFrame()))->getInterpolatedQuaternion();
			return mid;
		}

	}
}