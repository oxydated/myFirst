#pragma once
#include <memory>
#include "bone.h"
	
/*

Observer class
	Must know Subject class;
	
	bool updateCallback();
		Called during the while loop in the Scene object.
		must keep track of how many of its subjects have called it
			Internal counter will count up to the number of subjects.
			Incremented at every call to updateCallback()
			reset AFTER it reaches the number of subjects and updateCallback() calls
				updateTransform() from its bone and update() from its paired subject
		If there still are subjects to be processed before this observer, this function will return a null pointer,
			it'll NEITHER call updateTransform() nor update().
		Otherwise, it will call its bone's updateTransform() and its paired subject's update().
		Anyway, the top will be removed from stack. This Observer will return to the stack for every 
			one of its subjects untill the last one.
		Since Subject has a pointer to the Scene object, that's not necessary.
			
	Subject *subjectForChildren;
	
	int subjectNum;
		Number of subjects for this observer.
		
	std::vector<SubjectPtr> observedSubjects();
		Based on bone subclass controlled by this Observer.
	
	BonePtr theBone;
		The associated bone, whose UpdateTransform() callback this Observer will call.
		This pointer is null if the Observer was constructed with no bones That's the case for the Ticker's Observer.
		
NOTES:
	Once an Animated received an UPDATED message from its subject, it will read the subjects state, update its own state accordling,
	and issue an UPDATED message to its own observers, causing a chain reaction.
	
	Each observer is paired to a Bone object. Bone update is issued directly by the observer.

	Each observer is also paired to a Subject object. After updating the bone, the observer object will cause
		its paired subject to update its peers.

*/
	

namespace oxyde {
	namespace scene {

		class subject;
		using subjectPtr = std::shared_ptr<subject>;

		class observer {
		public:
			observer(subjectPtr, bonePtr);

			subjectPtr getPairedSubject();
			bonePtr getBone();
			void attachIt();
			void updateCallback();

		protected:
			subjectPtr subjectForChildren;
			int numObserved;
			int calledByHowManySoFar;
			bonePtr theControlledBone;

		};

		using observerPtr = std::shared_ptr<observer>;
	}
}
