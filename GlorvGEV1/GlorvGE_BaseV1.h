#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GlorvShapesV1.h"


//Used in the main loop to keep track of all data, very important
struct mainloopData {
	struct ShapeData* AllLoadedShapes;
	int totalshapes;
	long int sizeofshapes;
	int totalindices;
	int proecessinputelsewhere;
	int waitingonconsole;
};


//Used with getShaderStrings to pull back two strings at once
struct ShaderStrings {
	char* vertexS;
	char* fragS;
};

//Used with creating shaps to keep track of VAO and VBO IDs
struct ShapeData {
	unsigned int VAO, VBO, EBO;//The vector buffer and vector array object IDs.
	int vertsize, indsize;//The sizes of the vertice and indice arrays in their bytes
	float* vertices;//An array of all the vertices in groups of 3, x y z
	unsigned int* indices;//An array of which vertices to render in groups of 3, point a, point b, point c
	int* fulldata;//whether or not it has all data used, VAOs/VBOs filename etc.
	char* filename;
};
//typedef struct ShapeData newShape;

//Used to track mouse data
struct MouseData {
	int processingClick;
	double xposOnClick;//Last position for clicking
	double yposOnClick;
};





//SETUP FUNCTIONS

//Gets the strings for the shader
struct ShaderStrings getShaderStrings();

//One time set up for the shaders
void setupShaders();


//Sets up Glad/GLFW, creates a window, loads openGL, sets up the shaders and returns a window pointer
GLFWwindow* setupEVERYTHING(GLFWwindow* window);


struct mainloopData mainLoop(struct mainloopData mainData);