#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "GlorvShapesV1.h"



//Used with getShaderStrings to pull back two strings at once
struct ShaderStrings {
	char* vertexS;
	char* fragS;
};

//Used with creating shaps to keep track of VAO and VBO IDs
struct ShapeData {
	unsigned int VAO, VBO;
	int vertsize, indsize;
	float* vertices;
	unsigned int *indices;
	int *fulldata;
	char* filename;
};
typedef struct ShapeData newShape;


//SETUP FUNCTIONS

//Gets the strings for the shader
struct ShaderStrings getShaderStrings();

//One time set up for the shaders
void setupShaders();

//Sets up Glad/GLFW, creates a window, loads openGL, sets up the shaders and returns a window pointer
GLFWwindow* setupEVERYTHING(GLFWwindow* window);

//END OF SETUP FUNCTIONS



//SHAPE CREATION/SAVING

////Takes in the vertices, indices, and the sizes of both and returns the VAO and VBO IDs
//struct ShapeData createShape(float vertices[], unsigned int indices[], int vertSize, int indSize, int saveAll);
//
//
//
//struct ShapeData createShapeFromFile(char vertFile[], char indFile[], int saveAll);