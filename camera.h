#pragma once

#ifdef _WIN32
#include <glew.h>
#endif

#include "bone.h"
#include "lookAtBone.h"

namespace oxyde {
	namespace GL {
		namespace renderer {

			//		states:
			//  				| press button   | move mouse    | release button | 
			// _________________|________________|_______________|________________|
			// idle 			| start dragging |				 |				  |
			// start dragging	|				 | keep dragging | iddle		  |
			// keep dragging	|				 | keep dragging | iddle		  |

			enum class dragState { IDLE, START_DRAGGING, KEEP_DRAGGING };
			enum class dragMessage { PRESS_BUTTON, MOVE_MOUSE, RELEASE_BUTTON };

			//	states:
			//					   | key pressed    | start animation   | animation ended   | target moved      |
			//	___________________|________________|___________________|___________________|___________________|
			//	already on target  | changed target |				    |				    | already on target |
			//	changed target     |				| animation runing  |				    |					|
			//	animation runing   |				|				    | already on target | animation runing  |

			enum class targetState { ALREADY_ON_TARGET, CHANGED_TARGET, ANIMATION_RUNNING };
			enum class targetMessage { KEY_PRESSED, START_ANIMATION, ANIMATION_ENDED, TARGET_MOVED };

			class camera {
			public:
				void buildViewportMatrix();
				void buildPerspectiveMatrix();
				void fromWindowToCameraSpaceCoord(float xw, float yw, float & x, float & y);
				void fromWindowToTrackBall(float xw, float yw, float & trackX, float & trackY, float & trackZ);
				void fromCameraSpaceToWindowCoord(float x, float y, float z, float &xw, float &yw);
				void updateCamera();
				void rotateCameraAroundTarget(float owx, float owy, float dewx, float dewy);
				void moveCameraToOrFromTheTarget(short);
				void updateCameraLight();
				//void updateCamPos();


				//camera::keyDown(-1);
				static void keyDown(short);

				//camera::mouseButtonDown(x, y);
				static void mouseButtonDown(int x, int y);

				//camera::mouseMove(x, y);
				static void mouseMove(int x, int y);

				//camera::mouseButtonUp();
				static void mouseButtonUp();

				static void updateTargetPos();

				//camera::mouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
				static void mouseWheel(short);

				void changeState(dragMessage);

				void changeTargetState(targetMessage);

				static void importCameraFromDocument(const MSXML2::IXMLDOMNodePtr& documentElement, long width, long height);

				static void createCamera(float in_f, float in_n, float in_l, float in_r, float in_t, float in_b, float in_w, float in_h,
					int in_cameraNode, int in_targetNode, bool in_thereIsCamera);

				static void resizeWindow(long width, long height);

				void static updateLight();

			protected:
				camera(float in_f, float in_n, float in_l, float in_r, float in_t, float in_b, float in_w, float in_h, 
					GLint in_projLocation, GLint in_worldLocation, GLint in_invWorldLocation, GLint in_viewLocation, GLuint in_lightLocation, 
					int in_cameraNode, int in_targetNode, bool in_thereIsCamera);
				camera() = delete;
				camera(const camera&) = delete;

				void setNewWindowSizeForCamera(long width, long height);

				float f, n, l, r, t, b;
				std::array<float, 16> camMatrix, camMatrixInv;
				std::array<float, 16> perpectiveMatrix;
				std::array<float, 16> viewPortMatrix;
				float w, h;

				float winX, winY;
				float formerX, formerY;
				float currentX, currentY;

				float camX, camY, camZ;
				float targX, targY, targZ;
				float targetZ;

				dragState currentState;
				targetState currentTargetState;
				bool isTargetAnimationRunning;
				short incrementTarget;
				int currentBone;
				long currentTime;
				long startTime;
				int cameraBone;
				int targetBone;
				bool thereIsCamera;

				GLint projLocation; 
				GLint worldLocation; 
				GLint invWorldLocation; 
				GLint viewLocation; 
				GLuint lightLocation;

				oxyde::scene::lookAtbonePtr theCameraLookAtBone;

			private:
				static camera* theInstance;
			};

		}
	}
}
