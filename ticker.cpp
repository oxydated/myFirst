#include "ticker.h"

namespace oxyde {
	namespace scene {

		ticker ticker::theSingletonInstance;

		void ticker::setLoopSize(int size)
		{
			theSingletonInstance.loopSize = size;
		}

		void ticker::start()
		{
			theSingletonInstance.startTime = clock::now();
			theSingletonInstance.update();
		}

		void ticker::update()
		{
			theSingletonInstance.currentTime = std::chrono::duration_cast<duration>(clock::now() - theSingletonInstance.startTime);
		}

		void ticker::setCurrentTime(int newTime) {
			theSingletonInstance.currentTime = duration(newTime);
		}

		long ticker::getCurrentTime()
		{
			return theSingletonInstance.currentTime.count() % theSingletonInstance.loopSize;
		}
	}
}
