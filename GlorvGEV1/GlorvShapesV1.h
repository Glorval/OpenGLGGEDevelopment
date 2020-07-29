#pragma once
#include "GlorvGE_BaseV1.h"


/*//Used with creating shaps to keep track of VAO and VBO IDs
struct ShapeData {
	unsigned int VAO, VBO;
	int vertsize, indsize;
	float* vertices;
	unsigned int *indices;
	int *fulldata;
};*/



//Creates a shape using OpenGL and returns a shapedata structure with the VAO/VBO, and if saveAll is enabled it also holds the vertice/indice data and size of said data 
struct ShapeData createShape(float vertices[], unsigned int indices[], int vertSize, int indSize, int saveAll);

//Used to create a shape that already exists as a file, this just loads the data from the file then sends to createShape
struct ShapeData createShapeFromFile(char vertFile[], char indFile[], int saveAll);


//Saves given shape to filen, shape given by raw data.
void saveShapeToFile(float vertices[], unsigned int indices, int vertSize, int indSize, char fileName);

//Saves given shape to filen, shape given by a shape structure.
void saveShapeToFileStruct(struct ShapeData SaveShape);