#include "bezierFloatKeyFrame.h"

namespace oxyde {
	namespace scene {

		//	<BezierKey startTime = "0" endTime = "6720" coeff_B3 = "-40.738831" coeff_B2 = "61.108246" coeff_B1 = "0.000000" coeff_B0 = "10.836702" / >

		bezierFloatKeyframe::bezierFloatKeyframe(const MSXML2::IXMLDOMElementPtr &theBezierFloatKeyFrameElement) :keyframe(theBezierFloatKeyFrameElement)
		{
			coeff_B3 = oxyde::XML::getFloatAttributeFromElement(theBezierFloatKeyFrameElement, L"coeff_B3");
			coeff_B2 = oxyde::XML::getFloatAttributeFromElement(theBezierFloatKeyFrameElement, L"coeff_B2");
			coeff_B1 = oxyde::XML::getFloatAttributeFromElement(theBezierFloatKeyFrameElement, L"coeff_B1");
			coeff_B0 = oxyde::XML::getFloatAttributeFromElement(theBezierFloatKeyFrameElement, L"coeff_B0");
		}

		float bezierFloatKeyframe::getInterpolatedValueForTime(long time)
		{
			float t = getNormalizedTimeForTime(time);
			return coeff_B0 + t*(coeff_B1 + t*(coeff_B2 + coeff_B3*t));
			//return coeff_B0 + coeff_B1*t + coeff_B2*std::pow(t, 2) + coeff_B3*std::pow(t, 3);
		}

		float bezierFloatKeyframe::getCurrentInterpolatedValue()
		{
			float t = getNormalizedTime();
			return coeff_B0 + t*(coeff_B1 + t*(coeff_B2 + coeff_B3*t));
			//return coeff_B0 + coeff_B1*t + coeff_B2*std::pow(t, 2) + coeff_B3*std::pow(t, 3);
		}
	}
}
