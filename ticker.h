#pragma once
#include <memory>

/*
Ticker class 

	Will be a singleton, known by every instanced keyframe object.
	
	Ticker will have a Subject that will send UPDATED to its observers at every new frame.
	
	int update();
		Executed once for each new frame. Update the internal time on a frame-by-frame basis.
		All keyframe objects will read the same time value in a frame.
		
	int currentTime();
		Returns the time for the current frame.

*/

namespace oxyde {
	namespace scene {

		class ticker: std::enable_shared_from_this<ticker> {
		protected:
			ticker();

		public:
			void update();
			int currentTime();

			static std::shared_ptr<ticker> getTicker();
			static std::shared_ptr<ticker> theSingletonInstance;
		};

	}
}
