#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GlorvMath.h"
#include "GlorvShapesV1.h"
#include "linearAlg.h"


//Used in the main loop to keep track of all data, very important
struct mainloopData {
	struct ShapeData* AllLoadedShapes;
	int totalshapes;
	long int sizeofshapes;
	int totalindices;
	int proecessinputelsewhere;
	int waitingonconsole;
	int ending;//Whether or not to end the program
};



//Used with getShaderStrings to pull back two strings at once
struct ShaderStrings {
	char* vertexS;
	char* fragS;
};

//Used with creating shaps to keep track of VAO and VBO IDs
struct ShapeData {
	unsigned int VAO, VBO, EBO;//The vector buffer and vector array object IDs.
	int vertexcount, indexcount;//The amount of vertices/length of indices
	float* vertices;//An array of all the vertices in groups of 3, x y z
	unsigned int* indices;//An array of which vertices to render in groups of 3, point a, point b, point c
	int* fulldata;//whether or not it has all data used, VAOs/VBOs filename etc.
	char* filename;
	Vector4 location;//The location of the object in space
};
//typedef struct ShapeData newShape;

//Used to track mouse data
struct MouseData {
	int processingClick;
	double xposOnClick;//Last position for clicking
	double yposOnClick;
};

struct Setupdata {
	GLFWwindow* window;
	int shaderID;
};



//SETUP FUNCTIONS

//Gets the strings for the shader
struct ShaderStrings getShaderStrings();

//One time set up for the shaders
int setupShaders();


//Sets up Glad/GLFW, creates a window, loads openGL, sets up the shaders and returns a window pointer
struct Setupdata setupEVERYTHING(GLFWwindow* window);


struct mainloopData mainLoop(struct mainloopData mainData);




//USER INPUT BLOCK

unsigned int lastPressedKey;//Last pressed key char value
double lastPressedTime;//time of last pressed normal key
double lastSpecialPressTime;//Time of last pressed special key (Gaps should be bigger normally on these as enter hits are heavy)
int handlingLastPress;//Have we yet to check the last pressed key?

//Is used to detect a keypress, also saves the pressed key to a varaible for later use
void character_callback(GLFWwindow* window, unsigned int codepoint);

//Is used to detect when the enter key is hit, useful for end of typing
int enterDetection;
void enterDetector(GLFWwindow* window, int key, int scancode, int action, int mods);


//Used to wait for a keypress, only useful for if you want to wait for a single input for yes/no, single number, etc.
void waitForKeyPress(GLFWwindow* window, int display);

//Warning: Will stay in function until keypress. DO NOT USE FOR ONE OFF KEYPRESS
//Used as a singluar function to return individual pressed keys until enter is hit, display is whether or not to print to console as typing
char keyReader(GLFWwindow* window, int display);


//Warning, will stay in function until enter is hit
//Used to get typing input reliably, display displays it, nullterm keeps the \0 at the end of the string.
void typing(GLFWwindow* window, int display, int nullterm, char* string);