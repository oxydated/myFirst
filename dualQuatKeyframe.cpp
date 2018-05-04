#include "dualQuatKeyframe.h"

/*
	<dualQuatKey startTime="0" endTime="0">
		<startingDualQuat qs="0.194321" qx="0.682914" qy="-0.677293" qz="0.192722" dqs="-4.094089" dqx="14.388097" dqy="14.507496" dqz="4.128065"/>
		<interpolationParam angle="0.000000" ux="1.000000" uy="0.000000" uz="0.000000" slide="0.000000" mx="0.000000" my="0.000000" mz="0.000000"/>
	</dualQuatKey>
*/

namespace oxyde {
	namespace scene {
		dualQuatKeyframe::dualQuatKeyframe(const MSXML2::IXMLDOMElementPtr &dualQuatKeyframeElement):keyframe(dualQuatKeyframeElement)
		{
			MSXML2::IXMLDOMNodePtr theNode = MSXML2::IXMLDOMNodePtr(dualQuatKeyframeElement);
			const MSXML2::IXMLDOMElementPtr startingDualQuatElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./startingDualQuat"));
			const MSXML2::IXMLDOMElementPtr interpolationParamElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./interpolationParam"));

			startTransform[0] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"qs");
			startTransform[1] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"qx");
			startTransform[2] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"qy");
			startTransform[3] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"qz");
			startTransform[4] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"dqs");
			startTransform[5] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"dqx");
			startTransform[6] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"dqy");
			startTransform[7] = oxyde::XML::getFloatAttributeFromElement(startingDualQuatElement, L"dqz");
			
			theAngle = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"angle");
			theUvector[0] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"ux");
			theUvector[1] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"uy");
			theUvector[2] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"uz");
			theSfactor = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"slide");
			theMvector[0] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"mx");
			theMvector[1] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"my");
			theMvector[2] = oxyde::XML::getFloatAttributeFromElement(interpolationParamElement, L"mz");
		}
		int dualQuatKeyframe::getInterpolatedQuaternion(dualQuat &q)
		{
			int retValue = 0;
			float step = this->getNormalizedTime();
			if (step > 1.0) {
				retValue = 1;
			}
			else {
				if (step < 0.0) {
					retValue = -1;
				}
				else {

					//######################################################
					//# calculate the weighted motion from the quaternion at the beginning of the interval

					dualQuat r;
					//dual_Versor(float theta, float ux, float uy, float uz, float s, float mx, float my, float mz, DUALQUAARG(r));
					oxyde::DQ::dual_Versor(theAngle * step,
						theUvector[0], theUvector[1], theUvector[2],
						theSfactor * step,
						theMvector[0], theMvector[1], theMvector[2],
						DUALQUAARRAY(r));

					//float* curr = startTransform;

					oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(r),
						DUALQUAARRAY(startTransform),
						DUALQUAARRAY(q));
				}
			}
			return retValue;

		}

		void dualQuatKeyframe::getStartTransformation(dualQuat &startQuat)
		{
			startQuat[0] = startTransform[0];
			startQuat[1] = startTransform[1];
			startQuat[2] = startTransform[2];
			startQuat[3] = startTransform[3];
			startQuat[4] = startTransform[4];
			startQuat[5] = startTransform[5];
			startQuat[6] = startTransform[6];
			startQuat[7] = startTransform[7];
		}

		void dualQuatKeyframe::getEndTransformation(dualQuat &endQuat)
		{
			dualQuat r;
			
			oxyde::DQ::dual_Versor(theAngle,
				theUvector[0], theUvector[1], theUvector[2],
				theSfactor,
				theMvector[0], theMvector[1], theMvector[2],
				DUALQUAARRAY(r));
			
			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(r),
				DUALQUAARRAY(startTransform),
				DUALQUAARRAY(endQuat));
		}
	}
}
