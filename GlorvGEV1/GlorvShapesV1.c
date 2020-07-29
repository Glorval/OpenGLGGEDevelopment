#pragma once
#include "GlorvGE_BaseV1.h"







struct ShapeData createShape(float vertices[], unsigned int indices[], int vertSize, int indSize, int saveAll) {




	struct ShapeData Returns;
	unsigned int EBO;
	glGenVertexArrays(1, &Returns.VAO);
	glGenBuffers(1, &Returns.VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(Returns.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Returns.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, indices, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (saveAll == 1) {
		Returns.fulldata = 1;
		Returns.indices = indices;
		Returns.vertices = vertices;
		Returns.vertsize = vertSize;
		Returns.indsize = indSize;
	}

	return(Returns);
}





struct ShapeData createShapeFromFile(char vertFile[], char indFile[], int saveAll) {
	char vertFileFixed[100] = "Objects/";
	char indFileFixed[100] = "Objects/";
	strcat(vertFileFixed, vertFile);
	strcat(vertFileFixed, "/Vertices.txt");
	strcat(indFileFixed, indFile);
	strcat(indFileFixed, "/Indices.txt");

	//Vertex reading
	FILE* verts = fopen(vertFileFixed, "r");
	float* vertices;
	vertices = (float*)calloc(10, sizeof(float));


	int counter = 0;
	int freespaces = 10;
	while (1) {//Vertex reading



		//Read entry from file
		if (fscanf(verts, "%f", &vertices[counter]) == EOF) {
			break;
		}

		//printf("%f\n", vertices[counter]);//DEBUG VERTICE PRINTING


		counter++;
		freespaces--;//Keep track of how many spaces we have left
		if (freespaces == 0) {//No more spaces = ADD MOAR SPACE
			freespaces = 10;
			//printf("Had to allocate %d more blocks of memory for createShapeFromFile Vertices.\n", counter/10);
			vertices = (float*)realloc(vertices, (10 + counter) * sizeof(float));//Hush, nobody cares if it fails
		}
	}
	printf("Had to allocate %d bytes of memory for CreatShapeFromFile Vertices.\n", counter);
	int vertSize = counter * sizeof(float);
	vertices = (float*)realloc(vertices, vertSize);
	fclose(verts);







	//Indice reading
	counter = 0;//Reset the tracking variables
	freespaces = 10;

	FILE* inds = fopen(indFileFixed, "r");
	unsigned int* indices;
	indices = (unsigned int*)calloc(10, sizeof(unsigned int));


	while (1) {//Indice reading

		//Read entry from file
		if (fscanf(inds, "%u", &indices[counter]) == EOF) {
			break;
		}


		//printf("%f\n", vertices[counter]);//DEBUG VERTICE PRINTING

		counter++;
		freespaces--;//Keep track of how many spaces we have left
		if (freespaces == 0) {//No more spaces = ADD MOAR SPACE
			freespaces = 10;
			//printf("Had to allocate %d more blocks of memory for createShapeFromFile Indices.\n", counter / 10);
			indices = (unsigned int*)realloc(indices, (10 + counter) * sizeof(unsigned int));//Hush, nobody cares if it fails
		}
	}
	printf("Had to allocate %d bytes of memory for CreatShapeFromFile Indices.\n", counter);
	int indSize = counter * sizeof(unsigned int);
	indices = (unsigned int*)realloc(indices, indSize);
	fclose(inds);



	struct ShapeData newshape = createShape(vertices, indices, vertSize, indSize, saveAll);
	return(newshape);
}


//Saves given shape to filen, shape given by raw data.
void saveShapeToFile(float vertices[], unsigned int indices, int vertSize, int indSize, char fileName) {
	char* token;

	char vertFile[100] = "Objects/";//Get to the right directory
	strcat(vertFile, fileName);//Put the name of the file as the folder name
	strcat(vertFile, "/Vertices.txt");//All vertice files are named this
	FILE* vertSaveFile = fopen(vertFile, "w");//The place being written to is OurProgram/Objects/Filename/Vertices.txt
	fputs(vertices, vertFile);

}




//Saves given shape to filen, shape given by a shape structure.
void saveShapeToFileStruct(struct ShapeData SaveShape) {
	char* token;
	printf("%s", &SaveShape.filename);
	char vertFile[100] = "Objects/";//Get to the right directory
	strcat(vertFile, &SaveShape.filename);//Put the name of the file as the folder name
	strcat(vertFile, "/Vertices.txt");//All vertice files are named this
	FILE* vertSaveFile = fopen(vertFile, "w");//The place being written to is OurProgram/Objects/Filename/Vertices.txt

	fputs(SaveShape.vertices, vertFile);

}