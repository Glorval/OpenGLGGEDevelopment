#pragma once
#include "GlorvGE_BaseV1.h"
#define WINDOW_X 1440
#define WINDOW_Y 1080

#define VERTEX_SIZE 6 * sizeof(float)//The size in bytes of a vertex, currently 6 because x/y/z r/g/b
#define IND_SIZE 1 * sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b


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
double mouseX, mouseY;

void drawShapeCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		processingClick = 1;
		glfwGetCursorPos(window, &mouseX, &mouseY);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &mouseX, &mouseY);
		processingClick = 2;
	}
}





//struct ShapeData drawShape(GLFWwindow* window, struct mainloopData maindata) {
//	printf("Now drawing a shape.");
//	float* vertices = calloc(1, sizeof(float));
//	unsigned int* indices = calloc(1, sizeof(unsigned int));
//
//	int points = 0;
//	struct MouseData drawingdata;
//
//	int stillDrawing = 1;
//
//	glfwSetMouseButtonCallback(window, drawShapeCallback);
//	struct ShapeData returnshape;
//	returnshape.vertices = calloc(1, sizeof(float));
//	returnshape.indices = calloc(1, sizeof(unsigned int));
//
//	while (stillDrawing) {
//		if (points > 3) {
//			glBindVertexArray(returnshape.VAO);
//			glBindBuffer(GL_ARRAY_BUFFER, returnshape.VBO);
//			
//		}
//		if (processingClick == 1) {//we have a click
//			processingClick = 0;
//			points++;
//
//			double xpos, ypos;//We had a click so get the x/y
//			glfwGetCursorPos(window, &xpos, &ypos);
//			printf("%f, %f\n", xpos, ypos);
//
//			returnshape.vertices = realloc(returnshape.vertices, (points * sizeof(float)) * 7);//give more memory in prep of storing the x/y forever, *6 is because 6 entries per vertice
//			returnshape.indices = realloc(returnshape.indices, sizeof(unsigned int) * points);//Give more memory to this too to store the next indice
//
//			//start to store all the new data, for now just make the shape red
//			returnshape.vertices[(points - 1) * 6] = (xpos - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
//			returnshape.vertices[((points - 1) * 6) + 1] = -(ypos - (WINDOW_Y / 2)) / (WINDOW_Y / 2);
//			returnshape.vertices[((points - 1) * 6) + 2] = 0;
//			returnshape.vertices[((points - 1) * 6) + 3] = 0;
//			returnshape.vertices[((points - 1) * 6) + 4] = 0.5;
//			returnshape.vertices[((points - 1) * 6) + 5] = 0.0;
//
//			returnshape.indices[points - 1] = points - 1;//Indices just have to go 0->the last one
//
//
//			printf("\n\n\n");
//			for (int counter = 0; counter < points; counter++) {
//				printf("%f, %f \n", returnshape.vertices[counter * 6], returnshape.vertices[(counter + 1) * 6]);
//			}
//			printf("\n\n\n");
//
//		}
//		else if (processingClick == 2) {//2 is to end
//			stillDrawing = 0;
//		}
//		
//		if (points == 3) {
//			returnshape = createShape(vertices, indices, points * sizeof(float) * 6, (points) * sizeof(unsigned int), 1);
//		}
//		
//		
//		
//		if (points > 3) {
//			glDrawElements(GL_TRIANGLE_STRIP, points, GL_UNSIGNED_INT, returnshape.indices);
//		}
//		
//
//		
//		
//
//		maindata.proecessinputelsewhere = 1;
//		glfwPollEvents();
//
//
//		maindata = mainLoop(maindata);//Run the main loop but without it processing input itself.
//
//
//
//
//		/*if (points > 3) {
//			returnshape.vertices = vertices;
//			returnshape.indices = indices;
//			glBindVertexArray(returnshape.VAO);
//			
//
//			glDrawElements(GL_TRIANGLE_STRIP, points, GL_UNSIGNED_INT, indices);
//			glBindVertexArray(0);
//		}*/
//	}
//
//
//	returnshape.indices = realloc(returnshape.indices, sizeof(unsigned int) * (points + 2));
//	returnshape.indices[points] = 0;
//	returnshape.indices[points + 1] = 1;
//
//
//	//Now done with creating all the vertices for the shape, so turn it into a shape object
//	//struct ShapeData returnshape = createShape(vertices, indices, points * sizeof(float) * 6, (points + 2) * sizeof(unsigned int), 1);
//	
//
//	
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	printf("Done\n");
//	return(returnshape);
//
//
//}














//Plan for how it will work: Get all the vertices while rendering them as points, then the user clicks on pairs of 3 vertices to connect them as a triangle.  
//Lastly the user should be able to click a vertex and then have it follow the mouse until they click again, and a way to switch between these 3 modes of 'vector creation' 'vector connection' and 'vector changing'
struct ShapeData drawShape(GLFWwindow* window, struct mainloopData maindata) {
	printf("Now drawing a shape.");



	//USED FOR VERTEX CREATION
	int points = 0;
	float* vertices;//to store the vertices in easily
	int* indices;//same

	vertices = calloc(1, sizeof(float));//give them something so we can realloc later
	indices = calloc(1, sizeof(unsigned int));
	//END OF USED FOR VERTEX CREATION	



	

	struct ShapeData drawnshape;//The structure that we will return and render from


	glfwSetMouseButtonCallback(window, drawShapeCallback);//So when we click we can update the processing click global variable

	//modes
	int VERT_CREATE = 1;
	int VERT_CONNECT = 2;
	int VERT_CHANGE = 3;
	//end of modes


	int stillCreating = 1;//Basically just keep 1 until we're done with the shape
	int mode = VERT_CREATE;//Set the mode to vertex creation so that we can actually start with making vertices
	while (stillCreating == 1) {//While we are still creating the shape, lets us loop between the 3 modes until we are finished.





		//START OF VERTEX CREATION
		if (mode == VERT_CREATE) {
			printf("\nMode: Vertex Creation\n");
			int stillInMode = 1;
			while (stillInMode) {
				if (processingClick == 1) {//we have a click
					processingClick = 0;//reset as we are now dealing with it
					points++;

					double xpos, ypos;//We had a click so get the x/y
					glfwGetCursorPos(window, &xpos, &ypos);

					vertices = realloc(vertices, VERTEX_SIZE * points);//give more memory in prep of storing the x/y forever, *6 is because 6 entries per vertice
					indices = realloc(indices, IND_SIZE * points);//Give more memory to this too to store the next indice

					//start to store all the new data, for now just make the shape red
					
					vertices[(points - 1) * 6] = (xpos - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
					vertices[((points - 1) * 6) + 1] = -(ypos - (WINDOW_Y / 2)) / (WINDOW_Y / 2);
					vertices[((points - 1) * 6) + 2] = 0;
					vertices[((points - 1) * 6) + 3] = 0;
					vertices[((points - 1) * 6) + 4] = 0.5;
					vertices[((points - 1) * 6) + 5] = 0.0;
					//end of storing data
					printf("%f, %f\n", vertices[(points - 1) * 6], vertices[((points - 1) * 6) + 1]);
					indices[points - 1] = points - 1;//Indices just have to go 0->the last one


					//printf("\n\n\n");//DEBUG REMOVE
					//for (int counter = 0; counter < points; counter++) {//print all the vertice data
					//	printf("%f, %f \n", vertices[counter * 6], vertices[(counter * 6) + 1]);
					//}
					//printf("\n\n\n");

				}
				else if (processingClick == 2) {//We have a right click so stop drawing now
					processingClick = 0;//No need for right clicks here
				}

				if (points == 1) {
					drawnshape = createShape(vertices, indices, points * VERTEX_SIZE, points * IND_SIZE, 1);
				}
				else {//BUG Potential, if you place one point and try to move it drawnshape wont have the updated info, MAYBE
					drawnshape.vertices = vertices;
					drawnshape.indices = indices;
					glBufferData(GL_ARRAY_BUFFER, points * VERTEX_SIZE, drawnshape.vertices, GL_DYNAMIC_DRAW);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, points * IND_SIZE, drawnshape.indices, GL_DYNAMIC_DRAW);
				}


				if (points > 0) {//Draw our points, if we have any that is
					glPointSize(10.0f);
					glBindVertexArray(drawnshape.VAO);
					glDrawElements(GL_TRIANGLE_STRIP, points, GL_UNSIGNED_INT, drawnshape.indices);
					//glBindVertexArray(0);
				}


				//GL_POINTS
				//GL_TRIANGLE_STRIP


				/*if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
					mode = VERT_CREATE;
					stillInMode = 0;
				}else*/ if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
					mode = VERT_CHANGE;
					stillInMode = 0;
				} else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
					mode = VERT_CONNECT;
					stillInMode = 0;
				}





				//do everything else
				maindata.proecessinputelsewhere = 1;
				glfwPollEvents();
				maindata = mainLoop(maindata);//Run the main loop but without it processing input itself.

			}
		}
		//END OF VERTEX CREATION


		//START OF VERTEX CHANGING
		if (mode == VERT_CHANGE) {
			printf("In Vertex Changing\n");
			int stillInMode = 1;

			int selectedPoint = 0;//Do we have a selected point
			int selectedPointPos = 0;//The position in the array of the selected point
			int closestPointNumber = 0;//Which one in the array is the closest point, points to the X value


			while (stillInMode == 1) {
				




				if (processingClick == 1) {//Left click, so let us see whats goin on
					processingClick = 0;

					//START OF FINDING POINT
					if (selectedPoint == 0) {//We do NOT have a point selected yet SO LETS FIND ONE HOOAH
						mouseX = (mouseX - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
						mouseY = -(mouseY - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);
						float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS
						


						for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.
							float currentDistance = distanceTwoD(drawnshape.vertices[counter * VERTEX_LENGTH], mouseX, drawnshape.vertices[(counter * VERTEX_LENGTH) + 1], mouseY);
							if (currentDistance < closestDistance) {
								closestDistance = currentDistance;
								closestPointNumber = counter * VERTEX_SIZE; 
							}
						}

						selectedPoint = 1;
					}
					//END OF FINDING POINT


				//END OF LEFT CLICK

				//START OF RIGHT CLICK
				} else if (processingClick == 2) {//Right click stops the movement of the point
					processingClick = 0;
					selectedPoint = 0;//Deselects the point
				}
				//END OF RIGHT CLICK




				//START OF MOVING POINT
				if (selectedPoint == 1) {//Do we have a point selected?
					double* xy = calloc(2, sizeof(double));
					glfwGetCursorPos(window, &xy[0], &xy[1]);
					float XY[2];
					XY[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
					XY[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);
					drawnshape.vertices[closestPointNumber/ sizeof(float)] = XY[0];
					drawnshape.vertices[(closestPointNumber / sizeof(float)) + 1] = XY[1];
					glBufferSubData(GL_ARRAY_BUFFER, closestPointNumber, 2 * sizeof(float), XY);
				}
				//END OF MOVING POINT




				//Draw our new stuff
				glPointSize(10.0f);
				glBindVertexArray(drawnshape.VAO);
				// GL_POINTS
				glDrawElements(GL_TRIANGLE_STRIP,  points, GL_UNSIGNED_INT, drawnshape.indices);
				//glBindVertexArray(0);
				//End of drawing new stuff




				//do everything else
				maindata.proecessinputelsewhere = 1;
				glfwPollEvents();
				maindata = mainLoop(maindata);//Run the main loop but without it processing input itself.

				//Loop starts again
			}

			//ending
		}
		//END OF VERTEX CHANGING









	stillCreating = 0;








	}
	
	//Done with getting the vertices








	//Now done with creating all the vertices for the shape, so turn it into a shape object
	//struct ShapeData returnshape = createShape(vertices, indices, points * sizeof(float) * 6, (points + 2) * sizeof(unsigned int), 1);
	


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	printf("Done\n");
	return(drawnshape);


}