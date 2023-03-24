#pragma once
#include "../xml/XMLDocument.h"
#include "bone.h"
//#include "dualQuatKeyframe.h"
#include "../animation/dualQuatTrack.h"

/*

BY NOW, we're assuming all keyframe bones are dual quaternion keyframe bones

*/

/*
KeyFrameBone class extends Bone
	
	(static class member, since the Ticker is a Singleton. ENCAPSULATED IN quatKeyFrame)

	A keyframe animated bone will have two subjects: its parent bone and the Ticker.

	int ParentBoneID;
		(Since only the parent's transformation data is needed, this class keeps only the Parent Bone offset in the transformations arrays.)		
		
	std::vector<quatKeyFrame> track;
		No need for a separated track class or structure
		
	int currentKeyFrame;
		updateTransform() overridden to update currentKeyFrame accordling. 

*/

namespace oxyde {
	namespace scene {

		class keyframeBone : public bone {
		protected:

			// Must be kept for global transformation of bones articulated in a hierarchy (skeletons)
			int parentBoneID;

			dualQuatTrack localTransformTrack;

			// std::vector<dualQuatKeyframe> track;
			// int trackSize;

			// int currentKeyframe;

			// Visible only inside class definition. Will keep construction restricted to the factory
			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			keyframeBone(const MSXML2::IXMLDOMNodePtr&, const MSXML2::IXMLDOMElementPtr&, const notAccessible&);

			// transformation virtual method
			virtual void updateTransform() override;

			// Bone class factory
			static bonePtr createKeyframeBone(const MSXML2::IXMLDOMNodePtr&);

			// Register the factory at initialization time
			static void registerKeyframeBoneFactory();
		};
	}
}
