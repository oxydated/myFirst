#include <Windows.h>
#include <stdio.h>
#import<msxml6.dll>
#include "scene.h"
#include "bone.h"
#include "subject.h"
#include "ticker.h"
#include "debugLog.h"

namespace oxyde {
	namespace scene {

		scenePtr scene::instance = nullptr;

		scene::scene(const MSXML2::IXMLDOMNodePtr &  sceneNode, const notAccessible&)
		{
			std::function<void(observerPtr)> updateObserversFunction = [this](observerPtr theObserver)->void{
				this->observersToUpdate.push(theObserver);
			};

			theTickerSubject = std::make_shared<subject>(updateObserversFunction);

			//BONES CREATION:

			//	Starting from:
			//		< ? xml version = "1.0" encoding = "UTF-8" ? >
			//			<document>
			//				<scene>

			//	Get the Root ID

			int rootID = 0;
			std::wstring searchForSceneRootNodeObject = L"./node";
			MSXML2::IXMLDOMElementPtr sceneRootElement(sceneNode->selectSingleNode(searchForSceneRootNodeObject.data()));
			if (sceneRootElement) {
				rootID = oxyde::XML::getIntAttributeFromElement(sceneRootElement, L"nodeObject");
				bone::setRootNodeObject(rootID);
			}


			//	Loop over 
			//				<node nodeName="Bone001" nodeObject="2">
			std::wstring searchForBonesNodes = L".//node[./animaton]";
			MSXML2::IXMLDOMNodeListPtr boneNodes =  sceneNode->selectNodes(searchForBonesNodes.data());
			for (int i = 0; i < boneNodes->length; i++) {
				
				//		Pass down
				//			<animaton>
				//						<keyFrames\>
				//			<animaton\>
				//			to the Bones Factory.

				//				Bone Factory decide what concrete factory it will use
				//				The created bone has a list of the bones it observes
				//				the bone is returned to the loop

				bonePtr newBone = boneFactory::boneFactory::createBone(boneNodes->item[i]);
	
				//		An Observer object is created with the created bone as constructor parameter
				//			Now this Observer is bound to that bone
				//			Every Observer object has a Subject object as a member
				//		This Observer is put in a map indexed by "nodeObject" (the bone ID)
				if (newBone) {
					int nodeObject = 0;
					const MSXML2::IXMLDOMElementPtr nodeElement(boneNodes->item[i]);
					if (nodeElement) {
						nodeObject = oxyde::XML::getIntAttributeFromElement(nodeElement, L"nodeObject");
						bonesPernodeObject[nodeObject] = std::make_shared<observer>(std::make_shared<subject>(updateObserversFunction), newBone);
						boneNamesPerNodeObject[nodeObject] = oxyde::XML::getWStringAttributeFromElement(nodeElement, L"nodeName");
					}
				}
			}
	
			//	Loop over the Observer map indexed by "nodeObject". For each Observer Object in the map:
			for (auto i = bonesPernodeObject.begin(); i != bonesPernodeObject.end(); i++) {
				//		Keep the pointer to this Observer
				//		Retrieve the list of bones this Observer observes (the list of nodeObjects is provided by the bound bone)
				const std::vector<int>& listOfObservedBones = i->second->getBone()->getListOfObservedBones();
				//		This Observer keeps the number of entries in this list in a member variable as the number of observed bones.
				//		For each nodeObject in the list:
				for (int boneID : listOfObservedBones) {
				//			IF( the current nodeObject is not the Scene Root nodeObject):
					if (boneID != rootID) {
						//				Get the respective Observer object in the map indexed by "nodeObject"
						//				Get the Subject bound to the later Observer object
						//				Attach the current Observer to this Subject by the Observer's pointer
						bonesPernodeObject[boneID]->getPairedSubject()->attachObserver(i->second);
						//					the Subject will keep these pointers in a internal vector
					}
					else {
						//
						//			ELSE:
						//				Attach the current Observer to the Ticker's Subject (a member of the Scene object)
						theTickerSubject->attachObserver(i->second);
						//
					}
				}
			}
			//	Allocate vector of dualquaternions with size of (last key in the map + 1)
			int sizeOfVectors = bonesPernodeObject.rbegin()->first + 1;
			bone::setTransformationVector(sizeOfVectors);
		}

		void scene::updateFrame()
		{
			/*
			updateFrame();
			Keeps a stack of observers shared_ptr to be called.
			This stack is empty at the begining of every frame.
			The Ticker's subject push it's observers's pointers to the stack
			The While loop starts and will only end when the stack become empty.
			The observer pt at the top of the stack has its updateCallback() called;
			If its respective subject has observers to be notified, it'll put their pointers in the stack to be called as well.

			====================

			FRAME LOOP:

			*/

	//		The Ticker get the time value for the current frame from the system clock.
	//			This value is the same for all the frame

			//ticker::update();
	//
	//		The Ticker's Subject - a member of the Scene Object - Push all it's Observers pointers to the Scene stack
			theTickerSubject->update();

			oxyde::log::printText(L"entering frame loop");
	//
	//		while the stack is not empty:
			while (!observersToUpdate.empty()) {
				//			Pop the Observer pointer at the top
				//			Call UpdateCallback() from the pointer
				observerPtr theNextObserver = observersToUpdate.top();
				observersToUpdate.pop();
				theNextObserver->updateCallback();
				//				This method calls UpdateTransform() from the bound bone
				//				This function increment the Subject called counter in the Observer.
				//					If all of this object's observed Subjects have called on this Observer ( the counter will be equal to the number of observed Subjects):
				//						This function will call Update() from this Observer's bound Subject.
				//							That'll push all of this Subjects's Observer pointers, if any, to the Scene stack.
			}

			bone::printBones();
			oxyde::log::printText(L"exiting frame loop");
			bone::updateFrame();
	//					
	//		The skin transformation and drawing routine are then executed.
	//		SkinData::UpdateSkinPose() is called
		}

		std::shared_ptr<scene> scene::createScene(const MSXML2::IXMLDOMNodePtr & sceneNode)
		{
			instance = std::make_shared<scene>(sceneNode, notAccessible());
			return instance;
		}

		std::shared_ptr<scene> scene::getScene()
		{
			return instance->shared_from_this();
		}

	}
}
