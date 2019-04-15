#pragma once

#include "XMLDocument.h"

/*
class keyFrame{
	???	static Ticker*;
		???	static class member Ticker reference
	int startTime;
	int endTime;
	float getNormalizedTime();
		this function will get the Ticker reference
};
*/

namespace oxyde{
	namespace scene {
		class keyframe {
		protected:
			int startTime;
			int endTime;

		public:

			const int getStartTime() { return startTime; };			
			const int getEndTime() { return endTime; };

			keyframe(const MSXML2::IXMLDOMElementPtr&);

			float getNormalizedTime();
			
			float getNormalizedTimeForTime(long time);
		};

		using keyframePtr = std::shared_ptr<keyframe>;
	}
}
