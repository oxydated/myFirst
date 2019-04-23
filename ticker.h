#pragma once

#include <chrono>
#include <ratio>

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

		using clock = std::chrono::high_resolution_clock;
		using timePoint = std::chrono::time_point<clock>;
		//using duration = std::chrono::microseconds;
		using duration = std::chrono::duration<int, std::ratio<1, 1200>>;

		class ticker {
		protected:
			ticker() = default;
			timePoint startTime;
			duration currentTime;
			int loopSize;

			static ticker theSingletonInstance;

		public:
			static void setLoopSize(int);
			static void start();
			static void update();
			static void setCurrentTime(int newTime);
			static long getCurrentTime();
		};
	}
}

