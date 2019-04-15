#pragma once
#include "XMLDocument.h"
#include "bone.h"
#include "bezierFloatTrack.h"
#include "dualQuaternionEulerAngles.h"

namespace oxyde {
	namespace scene {

		class PRSbone : public bone {
		protected:

			// Must be kept for global transformation of bones articulated in a hierarchy (skeletons)
			int parentBoneID;

			// Rotation dual quaternion from Euler Angle
			oxyde::DQ::quatFromEulerFunc quatFromEuler;

			// Position tracks
			bezierFloatTrack XPosition;
			bezierFloatTrack YPosition;
			bezierFloatTrack ZPosition;

			// Rotation tracks
			bezierFloatTrack XRotation;
			bezierFloatTrack YRotation;
			bezierFloatTrack ZRotation;

			// Visible only inside class definition. Will keep construction restricted to the factory
			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			PRSbone(const MSXML2::IXMLDOMNodePtr&,
				MSXML2::IXMLDOMElementPtr in_XPosition,
				MSXML2::IXMLDOMElementPtr in_YPosition,
				MSXML2::IXMLDOMElementPtr in_ZPosition,
				MSXML2::IXMLDOMElementPtr in_XRotation,
				MSXML2::IXMLDOMElementPtr in_YRotation,
				MSXML2::IXMLDOMElementPtr in_ZRotation,
				oxyde::DQ::quatFromEulerFunc in_quatFromEuler,
				const notAccessible&);

			// transformation virtual method
			virtual void updateTransform() override;

			// Bone class factory
			static bonePtr createPRSbone(const MSXML2::IXMLDOMNodePtr&);

			// Register the factory at initialization time
			static void registerPRSboneFactory();
		};

	}
}
