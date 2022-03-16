/**
 * This is the object viewer function to load object,
 * display in openGL window, and handle user input
 */

#ifndef OBJECTVIEWER_H_
#define OBJECTVIEWER_H_

const float PI = 3.1415926;
using namespace std;

// mouse input info
struct mouseInfo {
	int leftButton, middleButton, rightButton;
	int locX, locY;
	int preLocX = 0, preLocY = 0;  // previous location
};

void objectViewer();
#endif // !OBJECTVIEWER_H_


