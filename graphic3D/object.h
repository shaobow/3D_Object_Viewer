#pragma once
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

#include <vector>
#include <fstream>
#include "matrix.h"

using namespace std;

typedef vec<float, 3> vecf3;
typedef matrix<float, 3> matf3;

// each face is composed by three vertices
struct face
{
	vecf3 p[3];  // three vertice coord in 3D space
	// color info
};

class object
{
private:
	int numVertice;  // # vertices
	int numFace;  // # faces
	vector<face> mesh;  // vector of faces
	vector<face> origin;

public:
	object();
	~object();
	float distX, distY; // scale factor

	// read from user specified file
	void load(ifstream& inFile);

	// read from user specified file
	void reload();

	// draw 3D object on plane with edge and vertices
	void drawEdge();

	// draw 3D object on plane with faces
	void drawFace();

	// rotate about X-axis
	void rotX(float ang);

	// rotate about Y-axis
	void rotY(float ang);
};
