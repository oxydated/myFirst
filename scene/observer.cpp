#include "observer.h"
#include "subject.h"

namespace oxyde {
	namespace scene {

		observer::observer(subjectPtr theSubject, bonePtr theBone):subjectForChildren(theSubject), theControlledBone(theBone), numObserved(0), calledByHowManySoFar(0)
		{
		}

		subjectPtr observer::getPairedSubject()
		{
			return subjectForChildren;
		}

		bonePtr observer::getBone()
		{
			return theControlledBone;
		}
		void observer::attachIt()
		{
			numObserved++;
		}
		void observer::updateCallback()
		{
			//				This function increment the Subject called counter in the Observer.
			calledByHowManySoFar++;
			if (calledByHowManySoFar == numObserved) {
				//				This method calls UpdateTransform() from the bound bone
				theControlledBone->updateTransform();
				//					If all of this object's observed Subjects have called on this Observer ( the counter will be equal to the number of observed Subjects):
				//						This function will call Update() from this Observer's bound Subject.
				subjectForChildren->update();
				//							That'll push all of this Subjects's Observer pointers, if any, to the Scene stack.
				calledByHowManySoFar = 0;
			}
		}
	}
}
