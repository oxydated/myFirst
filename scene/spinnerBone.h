#pragma once

#include "bone.h"
#include "../xml/XMLDocument.h"
#include "dualQuaternionFunctions.h"

namespace oxyde {
	namespace scene {

		class spinnerTipBone : public bone {
		protected:
			int parentBoneID;
			int sourceID;

			dualQuat newBoneTipLocalTransform;

			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			spinnerTipBone(const MSXML2::IXMLDOMNodePtr&, const notAccessible&);
			virtual void updateTransform();

			static bonePtr createSpinnerTipBone(const MSXML2::IXMLDOMNodePtr&);
			static void registerSpinnerTipBoneFactory();
		};

		class spinnerBaseBone : public bone {
		protected:
			int parentBoneID;
			int sourceID;

			dualQuat localTransformForNewBoneOnCreation;
			float Q0, Q1, Q2, Q3;
			float Qy0, Qy1, Qy2, Qy3, Qz0, Qz1, Qz2, Qz3;

			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			spinnerBaseBone(const MSXML2::IXMLDOMNodePtr&, const notAccessible&);
			virtual void updateTransform();

			static bonePtr createSpinnerBaseBone(const MSXML2::IXMLDOMNodePtr&);
			static void registerSpinnerBaseBoneFactory();
		};
	}
}
