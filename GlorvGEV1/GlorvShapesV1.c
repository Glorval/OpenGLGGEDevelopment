#pragma once
#include "GlorvGE_BaseV1.h"
#define WINDOW_X 1440
#define WINDOW_Y 1080





struct ShapeData createShape(float vertices[], unsigned int indices[], int vertSize, int indSize, int saveAll) {




	struct ShapeData Returns;
	glGenVertexArrays(1, &Returns.VAO);
	glGenBuffers(1, &Returns.VBO);
	glGenBuffers(1, &Returns.EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(Returns.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Returns.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &Returns.EBO);
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


//struct ShapeData createVertShape(float vertices[], int vertSize) {
//	struct ShapeData Returns;
//	glGenVertexArrays(1, &Returns.VAO);
//	glGenBuffers(1, &Returns.VBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(Returns.VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, Returns.VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_DYNAMIC_DRAW);
//
//	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// color attribute
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//
//
//	Returns.vertsize = vertSize;
//	Returns.vertices = vertices;
//	return(Returns);
//}


struct ShapeData createShapeFromFile(char vertFile[], char indFile[], int saveAll) {
	char vertFileFixed[100] = "Objects/";
	char indFileFixed[100] = "Objects/";
	strcat(vertFileFixed, vertFile);
	strcat(vertFileFixed, "Vertices.txt");
	strcat(indFileFixed, indFile);
	strcat(indFileFixed, "Indices.txt");

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

	char vertFile[100] = "Objects/";//Get to the right directory
	strcat(vertFile, &SaveShape.filename);//Put the name of the file as the folder name
	strcat(vertFile, "Vertices.txt");//All vertice files are named this
	FILE* vertSaveFile = fopen(vertFile, "w");//The place being written to is OurProgram/Objects/Filename/Vertices.txt

	for(int counter = 0; counter < SaveShape.vertsize; counter++){
		fprintf(vertSaveFile, "%f", SaveShape.vertices[counter]);
		if (counter != SaveShape.vertsize - 1) {
			fprintf(vertSaveFile, " ");
		}
	}

	char indFile[100] = "Objects/";//Get to the right directory
	strcat(indFile, &SaveShape.filename);//Put the name of the file as the folder name
	strcat(indFile, "Indices.txt");//All vertice files are named this
	FILE* indSaveFile = fopen(indFile, "w");//The place being written to is OurProgram/Objects/Filename/Vertices.txt

	for (int counter = 0; counter < SaveShape.indsize; counter++) {
		fprintf(indSaveFile, "%u", SaveShape.indices[counter]);
		if (counter != SaveShape.indsize - 1) {
			fprintf(indSaveFile, " ");
		}
	}


	fclose(indSaveFile);
}
















//DRAWING SHAPE BY HAND

int processingClick = 0;

void drawShapeCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		processingClick = 1;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		processingClick = 2;
	}
}





struct ShapeData drawShape(GLFWwindow* window, struct mainloopData maindata) {
	printf("Now drawing a shape.");
	float* vertices = calloc(1, sizeof(float));
	unsigned int* indices = calloc(1, sizeof(unsigned int));

	int points = 0;
	struct MouseData drawingdata;

	int stillDrawing = 1;

	glfwSetMouseButtonCallback(window, drawShapeCallback);

	while (stillDrawing) {
		if (processingClick == 1) {//we have a click
			processingClick = 0;
			points ++;

			double xpos, ypos;//We had a click so get the x/y
			glfwGetCursorPos(window, &xpos, &ypos);
			printf("%f, %f\n", xpos,ypos);

			vertices = realloc(vertices, (points* sizeof(float)) * 6);//give more memory in prep of storing the x/y forever, *6 is because 6 entries per vertice
			indices = realloc(indices, sizeof(unsigned int) * points);//Give more memory to this too to store the next indice

			//start to store all the new data, for now just make the shape red
			vertices[(points - 1) * 6] = (xpos - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
			vertices[((points - 1) * 6) + 1] = -(ypos - (WINDOW_Y / 2)) / (WINDOW_Y / 2);
			vertices[((points - 1) * 6) + 2] = (float)points/ (float)10;
			vertices[((points - 1) * 6) + 3] = (float)points/ (float)10;
			vertices[((points - 1) * 6) + 4] = .2;
			vertices[((points - 1) * 6) + 5] = 0.0;

			indices[points - 1] = points - 1;//Indices just have to go 0->the last one



		}else if (processingClick == 2) {//2 is to end
			stillDrawing = 0;
		}



		maindata.proecessinputelsewhere = 1;
		glfwPollEvents();
		maindata = mainLoop(maindata);//Run the main loop but without it processing input itself.
	}
	//indices = realloc(indices, (sizeof(unsigned int) * points) + 1);//Give more memory to connect first/last points
	//indices[points] = 0;
	for (int counter = 0; counter < points; counter++) {
		printf("%d", indices[counter]);
	}


	//Now done with creating all the vertices for the shape, so turn it into a shape object
	struct ShapeData returnshape = createShape(vertices, indices, points * sizeof(float) * 6, points * sizeof(unsigned int), 1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return(returnshape);


}





