/**
 * 3D object class to display
 * every three vertices form a face
 * all faces form the mesh object
 */

#include "object.h"

#include <sstream>
#include <math.h>
#include <algorithm>

#include "fssimplewindow.h"

 /**
  * Compute dot product of two vectors
  * 
  * @param[out] ans, the result
  * @param[in] vec1, first vec
  * @param[in] vec2, second vec
  */
float object::dotProduct(const vecf3 vec1, const vecf3 vec2)
{
	float ans = 0;
	for (int i = 0; i < 3; i++)	ans += vec1.content[i] * vec2.content[i];
	return ans;
}


/**
 * Compute norm vec of a face by computing 
 * the cross product of two vecs
 *
 * @param[out] n, the normalized norm vec
 * @param[in] tri, the face struct with three points
 */
vecf3 object::getNorm(face tri)
{
	// build two vecs with three points
	vecf3 vec1 = { tri.p[1].content[0] - tri.p[0].content[0],
				tri.p[1].content[1] - tri.p[0].content[1],
				tri.p[1].content[2] - tri.p[0].content[2] };
	vecf3 vec2 = { tri.p[2].content[0] - tri.p[0].content[0],
				tri.p[2].content[1] - tri.p[0].content[1],
				tri.p[2].content[2] - tri.p[0].content[2] };
	
	// cross product of two vecs
	vecf3 n = {
		vec1.content[1] * vec2.content[2] - vec1.content[2] * vec2.content[1],
		vec1.content[2] * vec2.content[0] - vec1.content[0] * vec2.content[2],
		vec1.content[0] * vec2.content[1] - vec1.content[1] * vec2.content[0]
	};

	// normalize the cross product result to one
	float k = sqrtf(n.content[0] * n.content[0] 
			      + n.content[1] * n.content[1] 
				  + n.content[2] * n.content[2]);
	for (int i = 0; i < 3; i++) n.content[i] /= k;
	return n;
}


/**
 * Load the object from input file stream
 * Object file has to follow required format
 *
 * @param[in] inFile, the file input stream 
 */
void object::load(ifstream& inFile)
{
	int lineCount = 0;  // line number counter

	std::string wholeLineString;  // string of one line from the input stream
	std::stringstream wholeLineStream;  // string stream to parse

	vector<vecf3> verts;  // temp vector for all vertices in order
	float minX = 0, maxX = 0, minY = 0, maxY = 0;  // reg minMAX to find margin range

	// go through file
	while (!inFile.eof()) {
		// read the whole line
		getline(inFile, wholeLineString);

		// read from stringstream
		wholeLineStream.str(wholeLineString);

		// parse comma separated string by temp string vec
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

		// handle vertices/other lines
		else if (lineCount <= numVertice) {
			vecf3 v;   
			for (int i = 1; i < temp.size(); i++) {  // start after ID
				// get vertice coord
				v.content[i - 1] = stof(temp.at(i));
				// count for center of gravity
				CG.content[i - 1] += v.content[i - 1] / numVertice;
			}

			// stored in cache with ID order
			verts.push_back(v);

			// find margin coords
			if (v.content[0] < minX) minX = v.content[0];
			else if (v.content[0] > maxX) maxX = v.content[0];
			if (v.content[1] < minY) minY = v.content[1];
			else if (v.content[1] > maxY) maxY = v.content[1];
		}

		// handle faces
		else if (lineCount <= numVertice + numFace) {
			// get vertices ID of a face 
			int f[3] = { 0,0,0 };
			for (int i = 0; i < temp.size(); i++)
				f[i] = stoi(temp.at(i));

			// use vertices ID of a face to construct tri 
			mesh.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}

		// calculate distance across axis
		distX = maxX - minX;
		distY = maxY - minY;

		// get ready for next line
		wholeLineStream.clear(); 
		lineCount++;  // count line number
	}

	// make norm pointing out
	correctNorm(mesh);
}


/**
 * Flip the order of vertices ID in a face 
 * to have norm vec pointing out
 * 
 * Use the vec pointing from center of gravity(CG) of body 
 * to CG of face to indicate out direction
 * Check norm calculated based on default input ID order
 * flip any two of three to change order
 *
 * @param[in] mesh, the 3D object mesh
 */
void object::correctNorm(vector<face>& mesh)
{
	for (auto& tri : mesh) {
		// compute norm of face in ID order
		vecf3 norm = getNorm(tri);

		// compute outward dir
		vecf3 outdir = { tri.p[0].content[0] + tri.p[1].content[0] + tri.p[2].content[0] - CG.content[0],
						 tri.p[0].content[1] + tri.p[1].content[1] + tri.p[2].content[1] - CG.content[1],
						 tri.p[0].content[2] + tri.p[1].content[2] + tri.p[2].content[2] - CG.content[2] };

		// check whether norm align
		float d = dotProduct(norm, outdir);
		if (d < 0) {
			// if not, flip order
			vecf3 temp;
			temp = tri.p[1];
			tri.p[1] = tri.p[2];
			tri.p[2] = temp;
		}
	}

	// finished loading, make deep copy
	origin = mesh;  
}


/**
 * Draw with blue lines and blue dots
 * of the 3D object
 */
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

/**
 * Draw with blue faces of the 3D object
 * 
 * The color of each face is given by 
 * the angle between norm and unit z
 * Use the normalized norm dot with unit z
 * to get blueish scale
 * 
 * distribute color evenly
 * 
 * Draw faces whose z value are larger later 
 * to avoid being covered
 */
void object::drawFace()
{
	// Assign color info for all faces
	for (auto& tri : mesh) {
		float nZ = getNorm(tri).content[2];  // dot product between norm and unit z
		tri.bValue = MINBLUE + nZ * (MAXBLUE - MINBLUE);
	}

	// sort mesh based on average z value
	sort(mesh.begin(), mesh.end(), [](face& t1, face& t2) {
		float z1 = (t1.p[0].content[2] + t1.p[1].content[2] + t1.p[2].content[2]) / 3.0f;
		float z2 = (t2.p[0].content[2] + t2.p[1].content[2] + t2.p[2].content[2]) / 3.0f;
		return z1 < z2;
		});

	// draw every face as triangle with assigned coclor
	for (auto& tri : mesh) {
		glColor3f(0.0f, 0.0f, tri.bValue);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; i++)
			glVertex3f(tri.p[i].content[0], tri.p[i].content[1], 0.0f);
		glEnd();
	}
}


/**
 * Rotate the 3D object about X-axis
 * by applying a rotation matrix to all vertices
 *
 * @param[in] ang, the rotation angle about X-axis
 */
void object::rotX(float ang)
{
	// init Rot matrix
	matf3 Rot;
	Rot.scalarMultiply(0.0f);
	Rot.set(1, 1, 1.0f);
	Rot.set(2, 2, cos(ang));
	Rot.set(2, 3, -sin(ang));
	Rot.set(3, 2, sin(ang));
	Rot.set(3, 3, cos(ang));

	// go through all vertices
	for (auto& tri : mesh)
		for (int i = 0; i < 3; i++) {
			vecf3 p_rot;
			Rot.vecMultiply(tri.p[i], p_rot);
			tri.p[i] = p_rot;
		}
}


/**
 * Rotate the 3D object about Y-axis
 * by applying a rotation matrix to all vertices
 *
 * @param[in] ang, the rotation angle about Y-axis
 */
void object::rotY(float ang)
{
	// init Rot matrix
	matf3 Rot;
	Rot.scalarMultiply(0.0f);
	Rot.set(1, 1, cos(ang));
	Rot.set(1, 3, sin(ang));
	Rot.set(2, 2, 1.0f);
	Rot.set(3, 1, -sin(ang));
	Rot.set(3, 3, cos(ang));

	// go through all vertices
	for (auto& tri : mesh)
		for (int i = 0; i < 3; i++) {
			vecf3 p_rot;
			Rot.vecMultiply(tri.p[i], p_rot);
			tri.p[i] = p_rot;
		}
}