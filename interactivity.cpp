#include "interactivity.h"

static bool lButtonPressed = false;
static int winX = 0;
static int winY = 0;

void setButtonPressed( bool isPressed) {
	lButtonPressed = isPressed;
}

bool getButtonPressed() {
	return lButtonPressed;
}

void setWinX(int x) {
	//if (lButtonPressed) {
		winX = x;
	//}
}

void setWinY(int y) {
	//if (lButtonPressed) {
		winY = y;
	//}
}

int getWinX() {
	return winX;
}

int getWinY() {
	return winY;
}

