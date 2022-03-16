/**
 * This is the object viewer function to load object,
 * display in openGL window, and handle user input
 */

#include "objectViewer.h"

#include <iostream>
#include <fstream>

#include "fssimplewindow.h"  // For openGL window and user input

#include "object.h"


void objectViewer()
{
	// my 3D object
	object poly;


	// display window config
	const int width = 800, height = 800;  // window size
	bool terminate = false;  // exit flag
	bool isColored = false;  // color flag


	// user input
	int key;  // keyboard
	bool isMouseDown = false;  // click
	mouseInfo myMouse;  // mouse location


	// load from file
	string datafilename;
	ifstream inFile;

	cout << "\n\nPlease enter the name of the file to read > ";  // ask for filename
	cin >> datafilename;
	if (datafilename.find(".txt") == string::npos) datafilename += ".txt";  // omit extension

	inFile.open(datafilename);
	if (inFile.is_open()) {
		// read file line by line
		poly.load(inFile);
		inFile.close();
	}
	else {  // may not have found file
		cout << "\nError reading file. Please check data and try again." << endl;
	}


	// start display
	FsOpenWindow(16, 16, width, height, 1, "3D Object Viewer"); // use double buffer
	const float scaleX = width / 2 / poly.distX, scaleY = height / 2 / poly.distY;
	const float inc = 1.0 * PI / 180;  // rotation increment

	while (!terminate) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		FsPollDevice();

		// mouse
		myMouse.preLocX = myMouse.locX;
		myMouse.preLocY = myMouse.locY;
		int mouseEvent = FsGetMouseEvent(myMouse.leftButton, myMouse.middleButton, myMouse.rightButton, myMouse.locX, myMouse.locY);
		if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN || mouseEvent == FSMOUSEEVENT_LBUTTONUP) 
			isMouseDown = !isMouseDown;
		if (isMouseDown) {
			poly.rotX(-inc * -(myMouse.locY - myMouse.preLocY));  // note: original Y pointing down
			poly.rotY(inc * (myMouse.locX - myMouse.preLocX));
		}

		// keyboard
		key = FsInkey();
		switch (key) {
		case FSKEY_ESC:
		case FSKEY_X:   // exit program
			terminate = true;
			break;
		case FSKEY_R:  // reload original pose
			poly.reload();
			break;
		case FSKEY_C:  // turn on/off color
			isColored = !isColored;
			break;
		case FSKEY_UP: // rotate CW about X-axis
			poly.rotX(-inc);
			break;
		case FSKEY_DOWN: // rotate CCW about X-axis
			poly.rotX(inc);
			break;
		case FSKEY_LEFT: // rotate CW about Y-axis
			poly.rotY(-inc);
			break;
		case FSKEY_RIGHT: // rotate CCW about Y-axis
			poly.rotY(inc);
			break;
		default:
			break;
		}

		// adjust the graphics plane
		glTranslated(0, height, 0); // make bottom-left the origin
		glScaled(1., -1., 0);  // positive y going up
		glTranslated(width / 2, height / 2, 0);  // place origin at center
		glScaled(scaleX, scaleY, 0);  // scale up

		if (isColored) poly.drawFace();
		else poly.drawEdge();

		// reset transformation matrix completely
		glLoadIdentity();

		FsSwapBuffers();  // use this instead of glFlush() when using double buffer
		FsSleep(20);
	}
}