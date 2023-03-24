#pragma once
#include "../xml/XMLDocument.h"
#include "bone.h"
#include "../animation/bezierFloatTrack.h"

namespace oxyde {
	namespace scene {

		class lookAtbone : public bone {
		protected:

			// Must be kept for global transformation of bones articulated in a hierarchy (skeletons)
			int parentBoneID;

			// RollAngle tracks
			bezierFloatTrack RollAngle;
			float currentRollAngle;

			// Position tracks
			bezierFloatTrack XPosition;
			bezierFloatTrack YPosition;
			bezierFloatTrack ZPosition;

			// Visible only inside class definition. Will keep construction restricted to the factory
			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			lookAtbone(const MSXML2::IXMLDOMNodePtr&,
				MSXML2::IXMLDOMElementPtr in_RollAngle,
				MSXML2::IXMLDOMElementPtr in_XPosition,
				MSXML2::IXMLDOMElementPtr in_YPosition,
				MSXML2::IXMLDOMElementPtr in_ZPosition,
				const notAccessible&);

			// transformation virtual method
			virtual void updateTransform() override;

			//get current Roll Angle
			const float getRollAngle() { return currentRollAngle; }

			// Bone class factory
			static bonePtr createlookAtbone(const MSXML2::IXMLDOMNodePtr&);

			// Register the factory at initialization time
			static void registerlookAtboneFactory();
		};
		
		using lookAtbonePtr = std::shared_ptr<lookAtbone>;

	}
}
