#pragma once

#include <string>
#include <memory>
#include <map>
#include <array>
#include <stack>
#include "XMLDocument.h"
#include "observer.h"
#include "subject.h"

	/*
	class scene
	Keeps all bones in maps and vectors

	BoneNamesById
	Will keep a vector of observer Ptrs sorted by nodeObject.

	A vector of pairs formed by subject bone names and observer bone names is built as well.
	std::vector<std::pair<subjectName, observerName>> subjectsAndItsObservers;
	Each bone must provide it's subjects names when constructed. (Factory)

	Next, each item in this vector is visited. The pair's subject attaches the pair's observer. The vector can be disposed, then.
	(bones are kept in bonesByName)

	updateFrame();
	Keeps a stack of observers shared_ptr to be called.
	This stack is empty at the begining of every frame.
	The Ticker's subject push it's observers's pointers to the stack
	The While loop starts and will only end when the stack become empty.
	The observer pt at the top of the stack has its updateCallback() called;
	If its respective subject has observers to be notified, it'll put their pointers in the stack to be called as well.

	create( XMLDocument);
	Create the scene by loading the bones and creating the vectors


	The dual quaternion data for Global and Skin Pose transformation will be held within arrays indexed by Bone ID.

	*/
namespace oxyde {
	namespace scene {

		class scene: std::enable_shared_from_this<scene> {
		protected:
			subjectPtr theTickerSubject;
			std::map<int, observerPtr> bonesPernodeObject;
			std::map<int, std::wstring> boneNamesPerNodeObject;
			std::stack<observerPtr> observersToUpdate;

			static std::shared_ptr<scene> instance;

			class notAccessible {
			public:
				explicit notAccessible() = default;
			};

		public:
			scene(const MSXML2::IXMLDOMNodePtr& sceneNode, const notAccessible&);
			void updateFrame();

			static std::shared_ptr<scene> createScene(const MSXML2::IXMLDOMNodePtr&  sceneNode);
			static std::shared_ptr<scene> getScene();
		};

		using scenePtr = std::shared_ptr<scene>;



	}
}