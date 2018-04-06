#include "subject.h"

namespace oxyde {
	namespace scene {

		subject::subject(std::function<void(observerPtr)> theUpdateObserverFunction): updateObservers(theUpdateObserverFunction)
		{
		}

		void subject::attachObserver(observerPtr theObserver)
		{
			observers.push_back(theObserver);
		}
	}
}
