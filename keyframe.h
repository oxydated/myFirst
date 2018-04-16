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

			keyframe(const MSXML2::IXMLDOMElementPtr&);

			float getNormalizedTime();
		};
	}
}
