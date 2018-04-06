#include "observer.h"

namespace oxyde {
	namespace scene {

		observer::observer(subjectPtr theSubject, bonePtr theBone):subjectForChildren(theSubject), theControlledBone(theBone)
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
	}
}
