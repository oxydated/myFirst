#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "observer.h"

/*

Subject class

	Must know Observer class
	
	Constructor receives a pointer to Scene object
		Subject must have access to the Stack object in Scene
	
	For now, the only relevant event to be treated here is UPDATED, which should cause the observers to update themselves as well.
	
	attachObserver( smartPtr Observer);
		Will add the observer ptr to its observers vector
	
	std::vector<ObserverPtr> observers;
	
	pushObserversToStack();
		Public method called by Observer. It'll push this Subject instance's observers onto the Stack.
	
	update();
		Will push the pointers of its observers vector to the observers stack in the Scene object

	All Animated place in the map will be disposed at the same time, so there's no need for a "detach" event, or a detach method.


*/

namespace oxyde {
	namespace scene {
		class subject {
		public:
			subject(std::function<void(observerPtr)>);

			void attachObserver(observerPtr);

			void update();

		private:
			std::vector<observerPtr> observers;
			std::function<void(observerPtr)> updateObservers;
		};
	}
}

