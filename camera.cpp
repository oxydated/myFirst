#include "camera.h"
#include "linearAlg.h"
#include "ticker.h"
#include "bone.h"
#include "debugLog.h"

namespace oxyde {
	namespace GL {
		namespace renderer {

			camera* camera::theInstance = nullptr;

			void camera::buildPerspectiveMatrix()
			{
				perpectiveMatrix[0] = (-2 * n) / (l - r);
				perpectiveMatrix[4] = 0;
				perpectiveMatrix[8] = (l + r) / (l - r);
				perpectiveMatrix[12] = 0;
				perpectiveMatrix[1] = 0;
				perpectiveMatrix[5] = (-2 * n) / (b - t);
				perpectiveMatrix[9] = (b + t) / (b - t);
				perpectiveMatrix[13] = 0;
				perpectiveMatrix[2] = 0;
				perpectiveMatrix[6] = 0;
				perpectiveMatrix[10] = f / (f - n);
				perpectiveMatrix[14] = -((f*n) / (f - n));
				perpectiveMatrix[3] = 0;
				perpectiveMatrix[7] = 0;
				perpectiveMatrix[11] = 1;
				perpectiveMatrix[15] = 0;

				glUniformMatrix4fv(projLocation, 1, GL_FALSE, perpectiveMatrix.data());
			}

			void camera::buildViewportMatrix() {

				viewPortMatrix[0] = 1.0;			viewPortMatrix[4] = 0.0;            viewPortMatrix[8] = 0.0;        viewPortMatrix[12] = 0;
				viewPortMatrix[1] = 0.0;            viewPortMatrix[5] = w / h;			viewPortMatrix[9] = 0.0;        viewPortMatrix[13] = 0;
				viewPortMatrix[2] = 0.0;            viewPortMatrix[6] = 0.0;            viewPortMatrix[10] = 1.0;		viewPortMatrix[14] = 0.0;
				viewPortMatrix[3] = 0.0;            viewPortMatrix[7] = 0.0;            viewPortMatrix[11] = 0.0;		viewPortMatrix[15] = 1.0;

				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewPortMatrix.data());
			}

			void camera::fromWindowToCameraSpaceCoord( float xw, float yw, float &x, float &y )
			{
				x = l + ((-l + r)*xw) / w;
				y = t + ((b - t)*yw) / h;
			}

			void camera::fromWindowToTrackBall(float xw, float yw, float &trackX, float &trackY, float &trackZ)
			{
				float x, y;
				fromWindowToCameraSpaceCoord(xw, yw, x, y);

				float radius = (std::sqrt(std::pow(l, 2) + std::pow(t, 2))*targetZ) / std::sqrt(std::pow(l, 2) + std::pow(n, 2) + std::pow(t, 2));


				float vectorMultiplier = (n*targetZ - std::sqrt(-(std::pow(targetZ, 2)*(std::pow(x, 2) + std::pow(y, 2))) 
					+ std::pow(radius, 2)*(std::pow(n, 2) + std::pow(x, 2) + std::pow(y, 2)))) / (std::pow(n, 2) + std::pow(x, 2) + std::pow(y, 2));

				trackX = x * vectorMultiplier;
				trackY = y * vectorMultiplier;
				trackZ = n * vectorMultiplier;
			}

			void camera::fromCameraSpaceToWindowCoord(float x, float y, float z, float & xw, float & yw)
			{
				xw = (w*(-(n*x) + l*z)) / ((l - r)*z);
				yw = (h*n*y - h*t*z) / (b*z - t*z);
			}

			void camera::updateCamera()
			{
				float cpx = camX, cpy = camY, cpz = camZ;
				float tpx = targX, tpy = targY, tpz = targZ;
				float upx = 0., upy = 0., upz = 1.;

				float nx, ny, nz;

				float den = std::sqrt(std::pow(cpx - tpx, 2) + std::pow(cpy - tpy, 2) + std::pow(cpz - tpz, 2));
				float vx = (-cpx + tpx) / den;
				float vy = (-cpy + tpy) / den;
				float vz = (-cpz + tpz) / den;

				den = std::sqrt(std::pow(vy, 2) - 2 * upx*upz*vx*vz + std::pow(vz, 2) - 2 * upy*vy*(upx*vx + upz*vz) - std::pow(upy, 2)*(-1 + 2 * std::pow(vy, 2) + std::pow(vz, 2)) - std::pow(upz, 2)*(-1 + std::pow(vy, 2) + 2 * std::pow(vz, 2)));
				float tx = (-(upy*vx*vy) - upz*vx*vz + upx*(std::pow(vy, 2) + std::pow(vz, 2))) / den;
				float ty = (upy - upy*std::pow(vy, 2) - vy*(upx*vx + upz*vz)) / den;
				float tz = (upz - (upx*vx + upy*vy)*vz - upz*std::pow(vz, 2)) / den;

				den = std::sqrt(std::pow((-1 + ty)*vx - tx*vy, 2) + std::pow(tx + tz*vx - tx*vz, 2) + std::pow(-1 + ty + tz*vy + vz - ty*vz, 2));
				nx = (-1 + ty + tz*vy + vz - ty*vz) / den;
				ny = (-(tz*vx) + tx*(-1 + vz)) / den;
				nz = ((-1 + ty)*vx - tx*vy) / den;

				float cosTheta = (-1 + ty - tz*vy + vz + ty*vz) / 2.;
				float sinTheta = std::sqrt(-((-3 + ty - tz*vy + vz + ty*vz)*(1 + ty - tz*vy + vz + ty*vz))) / 2.;

				float dx = -cpx;
				float dy = -cpy;
				float dz = -cpz;

				camMatrix[0] = 1 + (-1 + cosTheta)*std::pow(ny, 2) + (-1 + cosTheta)*std::pow(nz, 2);
				camMatrix[4] = nx*ny - cosTheta*nx*ny - nz*sinTheta;
				camMatrix[8] = nx*nz - cosTheta*nx*nz + ny*sinTheta;
				camMatrix[12] = dx + (dy - cosTheta*dy)*nx*ny + (-1 + cosTheta)*dx*std::pow(ny, 2) + (dz - cosTheta*dz)*nx*nz + (-1 + cosTheta)*dx*std::pow(nz, 2) + dz*ny*sinTheta - dy*nz*sinTheta;
				camMatrix[1] = nx*ny - cosTheta*nx*ny + nz*sinTheta;
				camMatrix[5] = cosTheta + (1 - cosTheta)*std::pow(ny, 2);
				camMatrix[9] = ny*nz - cosTheta*ny*nz - nx*sinTheta;
				camMatrix[13] = ny*(dx*nx + dy*ny + dz*nz) - cosTheta*(dx*nx*ny + dy*(-1 + std::pow(ny, 2)) + dz*ny*nz) + (-(dz*nx) + dx*nz)*sinTheta;
				camMatrix[2] = nx*nz - cosTheta*nx*nz - ny*sinTheta;
				camMatrix[6] = ny*nz - cosTheta*ny*nz + nx*sinTheta;
				camMatrix[10] = cosTheta + (1 - cosTheta)*std::pow(nz, 2);
				camMatrix[14] = nz*(dx*nx + dy*ny + dz*nz) - cosTheta*((dx*nx + dy*ny)*nz + dz*(-1 + std::pow(nz, 2))) + (dy*nx - dx*ny)*sinTheta;
				camMatrix[3] = 0;
				camMatrix[7] = 0;
				camMatrix[11] = 0;
				camMatrix[15] = 1;

				glUniformMatrix4fv(worldLocation, 1, GL_FALSE, camMatrix.data());

				std::array<float, 4>beforeTransf;
				std::array<float, 4>afterTransf;

				beforeTransf[0] = targX;
				beforeTransf[1] = targY;
				beforeTransf[2] = targZ;
				beforeTransf[3] = 1.;
				oxyde::linAlg::multiplyMatrixByVector(camMatrix.data(), beforeTransf.data(), afterTransf.data());
				float camSpaceTx = afterTransf[0] / afterTransf[3];
				float camSpaceTy = afterTransf[1] / afterTransf[3];
				float camSpaceTz = afterTransf[2] / afterTransf[3];

				targetZ = camSpaceTz;

				camMatrixInv[0] = 1 + (-1 + cosTheta)*std::pow(ny, 2) + (-1 + cosTheta)*std::pow(nz, 2);
				camMatrixInv[4] = nx*ny - cosTheta*nx*ny + nz*sinTheta;
				camMatrixInv[8] = nx*nz - cosTheta*nx*nz - ny*sinTheta;
				camMatrixInv[12] = -dx;
				camMatrixInv[1] = nx*ny - cosTheta*nx*ny - nz*sinTheta;
				camMatrixInv[5] = cosTheta + (1 - cosTheta)*std::pow(ny, 2);
				camMatrixInv[9] = ny*nz - cosTheta*ny*nz + nx*sinTheta;
				camMatrixInv[13] = -dy;
				camMatrixInv[2] = nx*nz - cosTheta*nx*nz + ny*sinTheta;
				camMatrixInv[6] = ny*nz - cosTheta*ny*nz - nx*sinTheta;
				camMatrixInv[10] = cosTheta + (1 - cosTheta)*std::pow(nz, 2);
				camMatrixInv[14] = -dz;
				camMatrixInv[3] = 0;
				camMatrixInv[7] = 0;
				camMatrixInv[11] = 0;
				camMatrixInv[15] = 1;

				glUniformMatrix4fv(invWorldLocation, 1, GL_FALSE, camMatrixInv.data());
			}

			void camera::rotateCameraAroundTarget(float owx, float owy, float dewx, float dewy)
			{
				float camSpaceCurX, camSpaceCurY, camSpaceCurZ;
				float camSpaceFormerX, camSpaceFormerY, camSpaceFormerZ;

				std::array<float, 4>beforeTransf;
				std::array<float, 4>afterTransf;

				/////////////////////////
				float cx, cy, cz;
				float ox, oy, oz;
				float tx, ty, tz;
				float dex, dey, dez;

				//////////////////////

				fromWindowToTrackBall(dewx, dewy, camSpaceCurX, camSpaceCurY, camSpaceCurZ);
				fromWindowToTrackBall(owx, owy, camSpaceFormerX, camSpaceFormerY, camSpaceFormerZ);

				beforeTransf[0] = camSpaceCurX;
				beforeTransf[1] = camSpaceCurY;
				beforeTransf[2] = camSpaceCurZ;
				beforeTransf[3] = 1.;
				oxyde::linAlg::multiplyMatrixByVector(camMatrixInv.data(), beforeTransf.data(), afterTransf.data());
				dex = afterTransf[0] / afterTransf[3];
				dey = afterTransf[1] / afterTransf[3];
				dez = afterTransf[2] / afterTransf[3];

				beforeTransf[0] = camSpaceFormerX;
				beforeTransf[1] = camSpaceFormerY;
				beforeTransf[2] = camSpaceFormerZ;
				beforeTransf[3] = 1.;
				oxyde::linAlg::multiplyMatrixByVector(camMatrixInv.data(), beforeTransf.data(), afterTransf.data());
				ox = afterTransf[0] / afterTransf[3];
				oy = afterTransf[1] / afterTransf[3];
				oz = afterTransf[2] / afterTransf[3];

				//////////////////////

				cx = camX;
				cy = camY;
				cz = camZ;

				tx = targX;
				ty = targY;
				tz = targZ;

				//////////////////////

				oxyde::log::printPointInSpace(L"orig", ox, oy, oz);

				oxyde::log::printPointInSpace(L"dest", dex, dey, dez);

				float lenOriTargetSpace = std::sqrt(std::pow(ox - tx, 2) + std::pow(oy - ty, 2) + std::pow(oz - tz, 2));

				float lenDestTargetSpace = std::sqrt(std::pow(dex - tx, 2) + std::pow(dey - ty, 2) + std::pow(dez - tz, 2));

				float cosTheta = ((dex - tx)*(ox - tx) + (dey - ty)*(oy - ty) + (dez - tz)*(oz - tz)) / (lenDestTargetSpace*lenOriTargetSpace);

				oxyde::log::printNamedParameter(L"cosTheta", cosTheta);

				cosTheta = cosTheta > 1.0 ? 1.0 : cosTheta;
				cosTheta = cosTheta < -1.0 ? -1.0 : cosTheta;

				float sinTheta = std::sqrt(1 - std::pow(cosTheta, 2));

				float lenCrossOriDest = std::sqrt(std::pow(dey*(ox - tx) + oy*tx - ox*ty + dex*(-oy + ty), 2) + 
					std::pow(-(oz*tx) + dez*(-ox + tx) + dex*(oz - tz) + ox*tz, 2) + 
					std::pow(dez*(oy - ty) + oz*ty - oy*tz + dey*(-oz + tz), 2));

				float nx = (dez*(oy - ty) + oz*ty - oy*tz + dey*(-oz + tz)) / lenCrossOriDest;
				float ny = (-(oz*tx) + dez*(-ox + tx) + dex*(oz - tz) + ox*tz) / lenCrossOriDest;
				float nz = (dey*(ox - tx) + oy*tx - ox*ty + dex*(-oy + ty)) / lenCrossOriDest;

				/*
				ma[0] = 1 + (-1 + cosTheta)*std::pow(ny, 2) + (-1 + cosTheta)*std::pow(nz, 2);
				ma[4] = nx*ny - cosTheta*nx*ny - nz*sinTheta;
				ma[8] = nx*nz - cosTheta*nx*nz + ny*sinTheta;
				ma[12] = sinTheta*(nz*ty - ny*tz) + (-1 + cosTheta)*(-(std::pow(ny, 2)*tx) + nx*ny*ty + nz*(-(nz*tx) + nx*tz));
				ma[1] = nx*ny - cosTheta*nx*ny + nz*sinTheta;
				ma[5] = cosTheta + (1 - cosTheta)*std::pow(ny, 2);
				ma[9] = ny*nz - cosTheta*ny*nz - nx*sinTheta;
				ma[13] = sinTheta*(-(nz*tx) + nx*tz) + (-1 + cosTheta)*(nx*ny*tx + (-1 + std::pow(ny, 2))*ty + ny*nz*tz);
				ma[2] = nx*nz - cosTheta*nx*nz - ny*sinTheta;
				ma[6] = ny*nz - cosTheta*ny*nz + nx*sinTheta;
				ma[10] = cosTheta + (1 - cosTheta)*std::pow(nz, 2);
				ma[14] = sinTheta*(ny*tx - nx*ty) + (-1 + cosTheta)*(nx*nz*tx + ny*nz*ty + (-1 + std::pow(nz, 2))*tz);
				ma[3] = 0;
				ma[7] = 0;
				ma[11] = 0;
				ma[15] = 1;
				*/

				float ncx = cx + (-1 + cosTheta)*std::pow(ny, 2)*(cx - tx) + (-1 + cosTheta)*std::pow(nz, 2)*(cx - tx) 
					- (-1 + cosTheta)*nx*ny*(cy - ty) + nz*sinTheta*(-cy + ty) - (-1 + cosTheta)*nx*nz*(cz - tz) + ny*sinTheta*(cz - tz);

				float ncy = cosTheta*cy - (-1 + cosTheta)*nx*ny*(cx - tx) + nz*sinTheta*(cx - tx) - (-1 + cosTheta)*std::pow(ny, 2)*(cy - ty) 
					+ ty - cosTheta*ty - (-1 + cosTheta)*ny*nz*(cz - tz) + nx*sinTheta*(-cz + tz);

				float ncz = cosTheta*cz - (-1 + cosTheta)*nx*nz*(cx - tx) + ny*sinTheta*(-cx + tx) - (-1 + cosTheta)*ny*nz*(cy - ty) 
					+ nx*sinTheta*(cy - ty) - (-1 + cosTheta)*std::pow(nz, 2)*(cz - tz) + tz - cosTheta*tz;

				//////////////////////

				camX = ncx;
				camY = ncy;
				camZ = ncz;
			}

			void camera::moveCameraToOrFromTheTarget(short inc)
			{
				float finc = float(inc) / 20.;
				float den = std::sqrt(std::pow(-camX + targX, 2) + std::pow(-camY + targY, 2) + std::pow(-camZ + targZ, 2));
				camX += (finc*(-camX + targX)) / den;
				camY += (finc*(-camY + targY)) / den;
				camZ += (finc*(-camZ + targZ)) / den;
			}

			void camera::mouseWheel(short inc)
			{
				theInstance->moveCameraToOrFromTheTarget(inc);
			}

			void camera::changeState(dragMessage theMessage)
			{

				//		states:
				//  				| press button   | move mouse    | release button | 
				// _________________|________________|_______________|________________|
				// idle 			| start dragging |				 |				  |
				// start dragging	|				 | keep dragging | iddle		  |
				// keep dragging	|				 | keep dragging | iddle		  |

				float camSpaceCurX, camSpaceCurY, camSpaceCurZ;
				float camSpaceFormerX, camSpaceFormerY, camSpaceFormerZ;

				float worldSpaceCurX, worldSpaceCurY, worldSpaceCurZ;
				float worldSpaceFormerX, worldSpaceFormerY, worldSpaceFormerZ;

				std::array<float, 4>beforeTransf;
				std::array<float, 4>afterTransf;

				switch (currentState) {
				case dragState::IDLE:
					switch (theMessage) {
					case dragMessage::PRESS_BUTTON:
						currentState = dragState::START_DRAGGING;
						break;
					}
					break;

				case dragState::KEEP_DRAGGING:
					switch (theMessage) {
					case dragMessage::MOVE_MOUSE:
						currentState = dragState::KEEP_DRAGGING;
						break;
					case dragMessage::RELEASE_BUTTON:
						currentState = dragState::IDLE;
						break;
					}
					break;

				case dragState::START_DRAGGING:
					switch (theMessage) {
					case dragMessage::MOVE_MOUSE:
						currentState = dragState::KEEP_DRAGGING;
						break;
					case dragMessage::RELEASE_BUTTON:
						currentState = dragState::IDLE;
						break;
					}
					break;
				}

				switch (currentState) {
				case dragState::IDLE:
					break;

				case dragState::KEEP_DRAGGING:
					currentX = winX;
					currentY = winY;
					rotateCameraAroundTarget(currentX, currentY, formerX, formerY);

					formerX = winX;
					formerY = winY;
					break;

				case dragState::START_DRAGGING:
					formerX = winX;
					formerY = winY;
					break;
				}
			}

			void camera::updateTargetPos()
			{
				theInstance->changeTargetState(targetMessage::TARGET_MOVED);
			}

			void camera::changeTargetState(targetMessage theMessage)
			{

				//	states:
				//					   | key pressed    | start animation   | animation ended   | target moved      |
				//	___________________|________________|___________________|___________________|___________________|
				//	already on target  | changed target |				    |				    | already on target |
				//	changed target     |				| animation runing  |				    | animation runing  |
				//	animation runing   |				|				    | already on target | animation runing  |

				float animationStep;

				switch (currentTargetState) {
				case targetState::ALREADY_ON_TARGET:
					switch (theMessage) {
					case targetMessage::KEY_PRESSED:
						currentTargetState = targetState::CHANGED_TARGET;
						break;
					case targetMessage::TARGET_MOVED:
						currentTargetState = targetState::ALREADY_ON_TARGET;
						break;
					}
					break;

				case targetState::ANIMATION_RUNNING:
					switch (theMessage) {
					case targetMessage::ANIMATION_ENDED:
						currentTargetState = targetState::ALREADY_ON_TARGET;
						break;
					case targetMessage::TARGET_MOVED:
						currentTargetState = targetState::ANIMATION_RUNNING;
						break;
					}
					break;

				case targetState::CHANGED_TARGET:
					switch (theMessage) {
					case targetMessage::START_ANIMATION:
						currentTargetState = targetState::ANIMATION_RUNNING;
						break;
					}
					break;
				}

				switch (currentTargetState) {
				case targetState::ALREADY_ON_TARGET:
					isTargetAnimationRunning = false;
					std::array<float, 3> bonePos = oxyde::scene::bone::getJointPositionAtIndex(currentBone);
					targX = bonePos[0];
					targY = bonePos[1];
					targZ = bonePos[2];
					updateCamera();
					break;

				case targetState::ANIMATION_RUNNING:
					currentTime = oxyde::scene::ticker::getCurrentTime();
					animationStep = (currentTime - startTime) / 900.;
					if (animationStep > 1. || animationStep < 0.) {
						changeTargetState(targetMessage::ANIMATION_ENDED);
					}
					else {
						std::array<float, 3> bonePos = oxyde::scene::bone::getJointPositionAtIndex(currentBone);
						targX += (bonePos[0] - targX)*animationStep;
						targY += (bonePos[1] - targY)*animationStep;
						targZ += (bonePos[2] - targZ)*animationStep;
					}
					updateCamera();
					break;

				case targetState::CHANGED_TARGET:
					isTargetAnimationRunning = true;
					startTime = oxyde::scene::ticker::getCurrentTime();
					currentBone = (currentBone + incrementTarget) % oxyde::scene::bone::getNumJoints();
					changeTargetState(targetMessage::START_ANIMATION);
					break;
				}

			}

			void camera::keyDown(short increment)
			{
				if (theInstance) {
					theInstance->incrementTarget = increment;
					theInstance->changeTargetState(targetMessage::KEY_PRESSED);
				}
			}

			void camera::createCamera(float in_f, float in_n, float in_l, float in_r, float in_t, float in_b, float in_w, float in_h)
			{
				GLint projLocation, worldLocation, invWorldLocation, viewLocation;
				GLint program = -1;
				glGetIntegerv(GL_CURRENT_PROGRAM, &program);
				if (program) {
					projLocation = glGetUniformLocation(program, "Proj");
					worldLocation = glGetUniformLocation(program, "World");
					invWorldLocation = glGetUniformLocation(program, "invWorld");
					viewLocation = glGetUniformLocation(program, "View");

					if (theInstance != nullptr) {
						delete theInstance;
					}

					theInstance = new camera(in_f, in_n, in_l, in_r, in_t, in_b, in_w, in_h, projLocation, worldLocation, invWorldLocation, viewLocation);
				}
			}

			camera::camera(float in_f, float in_n, float in_l, float in_r, float in_t, float in_b, float in_w, float in_h,
				GLint in_projLocation, GLint in_worldLocation, GLint in_invWorldLocation, GLint in_viewLocation) :
				f(in_f), n(in_n), l( in_l), r( in_r), t(in_t), b(in_b), w(in_w), h(in_h),
				projLocation(in_projLocation), worldLocation(in_worldLocation), invWorldLocation(in_invWorldLocation), viewLocation(in_viewLocation)
			{
				currentState = dragState::IDLE;
				currentTargetState = targetState::ALREADY_ON_TARGET;

				currentBone = oxyde::scene::bone::getNumJoints() - 1;

				camX = 400;
				camY = 400.0;
				camZ = -100.0;

				buildPerspectiveMatrix();
				buildViewportMatrix();
			}

			void camera::mouseButtonDown(int x, int y)
			{
				if (theInstance) {
					theInstance->winX = x;
					theInstance->winY = y;

					theInstance->changeState(dragMessage::PRESS_BUTTON);
				}
			}

			void camera::mouseMove(int x, int y)
			{
				if (theInstance) {
					theInstance->winX = x;
					theInstance->winY = y;

					theInstance->changeState(dragMessage::MOVE_MOUSE);
				}
			}

			void camera::mouseButtonUp()
			{
				if (theInstance) {
					theInstance->changeState(dragMessage::RELEASE_BUTTON);
				}
			}

		}
	}
}
