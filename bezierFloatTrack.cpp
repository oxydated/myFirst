#include "bezierFloatTrack.h"

namespace oxyde {
	namespace scene {

		//	<BezierTrack numKeys = "2">
		//		<BezierKey startTime = "0" endTime = "4320" coeff_B3 = "-1697.934814" coeff_B2 = "2546.902344" coeff_B1 = "0.000000" coeff_B0 = "-810.199219" / >
		//		<BezierKey startTime = "4320" endTime = "6720" coeff_B3 = "718.127869" coeff_B2 = "-1077.191650" coeff_B1 = "0.000000" coeff_B0 = "38.768188" / >
		//	< / BezierTrack>

		bezierFloatTrack::bezierFloatTrack(const MSXML2::IXMLDOMElementPtr &bezierFloatTrackElement)
		{
	/*
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");
				std::wstring queryKeyframes = L"./dualQuatTrack/dualQuatKey";
				MSXML2::IXMLDOMNodeListPtr dualQuatKeyframes = theNode->selectNodes(queryKeyframes.data());
				for (int i = 0; i < dualQuatKeyframes->length; i++) {
					MSXML2::IXMLDOMElementPtr dualQuatKeyframeElement = MSXML2::IXMLDOMElementPtr(dualQuatKeyframes->item[i]);
					if (dualQuatKeyframeElement) {
						track.push_back(dualQuatKeyframe(dualQuatKeyframeElement));
					}
				}
*/
			
			// populate track with bezierFloatKeyframe

			MSXML2::IXMLDOMNodePtr theNode = MSXML2::IXMLDOMNodePtr(bezierFloatTrackElement);
			MSXML2::IXMLDOMNodeListPtr bezierKeyframes = theNode->selectNodes(L"./BezierKey");
			for (int i = 0; i < bezierKeyframes->length; i++) {
				MSXML2::IXMLDOMElementPtr bezierKeyframeElement = MSXML2::IXMLDOMElementPtr(bezierKeyframes->item[i]);
				if (bezierKeyframeElement) {

					bezierFloatKeyframePtr theBezierKeyframe = std::make_shared<bezierFloatKeyframe>(bezierKeyframeElement);
					keyframes.push_back(theBezierKeyframe);
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

		float bezierFloatTrack::getValueForTime(long time)
		{
			return (std::static_pointer_cast<bezierFloatKeyframe>(getKeyFrameForTime(time)))->getInterpolatedValueForTime(time);
		}

		float bezierFloatTrack::getCurrentValue()
		{
			return (std::static_pointer_cast<bezierFloatKeyframe>(getCurrentKeyFrame()))->getCurrentInterpolatedValue();
		}
	}
}
