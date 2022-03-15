/**
3D object viewer
- Load comma-separated text object file 
- Display the loaded 3D object
- Click and drag mouse to rotate the object

Code written by Shaobo Wang
<The software assessment from Neocis>
@03/14/2022
**/

#include <iostream>
#include <fstream>
#include "fssimplewindow.h"
#include "object.h"

const float PI = 3.1415926;
using namespace std;

struct mouseInfo {
	int leftButton, middleButton, rightButton;
	int mouseEvent;
	int locX, locY;
	int preLocX = 0, preLocY = 0;
};

int main(void)
{
	const int width = 800, height = 800;  // window size
	bool terminate = false;
	int key;  // keyboard input
	int mouseEvent;  // mouse input
	mouseInfo myMouse;

	string datafilename;
	ifstream inFile;
	object poly;

	//cout << "\n\nPlease enter the name of the file to read > ";  //ask for filename
	//cin >> datafilename;
	//if (datafilename.find(".txt") == string::npos)
	//	datafilename += ".txt";  // allows user to omit extension

	//**---------DEBUG----------**//
	datafilename = "object.txt";
	//**------------------------**//

	inFile.open(datafilename);
	if (inFile.is_open()) {
		// read file line by line
		poly.load(inFile);
		inFile.close();
	}
	else  // may not have found file
		cout << "\nError reading file. Please check data and try again." << endl;

	FsOpenWindow(16, 16, width, height, 1, "3D Object Viewer"); // use double buffer
	const float scaleX = width / 2 / poly.distX, scaleY = height / 2 / poly.distY;
	const float inc = -1.0 * PI / 180;  // rotation increment
	bool isMouseDown = false;

	while (!terminate) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		FsPollDevice();
		key = FsInkey();

		// mouse
		myMouse.preLocX = myMouse.locX;
		myMouse.preLocY = myMouse.locY;
		mouseEvent = FsGetMouseEvent(myMouse.leftButton, myMouse.middleButton, myMouse.rightButton, myMouse.locX, myMouse.locY);
		if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN || mouseEvent == FSMOUSEEVENT_LBUTTONUP) isMouseDown = !isMouseDown;

		if (isMouseDown) {
			
			poly.rotX(-inc * (myMouse.locY - myMouse.preLocY));
			poly.rotY(-inc * (myMouse.locX - myMouse.preLocX));
		}

		// keyboard
		switch (key) {
		case FSKEY_ESC:
		case FSKEY_X:   // exit program
			terminate = true;
			break;
		case FSKEY_R:  // reload original pose
			poly.reload();
			break;
		case FSKEY_UP: // rotate CW about X-axis
			poly.rotX(-inc);
			break;
		case FSKEY_DOWN: // rotate CCW about X-axis
			poly.rotX(inc);
			break;
		case FSKEY_LEFT: // rotate CCW about Y-axis
			poly.rotY(inc);
			break;
		case FSKEY_RIGHT: // rotate CW about Y-axis
			poly.rotY(-inc);
			break;
		default:
			break;
		}

		//adjust the graphics plane
		glTranslated(0, height, 0); // make bottom-left the origin
		glScaled(1., -1., 0);  // positive y going up
		glTranslated(width / 2, height / 2, 0);  // place origin at center
		glScaled(scaleX, scaleY, 0);  // scale up

		poly.drawEdge();

		// reset transformation matrix completely
		glLoadIdentity();

		FsSwapBuffers();  // use this instead of glFlush() when using double buffer
		FsSleep(20);
	}
}