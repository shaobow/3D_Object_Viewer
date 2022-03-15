/**
Object Class
- # vertices
- # faces
- load function
- vec of faces

Code written by Shaobo Wang
<The software assessment from Neocis>
@03/14/2022
**/

#include <sstream>
#include <math.h>
#include "fssimplewindow.h"
#include "object.h"

object::object()
{
}

object::~object()
{
}

void object::load(ifstream& inFile)
{
	std::string wholeLineString;
	std::stringstream wholeLineStream;
	int lineCount = 0;
	vector<vecf3> verts;  // temp vector for all vertices in order
	float minX = 0, maxX = 0, minY = 0, maxY = 0;

	// go through file
	while (!inFile.eof()) {
		// read the whole line
		getline(inFile, wholeLineString);

		// read from stringstream
		wholeLineStream.str(wholeLineString);

		// parse comma separated string 
		vector<string> temp;
		while (wholeLineStream.good()) {
			string sub;
			getline(wholeLineStream, sub, ',');
			temp.push_back(sub);
		}

		// handle first line
		if (lineCount == 0){
			numVertice = stoi(temp.at(0));
			numFace = stoi(temp.at(1));
		}
		// handle vertices
		else if (lineCount <= numVertice) {
			vecf3 v;  // input vertice
			for (int i = 1; i < temp.size(); i++)
				v.content[i - 1] = stof(temp.at(i));
			verts.push_back(v);

			// find margin coords
			if (v.content[0] < minX) minX = v.content[0];
			else if (v.content[0] > maxX) maxX = v.content[0];
			if (v.content[1] < minY) minY = v.content[1];
			else if (v.content[1] > maxY) maxY = v.content[1];
		}
		// handle faces
		else if (lineCount <= numVertice + numFace) {
			int f[3] = { 0,0,0 };
			for (int i = 0; i < temp.size(); i++)
				f[i] = stoi(temp.at(i));
			mesh.push_back({ verts[f[0] - 1], verts[f[1] - 1],verts[f[2] - 1] });
		}

		// calculate distance across axis
		distX = maxX - minX;
		distY = maxY - minY;

		wholeLineStream.clear(); // get ready for next line
		lineCount++;  // count line number
	}

	origin = mesh;
}

void object::reload()
{
	mesh = origin;
}

void object::drawEdge()
{	
	// draw with blue
	glColor3f(0.0f, 0.0f, 1.0f);

	// draw each face as triangle	
	for (auto& tri : mesh) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 3; i++)
			glVertex3f(tri.p[i].content[0], tri.p[i].content[1], 0.0f);
		glEnd();
	}
	// draw all vertices as point
	glEnable(GL_POINT_SMOOTH);
	glPointSize(8.0);
	glBegin(GL_POINTS);
	glPointSize(10.0f);
	for (auto& tri : mesh) 
		for (int i = 0; i < 3; i++)
			glVertex3f(tri.p[i].content[0], tri.p[i].content[1], 0.0f);
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}

void object::drawFace()
{

}

void object::rotX(float ang)
{
	matf3 Rot;
	Rot.scalarMultiply(0.0f);
	Rot.set(1, 1, 1.0f);
	Rot.set(2, 2, cos(ang));
	Rot.set(2, 3, -sin(ang));
	Rot.set(3, 2, sin(ang));
	Rot.set(3, 3, cos(ang));
	for (auto& tri : mesh)
		for (int i = 0; i < 3; i++) {
			vecf3 p_rot;
			Rot.vecMultiply(tri.p[i], p_rot);
			tri.p[i] = p_rot;
		}
}

void object::rotY(float ang)
{
	matf3 Rot;
	Rot.scalarMultiply(0.0f);
	Rot.set(1, 1, cos(ang));
	Rot.set(1, 3, sin(ang));
	Rot.set(2, 2, 1.0f);
	Rot.set(3, 1, -sin(ang));
	Rot.set(3, 3, cos(ang));
	
	Rot.set(3, 3, cos(ang));
	for (auto& tri : mesh)
		for (int i = 0; i < 3; i++) {
			vecf3 p_rot;
			Rot.vecMultiply(tri.p[i], p_rot);
			tri.p[i] = p_rot;
		}
}