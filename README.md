# README

shaobow@andrew.cmu.edu

## 3D Object Viewer

### Introduction

This program is written to load a 3D object file and display it through a display window. 

### Instruction

#### 1. Object file format

The format of the file is:

- The first line contains two integers. The first integer is the number of vertices that define the 3D object, and the second number is the number of faces that define the 3D object.
- Starting at the second line each line will define one vertex of the 3D object and will consist of an integer followed by three real numbers. The integer is the ID of the vertex and the three real numbers define the (x,y,z) coordinates of the vertex. The number of lines in this section will be equal to the first integer in the file.
- Following the vertex section will be a section defining the faces of the 3D object. The number of lines in this section will be equal to the second integer on the first line of the file. Each line in this section will consist of three integers that define a triangle that is the face of the object. The three integers each refer to the ID of a vertex from the second section of the file.

#### 2. Users guide

##### 2.1 Rotate operation

Use a mouse to left-click and drag the object to rotate in 3D space. 

Use the keyboard to rotate:

- "UP": rotate upward about the X-axis
- "DOWN": rotate downward about the X-axis
- "LEFT": rotate leftward about Y-axis
- "RIGHT": rotate rightward about Y-axis

##### 2.2 File name input

Type in the filename right after running the program.

##### 2.3 Other features

Press "R" to restore the pose of the 3D object to the original

Press "C" to change between colored face mode and uncolored line mode

Press "X" or "Esc" to exit the program

