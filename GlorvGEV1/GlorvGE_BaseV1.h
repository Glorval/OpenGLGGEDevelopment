#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GlorvMath.h"
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




//USER INPUT BLOCK

unsigned int lastPressedKey;
double lastPressedTime;
int handlingLastPress;

//Is used to detect a keypress, also saves the pressed key to a varaible for later use
void character_callback(GLFWwindow* window, unsigned int codepoint);

//Is used to detect when the enter key is hit, useful for end of typing
int enterDetection;
void enterDetector(GLFWwindow* window, int key, int scancode, int action, int mods);

//Warning: Will stay in function until keypress. DO NOT USE FOR ONE OFF KEYPRESS
//Used as a singluar function to return pressed keys, display is whether or not to print to console as typing. Proper format for typing input is while(returnofkeyreader != '\0'){returnofkeyreader = keyreader()}
char keyReader(GLFWwindow* window, int display);