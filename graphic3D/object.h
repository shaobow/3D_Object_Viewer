/**
 * 3D object class to display
 * every three vertices form a face
 * all faces form the mesh object
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <vector>
#include <fstream>
#include "matrix.h"

using namespace std;

// type for rigid body transformation
typedef vec<float, 3> vecf3;  // 3x1 vector for 3D points
typedef matrix<float, 3> matf3;  // 3x3 rotation matrix for 3D cases

// b value in rgb coord
const float MAXBLUE = 1.000;  // #0000FF 
const float MINBLUE = 0.373;  // #00005F 

// each face is composed by three vertices
struct face
{
	vecf3 p[3];  // three vertice coord in 3D space
	float bValue;  // color info
};

class object
{
private:
	int numVertice, numFace;  // # vertices & # faces

	vecf3 CG = { 0.0f,0.0f,0.0f };  // center of gravity

	vector<face> mesh;  // vector of faces
	vector<face> origin;
	
	// dot product between vecs
	float dotProduct(const vecf3 a, const vecf3 b);  

	// calculate norm of a face
	vecf3 getNorm(face tri);  

	// change order to make norms stick out
	void correctNorm(vector<face>& mesh);  

public:
	object() {};
	object(ifstream& inFile) { load(inFile); }

	float distX, distY; // scale factor

	// read from user specified file
	void load(ifstream& inFile);

	// restore mesh to origin
	void reload() { mesh = origin; }

	// draw 3D object on plane with edge and vertices
	void drawEdge();

	// draw 3D object on plane with colored faces
	void drawFace();

	// rotate about X-axis
	void rotX(float ang);

	// rotate about Y-axis
	void rotY(float ang);
};
#endif // !OBJECT_H_