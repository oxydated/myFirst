#include <set>
#include "spinnerBone.h"
#include "debugLog.h"
#include "linAlg.h"

namespace oxyde {
	namespace scene {

		/////////////// Spinner Tip Bone

		spinnerTipBone::spinnerTipBone(const MSXML2::IXMLDOMNodePtr & theNode, const notAccessible &) :bone(MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../..")))
		{
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");

				std::wstring theSourceQueryString = L"//node[@nodeName='" + 
					oxyde::XML::getWStringAttributeFromElement(theNode, L"source")+
					L"'] [1]";

				const MSXML2::IXMLDOMElementPtr theSourceElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(theSourceQueryString.data()));

				sourceID = oxyde::XML::getIntAttributeFromElement(theSourceElement, "nodeObject");

				std::set<int> setToKeepObservedIDsUnique({ rootNodeObject, parentBoneID, sourceID });
				listOfObservedBones = std::vector<int>(setToKeepObservedIDsUnique.begin(), setToKeepObservedIDsUnique.end());
				listOfObservedBones.shrink_to_fit();

				newBoneTipLocalTransform[0] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qs");
				newBoneTipLocalTransform[1] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qx");
				newBoneTipLocalTransform[2] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qy");
				newBoneTipLocalTransform[3] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qz");
				newBoneTipLocalTransform[4] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqs");
				newBoneTipLocalTransform[5] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqx");
				newBoneTipLocalTransform[6] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqy");
				newBoneTipLocalTransform[7] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqz");

				newBoneTipLocalTransform[0] = -newBoneTipLocalTransform[0];
				newBoneTipLocalTransform[1] = -newBoneTipLocalTransform[1];
				newBoneTipLocalTransform[2] = -newBoneTipLocalTransform[2];
				newBoneTipLocalTransform[3] = -newBoneTipLocalTransform[3];
				newBoneTipLocalTransform[4] = -newBoneTipLocalTransform[4];
				newBoneTipLocalTransform[5] = -newBoneTipLocalTransform[5];
				newBoneTipLocalTransform[6] = -newBoneTipLocalTransform[6];
				newBoneTipLocalTransform[7] = -newBoneTipLocalTransform[7];
			}
		}

		void spinnerTipBone::updateTransform()
		{
			//dualQuat localTransform;
			dualQuat &boneGlobalTransform = boneTransformation[nodeObject];
			dualQuat &sourceGlobalTransform = boneTransformation[sourceID];

			//   --> newBoneTip_local_transform*theSource.transform * (inverse theParent.transform)

			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(sourceGlobalTransform),
				DUALQUAARRAY(newBoneTipLocalTransform),
				DUALQUAARRAY(boneGlobalTransform));

			//oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(newBoneTipLocalTransform),
			//	DUALQUAARRAY(sourceGlobalTransform),
			//	DUALQUAARRAY(boneGlobalTransform));

			oxyde::log::printText(L"update bone: " + std::to_wstring(this->nodeObject));
		}

		bonePtr spinnerTipBone::createSpinnerTipBone(const MSXML2::IXMLDOMNodePtr & theNode)
		{
			//std::wstring queryForAnyChild(L"./*");
			//MSXML2::IXMLDOMNodeListPtr theChildList = theNode->selectNodes(queryForAnyChild.data());

			//if (theChildList->length > 0) {
			//	return std::make_shared<spinnerTipBone>(theNode, notAccessible());
			//}

			//return nullptr;

			return std::make_shared<spinnerTipBone>(theNode, notAccessible());
		}

		void spinnerTipBone::registerSpinnerTipBoneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"spinnerTip", createSpinnerTipBone);
		}

		/////////////// Spinner Base Bone

		spinnerBaseBone::spinnerBaseBone(const MSXML2::IXMLDOMNodePtr & theNode, const notAccessible &) :bone(MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../..")))
		{
			const MSXML2::IXMLDOMElementPtr theParentElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(L"./../../.."));
			if (theParentElement) {
				std::wstring test(MSXML2::IXMLDOMNodePtr(theParentElement)->xml);
				parentBoneID = oxyde::XML::getIntAttributeFromElement(theParentElement, "nodeObject");

				std::wstring theSourceQueryString = L"//node[@nodeName='" +
					oxyde::XML::getWStringAttributeFromElement(theNode, L"source") +
					L"'] [1]";

				const MSXML2::IXMLDOMElementPtr theSourceElement = MSXML2::IXMLDOMElementPtr(theNode->selectSingleNode(theSourceQueryString.data()));

				sourceID = oxyde::XML::getIntAttributeFromElement(theSourceElement, "nodeObject");

				std::set<int> setToKeepObservedIDsUnique({ rootNodeObject, parentBoneID, sourceID });
				listOfObservedBones = std::vector<int>(setToKeepObservedIDsUnique.begin(), setToKeepObservedIDsUnique.end());
				listOfObservedBones.shrink_to_fit();

				float qs, qx, qy, qz, dqs, dqx, dqy, dqz;

				qs = localTransformForNewBoneOnCreation[0] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qs");
				qx = localTransformForNewBoneOnCreation[1] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qx");
				qy = localTransformForNewBoneOnCreation[2] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qy");
				qz = localTransformForNewBoneOnCreation[3] = oxyde::XML::getFloatAttributeFromElement(theNode, L"qz");
				dqs = localTransformForNewBoneOnCreation[4] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqs");
				dqx = localTransformForNewBoneOnCreation[5] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqx");
				dqy = localTransformForNewBoneOnCreation[6] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqy");
				dqz = localTransformForNewBoneOnCreation[7] = oxyde::XML::getFloatAttributeFromElement(theNode, L"dqz");

				//qs = -qs;
				//qx = -qx;
				//qy = -qy;
				//qz = -qz;
				//dqs = -dqs;
				//dqx = -dqx;
				//dqy = -dqy;
				//dqz = -dqz;

				Q0 = -2 * dqs*qx + 2 * dqs*std::pow(qs, 2)*qx + 2 * dqs*std::pow(qx, 3) - 2 * dqz*qy + 2 * dqz*std::pow(qs, 2)*qy + 2 * dqz*std::pow(qx, 2)*qy +
					2 * dqs*qx*std::pow(qy, 2) + 2 * dqz*std::pow(qy, 3) + 2 * dqy*qz - 2 * dqy*std::pow(qs, 2)*qz - 2 * dqy*std::pow(qx, 2)*qz -
					2 * dqy*std::pow(qy, 2)*qz + 2 * dqs*qx*std::pow(qz, 2) + 2 * dqz*qy*std::pow(qz, 2) - 2 * dqy*std::pow(qz, 3);
				Q1 = -std::pow(qs, 2) - std::pow(qx, 2) + std::pow(qy, 2) + std::pow(qz, 2);
				Q2 = qs*qy - qx*qz;
				Q3 = qx*qy + qs*qz;

				Qy0 = 2 * dqy*qx - 2 * dqy*std::pow(qs, 2)*qx - 2 * dqy*std::pow(qx, 3) - 2 * dqx*qy + 2 * dqx*std::pow(qs, 2)*qy + 2 * dqx*std::pow(qx, 2)*qy -
					2 * dqy*qx*std::pow(qy, 2) + 2 * dqx*std::pow(qy, 3) + 2 * dqs*qz - 2 * dqs*std::pow(qs, 2)*qz - 2 * dqs*std::pow(qx, 2)*qz -
					2 * dqs*std::pow(qy, 2)*qz - 2 * dqy*qx*std::pow(qz, 2) + 2 * dqx*qy*std::pow(qz, 2) - 2 * dqs*std::pow(qz, 3);
				Qy1 = -2 * qs*qx + 2 * qy*qz;
				Qy2 = -2 * qs*qy - 2 * qx*qz;
				Qy3 = -2 * std::pow(qs, 2) + 2 * std::pow(qx, 2) + 2 * std::pow(qy, 2) - 2 * std::pow(qz, 2);
				Qz0 = 2 * dqz*qx - 2 * dqz*std::pow(qs, 2)*qx - 2 * dqz*std::pow(qx, 3) - 2 * dqs*qy + 2 * dqs*std::pow(qs, 2)*qy + 2 * dqs*std::pow(qx, 2)*qy -
					2 * dqz*qx*std::pow(qy, 2) + 2 * dqs*std::pow(qy, 3) - 2 * dqx*qz + 2 * dqx*std::pow(qs, 2)*qz + 2 * dqx*std::pow(qx, 2)*qz +
					2 * dqx*std::pow(qy, 2)*qz - 2 * dqz*qx*std::pow(qz, 2) + 2 * dqs*qy*std::pow(qz, 2) + 2 * dqx*std::pow(qz, 3);
				Qz1 = -std::pow(qs, 2) + std::pow(qx, 2) - std::pow(qy, 2) + std::pow(qz, 2);
				Qz2 = 2 * qx*qy - 2 * qs*qz;
				Qz3 = 4 * qs*qx + 4 * qy*qz;
			}
		}

		void spinnerBaseBone::updateTransform()
		{
			float gs, gx, gy, gz, dgs, dgx, dgy, dgz;
			dualQuat &currentLocalSourceTransformation = boneLocalTransformation[sourceID];
			gs = currentLocalSourceTransformation[0];
			gx = currentLocalSourceTransformation[1];
			gy = currentLocalSourceTransformation[2];
			gz = currentLocalSourceTransformation[3];
			dgs = currentLocalSourceTransformation[4];
			dgx = currentLocalSourceTransformation[5];
			dgy = currentLocalSourceTransformation[6];
			dgz = currentLocalSourceTransformation[7];

			//gs = -gs;
			//gx = -gx;
			//gy = -gy;
			//gz = -gz;
			//dgs = -dgs;
			//dgx = -dgx;
			//dgy = -dgy;
			//dgz = -dgz;

			float G1 = -std::pow(gs, 2) - std::pow(gx, 2) + std::pow(gy, 2) + std::pow(gz, 2);
			float G2 = 4 * gs*gy - 4 * gx*gz;
			float G3 = 4 * gx*gy + 4 * gs*gz;

			float cosAlpha = Q0 + G1*Q1 + G2*Q2 + G3*Q3;

			float Gv1 = -2 * gx*gy - 2 * gs*gz;
			float Gv2 = std::pow(gs, 2) + std::pow(gx, 2) - std::pow(gy, 2) - std::pow(gz, 2);
			float Gv3 = -(gs*gy) + gx*gz;

			float unx = 0;
			float uny = Qy0 + Gv1*Qy1 + Gv2*Qy2 + Gv3*Qy3;
			float unz = Qz0 + Gv1*Qz1 + Gv2*Qz2 + Gv3*Qz3;

			float sinAlpha = std::sqrt(std::pow(unx, 2) + std::pow(uny, 2) + std::pow(unz, 2));

			float nx = unx / sinAlpha;
			float ny = uny / sinAlpha;
			float nz = unz / sinAlpha;

			float halfCosAlpha = 0;
			float halfSinAlpha = 0;

			oxyde::linAlg::halfAngleCos(cosAlpha, &halfCosAlpha);
			oxyde::linAlg::halfAngleSin(sinAlpha, &halfSinAlpha);

			float os, ox, oy, oz, dos, dox, doy, doz;
			oxyde::DQ::pure_Real_quaternion(halfCosAlpha, halfSinAlpha*nx, halfSinAlpha*ny, halfSinAlpha*nz, os, ox, oy, oz, dos, dox, doy, doz);

			

			float rs, rx, ry, rz, drs, drx, dry, drz;
			oxyde::DQ::dual_Versor(std::acos(cosAlpha), nx, ny, nz, 0, 0, 0, 0, rs, rx, ry, rz, drs, drx, dry, drz);

			float ss = -rs, sx = -rx, sy = -ry, sz = -rz, dss = -drs, dsx = -drx, dsy = -dry, dsz = -drz;

			dualQuat localTransform;

			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(localTransformForNewBoneOnCreation),
				DUALQUACOMP(s),
				DUALQUAARRAY(localTransform));

			//oxyde::DQ::dual_quaternion_product(DUALQUACOMP(o), 
			//	DUALQUAARRAY(localTransformForNewBoneOnCreation),
			//	DUALQUAARRAY(localTransform));

			//localTransform = { -gs, -gx, -gy, -gz, -dgs, -dgx, -dgy, -dgz };
			//localTransform = { gs, gx, gy, gz, dgs, dgx, dgy, dgz };

			boneLocalTransformation[nodeObject] = localTransform;

			dualQuat &boneGlobalTransform = boneTransformation[nodeObject];
			dualQuat &parentGlobalTransform = boneTransformation[parentBoneID];

			oxyde::DQ::dual_quaternion_product(DUALQUAARRAY(parentGlobalTransform),
				DUALQUAARRAY(localTransform),
				DUALQUAARRAY(boneGlobalTransform));
		}

		bonePtr spinnerBaseBone::createSpinnerBaseBone(const MSXML2::IXMLDOMNodePtr & theNode)
		{
			//std::wstring queryForAnyChild(L"./*");
			//MSXML2::IXMLDOMNodeListPtr theChildList = theNode->selectNodes(queryForAnyChild.data());

			//if (theChildList->length > 0) {
			//	return std::make_shared<spinnerBaseBone>(theNode, notAccessible());
			//}

			//return nullptr;


			return std::make_shared<spinnerBaseBone>(theNode, notAccessible());
		}

		void spinnerBaseBone::registerSpinnerBaseBoneFactory()
		{
			boneFactory::boneFactory::registerBoneConcreteFactory(L"spinnerBase", createSpinnerBaseBone);
		}

	}
}

namespace {
	class executeBeforeMain {
	public:
		explicit executeBeforeMain() {
			oxyde::scene::spinnerTipBone::registerSpinnerTipBoneFactory();
			oxyde::scene::spinnerBaseBone::registerSpinnerBaseBoneFactory();
		}
	};

	executeBeforeMain instance = executeBeforeMain();
}
