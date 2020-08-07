#pragma once
#include "GlorvGE_BaseV1.h"
#define WINDOW_X 1440
#define WINDOW_Y 1080

#define VERTEX_SIZE 6 * sizeof(float)//The size in bytes of a vertex, currently 6 because x/y/z r/g/b
#define IND_SIZE 1 * sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b

#define DEPTH_ADJUSTER 20000//How much to divide user input by when making 2d objects and layering

#define CHAR_NUM_TO_NUM (int) '0' - 1



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

int closestVert(struct ShapeData givenshape, int points, GLFWwindow* window) {
	int closestPointNumber = 0;

	double* xy = calloc(2, sizeof(double));
	glfwGetCursorPos(window, &xy[0], &xy[1]);

	xy[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
	xy[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);

	float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS

	for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

		double currentDistance = distanceTwoDD(givenshape.vertices[counter * VERTEX_LENGTH], xy[0], givenshape.vertices[(counter * VERTEX_LENGTH) + 1], xy[1]);//Find the current distance
		if (currentDistance < closestDistance) {//If the current distance is so far the shortest, save it 
			closestDistance = currentDistance;
			closestPointNumber = counter;
		}

	}
	return(closestPointNumber);
}







int processingClick = 0;
//double mouseX, mouseY;

void drawShapeCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		processingClick = 1;
		//glfwGetCursorPos(window, &mouseX, &mouseY);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//glfwGetCursorPos(window, &mouseX, &mouseY);
		processingClick = 2;
	}
}




unsigned int lastPressedKey = 0;
double lastPressedTime = 0;
int handlingLastPress = 0;

void character_callback(GLFWwindow* window, unsigned int codepoint) {
	if (lastPressedTime < glfwGetTime() - .05) {
		handlingLastPress = 1;
		lastPressedKey = codepoint;
		lastPressedTime = glfwGetTime();
	}
}

int enterDetection = 0;
void enterDetector(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ENTER) {
		enterDetection = 1;
	}
}




//Plan for how it will work: Get all the vertices while rendering them as points, then the user clicks on pairs of 3 vertices to connect them as a triangle.  
//Lastly the user should be able to click a vertex and then have it follow the mouse until they click again, and a way to switch between these 3 modes of 'vector creation' 'vector connection' and 'vector changing'

struct ShapeData drawShape(GLFWwindow* window, struct mainloopData maindata) {
	printf("Now drawing a shape.\n\n\n");
	
	glfwSetCharCallback(window, character_callback);//For tippy typing


	//Variable stack
	int menuPosition = 0;//The position in the menu 'function.'
	int drawVertices = 1;

	double TIME_BETWEEN_MODES = .25;//The minimum time between switching modes.

	//VERTEX AUXILLERY DATA, CARRIES THROUGH TO MANY VERTICES
		float red, green, blue;//Currently selected colour
		red = 1.0;
		green = 0.0;
		blue = 0.0;

		struct LayerNames* userLayerNames;
		int currentLayer = -1;//Keeps track of the current layer
		userLayerNames = malloc(sizeof(struct LayerNames) * 10);//Allows the user to store 10 layers
		for (int counter = 0; counter < 10; counter++) {
			userLayerNames[counter].name = NULL;
		}
	//VERTEX AUXILLERY DATA, CARRIES THROUGH TO MANY VERTICES


	//USED FOR VERTEX CREATION
		int points = 0;
		float* vertices;//to store the vertices in easily
		int* indices;//same
		int totalIndices = 0;//To store how many indices we have total, cause ya know thats important

		vertices = calloc(1, sizeof(float));//give them something so we can realloc later
		indices = calloc(1, sizeof(unsigned int));
	//END OF USED FOR VERTEX CREATION	


	//USED FOR VERTEX MOVING
		int selectedPoint = 0;//Do we have a selected point
		int selectedPointPos = 0;//The position in the array of the selected point
		int closestPointNumber = 0;//Which one in the array is the closest point, points to the X value
	//END OF USED FOR VERTEX MOVING



	

	struct ShapeData drawnshape;//The structure that we will return and render from


	glfwSetMouseButtonCallback(window, drawShapeCallback);//So when we click we can update the processing click global variable

	//modes
		int VERT_CREATE = 1;
		int VERT_CONNECT = 2;
		int VERT_CHANGE = 3;
		int VERT_COLOUR = 4;
		int END_OF_CREATION = 0;
		int MENU = -1;

		//SET DEFAULT MODE
		int mode = VERT_CREATE;//Set the mode to vertex creation so that we can actually start with making vertices
	//end of modes


	int stillCreating = 1;//Basically just keep 1 until we're done with the shape

	double time = 0;
	double timeAtLastPrintf = 0;//Used to make sure we dont spam the crap out of the 'Mode is: x' text
	//End of variable stack



	while (stillCreating == 1) {//While we are still creating the shape, lets us loop between the 3 modes until we are finished.
		

		//START OF VERTEX CREATION
		if (mode == VERT_CREATE) {

			//IF WE HAVE A LEFT CLICK TO CREATE, SAVE ALL VERTICIES NO LONGER SAVES INDICES BECAUSE CONNECT MODE
			if (processingClick == 1) {//we have a click
				processingClick = 0;//reset as we are now dealing with it
				points++;

				double xpos, ypos;//We had a click so get the x/y
				glfwGetCursorPos(window, &xpos, &ypos);

				vertices = realloc(vertices, VERTEX_SIZE * points);//give more memory in prep of storing the x/y forever, *6 is because 6 entries per vertice

				//start to store all the new data, for now just make the shape red
					
				vertices[(points - 1) * 6] = (xpos - (.5 * WINDOW_X)) / (.5 * WINDOW_X);//x
				vertices[((points - 1) * 6) + 1] = -(ypos - (WINDOW_Y / 2)) / (WINDOW_Y / 2);//y
				if (currentLayer == -1) {//No layer set, default to zero
					vertices[((points - 1) * 6) + 2] = 0;//z
				} else {
					vertices[((points - 1) * 6) + 2] = userLayerNames[currentLayer].depth;//z
				}
				vertices[((points - 1) * 6) + 3] = red;//red
				vertices[((points - 1) * 6) + 4] = green;//green
				vertices[((points - 1) * 6) + 5] = blue;//blue
				//end of storing data

			}
			//VERTEX DELETION
			else if (processingClick == 2) {//Right click deletes vertices
				processingClick = 0;//Reset the click
				int vertToDelete = closestVert(drawnshape, points, window);//Find the closest vertice to the click
				

				for (int current = vertToDelete * VERTEX_LENGTH; current < (points - 1) * VERTEX_LENGTH; current = current + VERTEX_LENGTH) {//This starts at the vertice to get rid of and goes through all the rest of the vertices
					for(int oneOfSix = 0; oneOfSix < 6; oneOfSix++){//and goes through all the rest of the vertices pushing them back a slot, removing a point counter (Because one got deleted) and then eventually freeing memory from OpenGL
						vertices[current + oneOfSix] = vertices[current + VERTEX_LENGTH + oneOfSix];
					}
				}
				points--;//We got rid of a point
				vertices = realloc(vertices, VERTEX_SIZE * points);//can re-allocate memory again, not strictly needed but oh well prevents garbage entries
				drawnshape.vertices = vertices;
				glBufferData(GL_ARRAY_BUFFER, points * VERTEX_SIZE, drawnshape.vertices, GL_DYNAMIC_DRAW);//Reallocate all the memory used by opengl

				

				//now we have removed the vertice, updated openGl, now we have to re-arrange any indices.
				//The way this will be done: Look at each group of three indices (Triangle) and if any of the three reference it REMOVE THEM and shift everything over.
				for (int currentThree = 0; currentThree < totalIndices; currentThree = currentThree + 3) {//Run through all indices 3 at a time
					if ((indices[currentThree] == vertToDelete) || (indices[currentThree + 1] == vertToDelete) || (indices[currentThree + 2] == vertToDelete)) {//Do any of the three point to the removed vertex?
						if (currentThree == totalIndices - 3) {//Is this the last trio of index entries? Because if so we just need to cut the memory off
							indices = realloc(indices, IND_SIZE * totalIndices);//Just chop it off
						}else{
							for (int counter = currentThree; counter < totalIndices - 3; counter += 3) {//Shift everything back a trio overwriting the useless entry
								indices[counter] = indices[counter + 3];
								indices[counter + 1] = indices[counter + 4];
								indices[counter + 2] = indices[counter + 5];
							}
							currentThree = currentThree - 3;//Just back this up to make sure we dont skip any, because if it goes entry 1, 2, 3 and we remove 1 it becomes 2, 3, 3 with the last 3 being cutoff but we're now in POSITION 2, past 2
							indices = realloc(indices, IND_SIZE * totalIndices);//Then chop off the duplicate end
						}
						totalIndices = totalIndices - 3;//We have removed a trio of entries so keep track of it
					}
				}

				for (int current = 0; current < totalIndices; current++) {//Run through all indices that remain and if any were referencing a vertex past the deleted one, lower the value they point to
					if (indices[current] > vertToDelete) {//The reason for this is because if we have vertex 0, 1, 2 ,3 ,4 and remove 2, 3 becomes 2, 4 becomes 3 and we need the pointer to 4 to point to 3
						indices[current] --;
					}
				}	
				drawnshape.indices = indices;

			//END OF VERTEX DELETION
			}
			//END OF CLICK REGISTERING




			//CREATE SHAPEDATA IF WE CREATE A VERTEX
			if (points == 1) {//If we have our first point, create the actual data in opengl for it
				drawnshape = createShape(vertices, indices, points * VERTEX_SIZE, points * IND_SIZE, 1);
			}
			//OTHERWISE UPDATE THE SHAPE
			else if(points > 1) {//BUG Potential, if you place one point and try to move it drawnshape wont have the updated info, MAYBE
				drawnshape.vertices = vertices;
				//drawnshape.indices = indices;
				glBufferData(GL_ARRAY_BUFFER, points * VERTEX_SIZE, drawnshape.vertices, GL_DYNAMIC_DRAW);
				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, points * IND_SIZE, drawnshape.indices, GL_DYNAMIC_DRAW);
			}

		}//END OF IF STATEMENT
		//END OF VERTEX CREATION


		//START OF VERTEX CHANGING
		else if (mode == VERT_CHANGE) {

			if (processingClick == 1) {//Left click, so let us see whats goin on
				processingClick = 0;

				//START OF FINDING POINT
				if (selectedPoint == 0) {//We do NOT have a point selected yet SO LETS FIND ONE HOOAH
					double* xy = calloc(2, sizeof(double));
					glfwGetCursorPos(window, &xy[0], &xy[1]);
					float XY[2];
					XY[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
					XY[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);
					float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS
						

					for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

						float currentDistance = distanceTwoD(drawnshape.vertices[counter * VERTEX_LENGTH], XY[0], drawnshape.vertices[(counter * VERTEX_LENGTH) + 1], XY[1]);//Find the current distance
						if (currentDistance < closestDistance) {//If the current distance is so far the shortest, save it 
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




				//START OF MOVING POINT, FOLLOW THE CURSOUR
				if (selectedPoint == 1) {//Do we have a point selected?
					double* xy = calloc(2, sizeof(double));
					glfwGetCursorPos(window, &xy[0], &xy[1]);//Get the x/y in pixles

					float XY[2];
					XY[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);//convert the double x/y in pixles to float x/y in graph
					XY[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);

					drawnshape.vertices[closestPointNumber/ sizeof(float)] = XY[0];//save in the shapedata
					drawnshape.vertices[(closestPointNumber / sizeof(float)) + 1] = XY[1];

					glBufferSubData(GL_ARRAY_BUFFER, closestPointNumber, 2 * sizeof(float), XY);//update opengl
				}
				//END OF MOVING POINT

		}
		//END OF VERTEX CHANGING SEGMENT


		//START OF VERTEX CONNECTING
		else if (mode == VERT_CONNECT) {
			int closestPointNumber = 0;//The closest vertex entry
			int tempIndices[3];//To temporarily store the indices until we have 3
			int howManyIndices = 0;//To indicate when we have 3 indices stored.


			if (processingClick == 1) {//left click, find the closest point and its array value and save an index to it
				processingClick = 0;

				//FIND CLOSEST POINT TO CLICK
				double* xy = calloc(2, sizeof(double));
				glfwGetCursorPos(window, &xy[0], &xy[1]);

				float XY[2];

				XY[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
				XY[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);

				float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS

				for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

					float currentDistance = distanceTwoD(drawnshape.vertices[counter * VERTEX_LENGTH], XY[0], drawnshape.vertices[(counter * VERTEX_LENGTH) + 1], XY[1]);//Find the current distance
					if (currentDistance < closestDistance) {//If the current distance is so far the shortest, save it 
						closestDistance = currentDistance;
						closestPointNumber = counter;
					}

				}

				//Now we have the closest point, so make a temp index out of it.
				//BUG CURRENTLY SAVES INDICES EVEN IF NOT  FULLY CREATED. NEEDS TO GATHER THREE THEN SAVE ALL THREE AT ONCE
				totalIndices++;
					
				indices = realloc(indices, sizeof(unsigned int) * totalIndices);//Grab more memory to store em in
				indices[totalIndices - 1] = closestPointNumber;
				drawnshape.indices = indices;

				} else if (processingClick == 2) {//Right click. Does nothing at the moment
					processingClick = 0;
				}

		}
		//END OF VERTEX CONNECTION


		//VERT COLOUR CHANGING
		else if (mode == VERT_COLOUR) {
		int closestPointNumber = 0;//The closest vertex entry

			if (processingClick == 1) {//left click, find the closest point and switch its colour to the currently active colour
				processingClick = 0;

				//FIND CLOSEST POINT TO CLICK
				double* xy = calloc(2, sizeof(double));
				glfwGetCursorPos(window, &xy[0], &xy[1]);

				xy[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
				xy[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);

				float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS

				for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

					double currentDistance = distanceTwoDD(drawnshape.vertices[counter * VERTEX_LENGTH], xy[0], drawnshape.vertices[(counter * VERTEX_LENGTH) + 1], xy[1]);//Find the current distance
					if (currentDistance < closestDistance) {//If the current distance is so far the shortest, save it 
						closestDistance = currentDistance;
						closestPointNumber = counter;
					}

				}//End of tracking left click, we have a point  now


				//so now that we have a point, update its colour to the active colour

				//update shapedata
				drawnshape.vertices[(closestPointNumber * VERTEX_LENGTH) + 3] = red;
				drawnshape.vertices[(closestPointNumber * VERTEX_LENGTH) + 4] = green;
				drawnshape.vertices[(closestPointNumber * VERTEX_LENGTH) + 5] = blue;
				//end of update shapedata

				//update openGLs data next
				float datashifter[3];
				datashifter[0] = red;
				datashifter[1] = green;
				datashifter[2] = blue;//						Get to the right portion of the vertices, then go to the third one (Start of RGB)
				glBufferSubData(GL_ARRAY_BUFFER, (closestPointNumber * VERTEX_SIZE) + (sizeof(float) * 3), 3 * sizeof(float), datashifter);//update opengl



			//End of processing left click
		} else if (processingClick == 2) {//Right click copies the nearest colour
			//TO DO
		}



		}
		//END OF VERT COLOUR

		//MENU TO SELECT STUFF
		else if (mode == MENU) {
			//SELECT COLOUR
			if (lastPressedKey == '1' && handlingLastPress) {
				handlingLastPress = 0;
				printf("What colour would you like? ");
				scanf("%f", &red);
				scanf("%f", &green);
				scanf("%f", &blue);
				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF SELECT COLOUR

			//CREATE LAYER
			else if (lastPressedKey == '2' && handlingLastPress) {
				handlingLastPress = 0;
				int layersRemain = -1;

				for (int counter = 0; counter < 10; counter++) {//check if we have any layers remaining, do so by seeing if any are un-named
					if (userLayerNames[counter].name == NULL) {
						layersRemain = counter;
						break;
					}
				}


				if (layersRemain >= 0) {//We have at least one layer remaining

					//NAMING THE LAYER
					printf("What would you like to name this layer?   ");
					char input[25];
					int current = 0;
					memset(input, '\0', 25);
					glfwSetKeyCallback(window, enterDetector);
					handlingLastPress = 0;//Clear this before we head in
					while (1) {//Keep looping here until we get an 'enter' press
						glfwPollEvents();
						if (handlingLastPress) {
							handlingLastPress = 0;
							input[current] = lastPressedKey;
							printf("%c", lastPressedKey);
							current++;							
						}
						if (enterDetection == 1) {
							enterDetection = 0;//We handled it
							break;//and enter is 'we are done here'
						}
					}
					printf("\n\n");
					glfwSetKeyCallback(window, NULL);
					userLayerNames[layersRemain].name = malloc(sizeof(char) * 25);
					strcpy(userLayerNames[layersRemain].name, input);
					//END OF NAMING THE LAYER




					printf("What height would you like to make it? 0 - 9.    ");

					while (1) {//Keep looping here until we get a number
						glfwPollEvents();
						if (handlingLastPress) {
							handlingLastPress = 0;
							userLayerNames[layersRemain].depth = (float) lastPressedKey - (float)'0';
							printf("%c\n", lastPressedKey);
							if (userLayerNames[layersRemain].depth != 0) {
								userLayerNames[layersRemain].depth = userLayerNames[layersRemain].depth/DEPTH_ADJUSTER;
							}
							break;
						}
					}
					



				}

				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF CREATE LAYER

			//SWITCH LAYER
			else if (lastPressedKey == '3' && handlingLastPress) {
				handlingLastPress = 0;//Handled
				int counter = 0;
				printf("What layer would you like to select?\n");
				while (userLayerNames[counter].name != NULL) {
					printf("%d) %s, depth %f\n", counter + 1, userLayerNames[counter].name, userLayerNames[counter].depth * DEPTH_ADJUSTER);
					counter++;
				}

				
				handlingLastPress = 0;//Make sure its clear
				while (1) {//Wait until we get a key
					glfwPollEvents();
					if (handlingLastPress == 1) {
						handlingLastPress = 0;//clear it
						//printf("%c", lastPressedKey);
						break;
					}
				}

				//We have the selected layer now (Hopefully) so we can select it
				
				currentLayer = (int) lastPressedKey - CHAR_NUM_TO_NUM;//Since we are selecting a layer, copy the key pressed (1 = 0, 2 = 1, etc) to the layer (0-9)
				printf("%d\n", currentLayer);
				printf("Selected Layer: %s\n\n", userLayerNames[currentLayer].name);

				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF SWITCHING LAYERS

			//SWITCH DRAW VERTICE MODE
			else if (lastPressedKey == '4' && handlingLastPress) {
				handlingLastPress = 0;
				printf("Vertex rendering has been flipped.\n\n");
				if (drawVertices == 1) {
					drawVertices = 0;
				} else {
					drawVertices = 1;
				}

				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF SWITCH DRAW VERTICE MODE




		}
		//END OF MENU

		


		//END OF MODES, ONTO UPDATES


		glEnable(GL_DEPTH_TEST);
		if (points > 0) {//Make sure we have stuff to draw...
			//Draw our new stuff
			//glPointSize(10.0f);
			glBindVertexArray(drawnshape.VAO);
			if (drawVertices == 1) {
				glDrawArrays(GL_POINTS, 0, points);
			}
			glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_INT, drawnshape.indices);
			glBindVertexArray(0);
			//End of drawing new stuff
		}





		//MODE SELECTOR CODE

		//create mode
		if (lastPressedKey == '1' && handlingLastPress){
			handlingLastPress = 0;//Dealt with keypress
			if(mode != VERT_CREATE){
				printf("\nMode: Vertex Creation\n\n");
				mode = VERT_CREATE;

				//cleanup variables
				selectedPoint = 0;
			}
		}//Vertex creation mode
		//move mode
		else if (lastPressedKey == '2' && handlingLastPress) {
			handlingLastPress = 0;//Dealt with keypress
			if (mode != VERT_CHANGE) {
				printf("\nMode: Vertex Changing\n\n");
				mode = VERT_CHANGE;

				//cleanup variables
				selectedPoint = 0;
			}
		} 
		//Vertex connect mode
		else if (lastPressedKey == '3' && handlingLastPress) {
			handlingLastPress = 0;//Dealt with keypress
			if (mode != VERT_CONNECT) {
				timeAtLastPrintf = time;
				printf("\nMode: Vertex Connection.\n\n");
				mode = VERT_CONNECT;

				//cleanup variables
				selectedPoint = 0;
			}

		} //vertex connect mode
		//vertex delete
		else if (lastPressedKey == '4' && handlingLastPress) {
			time = glfwGetTime();
			if (time - timeAtLastPrintf > TIME_BETWEEN_MODES) {
				timeAtLastPrintf = time;
				mode = VERT_COLOUR;
				printf("\nMode: Vertex Colour Changing.\n\n");

				//cleanup variables
				selectedPoint = 0;
			}
		}
		//Old style below for things that arent characters
		//menu
		else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//Escape 'brings up menu'
			time = glfwGetTime();
			if (time - timeAtLastPrintf > TIME_BETWEEN_MODES) {
				timeAtLastPrintf = time;
				printf("\nWhat would you like to do?\n1) Select colour.\n2) Create Layer\n3) Switch Layer\n4) Switch Vertex Rendering\n\n");
				mode = MENU;
				selectedPoint = 0;
			}
		}
		//end creation
		else if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
			time = glfwGetTime();
			if (time - timeAtLastPrintf > TIME_BETWEEN_MODES) {
				timeAtLastPrintf = time;
				mode = END_OF_CREATION;

				//cleanup variables
				selectedPoint = 0;
			}
		}
		
		//END OF MODE SELECTOR


		//GLOBAL VAR RESETS





		//UPDATE THE ENTIRE REST OF THE PROGRAM
		maindata.proecessinputelsewhere = 1;
		glfwPollEvents();
		maindata = mainLoop(maindata);//Run the main loop but without it processing input itself.
		//END OF UPDATING THE REST OF THE PROGRAM
		
	}
	
	//Done with shape creation, finalize all the data and return it to merge with The Data








	//Now done with creating all the vertices for the shape, so turn it into a shape object
	//struct ShapeData returnshape = createShape(vertices, indices, points * sizeof(float) * 6, (points + 2) * sizeof(unsigned int), 1);
	


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	printf("Done\n");
	return(drawnshape);


}