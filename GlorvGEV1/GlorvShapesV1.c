#pragma once
#include "GlorvGE_BaseV1.h"
#define WINDOW_X 1440
#define WINDOW_Y 1080

#define IND_SIZE 1 * sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b
#define VERTEX_SIZE VERTEX_LENGTH * sizeof(float)//The size in bytes of a vertex, currently 6 because x/y/z r/g/b

#define EQFLOAT 0.0000001

#define DEPTH_ADJUSTER 2000000//How much to divide user input by when making 2d objects and layering

#define AdjustCharToArrayInt (int) '0' - 1

//Objects/
//Vertices.txt
//Indices.txt
//ObjData.txt


struct ShapeData createShape(float vertices[], unsigned int indices[], int vertSize, int indSize, int saveAll) {




	struct ShapeData Returns;
	glGenVertexArrays(1, &Returns.VAO);
	glGenBuffers(1, &Returns.VBO);
	glGenBuffers(1, &Returns.EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(Returns.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Returns.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * VERTEX_SIZE, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &Returns.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * IND_SIZE, indices, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (saveAll == 1) {
		Returns.fulldata = 1;
		//Returns.indices = malloc(sizeof(unsigned int) * indSize);
		Returns.indices = indices;
		//Returns.vertices = malloc(sizeof(float) * vertSize);
		Returns.vertices = vertices;
		Returns.vertexcount = vertSize;
		Returns.indexcount = indSize;
	}

	return(Returns);
}


struct ShapeData createShapeFromFile(char filename[], int saveAll) {
	char vertFileFixed[100] = "Objects/";
	char indFileFixed[100] = "Objects/";
	strcat(vertFileFixed, filename);
	strcat(vertFileFixed, "Vertices.txt");
	strcat(indFileFixed, filename);
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
	printf("Had to allocate %d bytes of memory for CreateShapeFromFile Vertices.\n", counter);
	int vertSize = counter / VERTEX_LENGTH;
	vertices = (float*)realloc(vertices, vertSize * sizeof(float) * VERTEX_LENGTH);
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
	int indSize = counter;
	indices = (unsigned int*)realloc(indices, indSize*sizeof(unsigned int));
	fclose(inds);



	struct ShapeData newshape = createShape(vertices, indices, vertSize, indSize, saveAll);
	return(newshape);
}


//Saves given shape to filen, shape given by raw data.
void saveRawShapeToFile(float vertices[], unsigned int indices, int vertSize, int indSize, char fileName) {
	//char* token;

	char vertFile[100] = "Objects/";//Get to the right directory
	strcat(vertFile, fileName);//Put the name of the file as the folder name
	strcat(vertFile, "Vertices.txt");//All vertice files are named this
	FILE* vertSaveFile = fopen(vertFile, "w");//The place being written to is OurProgram/Objects/FilenameVertices.txt
	fputs(vertices, vertFile);//Write the data to the file

	
	char indfile[100] = "Objects/";//Get to the right directory
	strcat(indfile, fileName);//Put the name of the file as the folder name
	strcat(indfile, "Indices.txt");//All indice files are named this
	FILE* indSaveFile = fopen(indfile, "w");//The place being written to is OurProgram/Objects/FilenameIndices.txt
	fputs(indices, indfile);//Write the data to the file
}

//Saves given shape to filen, shape given by raw data.
void saveShapeToFile(struct ShapeData savedata) {

	char vertFile[100] = "Objects/";//Get to the right directory
	strcat(vertFile, savedata.filename);//Put the name of the file as the folder name
	strcat(vertFile, "Vertices.txt");//All vertice files are named this
	FILE* vertSaveFile = fopen(vertFile, "w");//The place being written to is OurProgram/Objects/FilenameVertices.txt
	for (int counter = 0; counter < savedata.vertexcount; counter++) {
		fprintf(vertSaveFile, "%f", savedata.vertices[counter]);//FIX Make it print in sets of three
		if (counter != savedata.vertexcount - 1) {//Add a space so long as this isnt the last element
			fprintf(vertSaveFile, " ");
		}
	}
	fclose(vertSaveFile);


	char indfile[100] = "Objects/";//Get to the right directory
	strcat(indfile, savedata.filename);//Put the name of the file as the folder name
	strcat(indfile, "Indices.txt");//All indice files are named this
	FILE* indSaveFile = fopen(indfile, "w");//The place being written to is OurProgram/Objects/FilenameIndices.txt
	for (int counter = 0; counter < savedata.indexcount; counter++) {
		fprintf(indSaveFile, "%d", savedata.indices[counter]);
		if (counter != savedata.indexcount - 1) {//Add a space so long as this isnt the last element
			fprintf(indSaveFile, " ");
		}
	}
	fclose(indSaveFile);

	//TO DO: Get this to save layer data
	//char objdatafile[100] = "Objects/";
	//strcat(objdatafile, savedata.filename);//Put the name of the file as the folder name
	//strcat(objdatafile, ObjDataText);//All vertice files are named this
	//FILE* objdataSaveFile = fopen(objdatafile, "w");//The place being written to is OurProgram/Objects/FilenameObjData.txt
	////write the data to the file
	//fprintf(objdataSaveFile, "%d", savedata.)

}





//DRAWING SHAPE BY HAND

//Returns the closest  vertice to the mouse in terms of WHICH vertice, not the array value. So vertice 0, 1, 2 etc not position 0, 6, 12
int closestVert(struct ShapeData* givenshape, int points, GLFWwindow* window) {
	int closestPointNumber = 0;

	double* xy = calloc(2, sizeof(double));
	glfwGetCursorPos(window, &xy[0], &xy[1]);

	xy[0] = (xy[0] - (.5 * WINDOW_X)) / (.5 * WINDOW_X);
	xy[1] = -(xy[1] - (.5 * WINDOW_Y)) / (.5 * WINDOW_Y);

	float closestDistance = 100000;//Literally no way to get past this because ITS SUPPOSED TO BE -1 -> 1 CORDS

	for (int counter = 0; counter < points; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

		double currentDistance = distanceTwoDD(givenshape->vertices[counter * VERTEX_LENGTH], xy[0], givenshape->vertices[(counter * VERTEX_LENGTH) + 1], xy[1]);//Find the current distance
		if (currentDistance < closestDistance) {//If the current distance is so far the shortest, save it 
			closestDistance = currentDistance;
			closestPointNumber = counter;
		}

	}
	return(closestPointNumber);
}

//Deletes all indice references to given point
void deleteIndice(struct ShapeData* givenShape, int vert) {
	for (int currentThree = 0; currentThree < givenShape->indexcount; currentThree = currentThree + 3) {//Run through all indices 3 at a time
		if ((givenShape->indices[currentThree] == vert) || (givenShape->indices[currentThree + 1] == vert) || (givenShape->indices[currentThree + 2] == vert)) {//Do any of the three point to the removed vertex?
			if (currentThree != givenShape->indexcount - 3) {//Make sure that we aren't on the final three, if we are its just time to chop the memory off already
				for (int counter = currentThree; counter < givenShape->indexcount - 3; counter += 3) {//Shift everything back a trio overwriting the useless entry
					givenShape->indices[counter] = givenShape->indices[counter + 3];
					givenShape->indices[counter + 1] = givenShape->indices[counter + 4];
					givenShape->indices[counter + 2] = givenShape->indices[counter + 5];
				}
				currentThree = currentThree - 3;//Just back this up to make sure we dont skip any, because if it goes entry 1, 2, 3 and we remove 1 it becomes 2, 3, 3 with the last 3 being cutoff but we're now in POSITION 2, past 2
			}
			givenShape->indexcount = givenShape->indexcount - 3;//We have removed a trio of entries so keep track of it
		}
	}
	givenShape->indices = realloc(givenShape->indices, IND_SIZE * givenShape->indexcount);//Re-allocate the memory now that we're done shifting
}

//Deletes given vertice and all index entries pointing to it. Vert is in nonadjusted position, so not 0, 6, 12, its just in vertex 0, 1, 2, 3 etc
void deleteVertice(struct ShapeData* givenShape, int vert, int updateOpenGL) {
	for (int currentvert = vert * VERTEX_LENGTH; currentvert < givenShape->vertexcount * VERTEX_LENGTH; currentvert += VERTEX_LENGTH) {//Run through all vertices at and past the given one
		for (int swapsies = 0; swapsies < VERTEX_LENGTH; swapsies++) {//because then we run through and copy the next vertice into the current one, overwriting the one to be deleted
			givenShape->vertices[currentvert + swapsies] = givenShape->vertices[currentvert + swapsies + VERTEX_LENGTH];
		}
	}

	givenShape->vertices = realloc(givenShape->vertices, givenShape->vertexcount * VERTEX_SIZE);//'lower' the memory assigned to the array
	givenShape->vertexcount--;//we deleted a vertice, so keep track of it

	if (updateOpenGL) {//Do we update opengl from in here?
		glBufferData(GL_ARRAY_BUFFER, givenShape->vertexcount * VERTEX_SIZE, givenShape->vertices, GL_DYNAMIC_DRAW);//Reallocate all the memory used by opengl
	}


	for (int currentThree = 0; currentThree < givenShape->indexcount; currentThree = currentThree + 3) {//Run through all indices 3 at a time
		if ((givenShape->indices[currentThree] == vert) || (givenShape->indices[currentThree + 1] == vert) || (givenShape->indices[currentThree + 2] == vert)) {//Do any of the three point to the removed vertex?
			if (currentThree != givenShape->indexcount - 3) {//Make sure that we aren't on the final three, if we are its just time to chop the memory off already
				for (int counter = currentThree; counter < givenShape->indexcount - 3; counter += 3) {//Shift everything back a trio overwriting the useless entry
					givenShape->indices[counter] = givenShape->indices[counter + 3];
					givenShape->indices[counter + 1] = givenShape->indices[counter + 4];
					givenShape->indices[counter + 2] = givenShape->indices[counter + 5];
				}
				currentThree = currentThree - 3;//Just back this up to make sure we dont skip any, because if it goes entry 1, 2, 3 and we remove 1 it becomes 2, 3, 3 with the last 3 being cutoff but we're now in POSITION 2, past 2
			}
			givenShape->indexcount = givenShape->indexcount - 3;//We have removed a trio of entries so keep track of it
		}
	}
	givenShape->indices = realloc(givenShape->indices, IND_SIZE * givenShape->indexcount);//Re-allocate the memory now that we're done shifting



	for (int current = 0; current < givenShape->indexcount; current++) {//Run through all indices that remain and if any were referencing a vertex past the deleted one, lower the value they point to
		if (givenShape->indices[current] > vert) {//The reason for this is because if we have vertex 0, 1, 2 ,3 ,4 and remove 2, 3 becomes 2, 4 becomes 3 and we need the pointer to 4 to point to 3
			givenShape->indices[current] --;
		}
	}	

	
}


//Plan for how it will work: Get all the vertices while rendering them as points, then the user clicks on pairs of 3 vertices to connect them as a triangle.  
//Lastly the user should be able to click a vertex and then have it follow the mouse until they click again, and a way to switch between these 3 modes of 'vector creation' 'vector connection' and 'vector changing'
struct ShapeData drawShape(GLFWwindow* window, int shaderID) {
	printf("\nNow drawing a shape.\n\n\n");
	printf("\nMode: Vertex Creation\n\n");//default mode


	glfwSetCharCallback(window, character_callback);//For tippy typing
	glPointSize(10.0f);//For the vertex rendering

	//Variable stack

	//OPTIONS
		int drawVertices = 1;//whether to draw the endpoints as blocks
		int AutoSelectLayers = 0;//Whether to automatically select newly created layers
		int Confirmations = 1;//Whether to ask the user to confirm certain actions

	//END OF OPTIONS

	
	const double TIME_BETWEEN_MODES = .25;//The minimum time between switching modes.

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



	//USED FOR VERTEX MOVING
		int selectedPoint = 0;//Do we have a selected point
		int selectedPointPos = 0;//The position in the array of the selected point
		int closestPointNumber = 0;//Which one in the array is the closest point, points to the X value
	//END OF USED FOR VERTEX MOVING


	//SHAPEDATA
		struct ShapeData drawnshape;//The structure that we will return and render from
		drawnshape.vertices = malloc(sizeof(float));//Initalize each
		drawnshape.indices = malloc(sizeof(unsigned int));
		drawnshape.vertexcount = 0;
		drawnshape.indexcount = 0;
	//END OF SHAPEDATA

	glfwSetMouseButtonCallback(window, mouseclickCallback);//So when we click we can update the processing click global variable

	//modes
		const int VERT_CREATE = 1;
		const int VERT_CONNECT = 2;
		const int VERT_CHANGE = 3;
		const int VERT_COLOUR = 4;
		const int END_OF_CREATION = 0;
		const int MENU = -1;

		//SET DEFAULT MODE
		int mode = VERT_CREATE;//Set the mode to vertex creation so that we can actually start with making vertices
	//end of modes

	//VERTEX CONNECTION VARIABLES
		int tempIndices[3];//To temporarily store the indices until we have 3
		int indiceConnections = 0;//To indicate when we have 3 indices stored.
	//END OF VERTEX CONNECTION VARIABLES

	double time = 0;
	double timeAtLastPrintf = 0;//Used to make sure we dont spam the crap out of the 'Mode is: x' text
	//End of variable stack



	while (1) {//While we are still creating the shape, lets us loop between the 3 modes until we are finished.
		

		//START OF VERTEX CREATION
		if (mode == VERT_CREATE) {

			//IF WE HAVE A LEFT CLICK TO CREATE, SAVE ALL VERTICIES NO LONGER SAVES INDICES BECAUSE CONNECT MODE
			if (processingClick == 1) {//we have a click
				processingClick = 0;//reset as we are now dealing with it
				drawnshape.vertexcount++;

				double xpos, ypos;//We had a click so get the x/y
				glfwGetCursorPos(window, &xpos, &ypos);

				drawnshape.vertices = realloc(drawnshape.vertices, drawnshape.vertexcount * VERTEX_SIZE);//give more memory in prep of storing the x/y forever, *6 is because 6 entries per vertice

				//start to store all the new data, for now just make the shape red
					
				drawnshape.vertices[(drawnshape.vertexcount - 1) * 6] = (xpos - (.5 * WINDOW_X)) / (.5 * WINDOW_X);//x
				drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 1] = -(ypos - (WINDOW_Y / 2)) / (WINDOW_Y / 2);//y
				if (currentLayer == -1) {//No layer set, default to zero
					drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 2] = 0;//z
				} else {
					drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 2] = userLayerNames[currentLayer].depth;//z
				}
				drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 3] = red;//red
				drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 4] = green;//green
				drawnshape.vertices[((drawnshape.vertexcount - 1) * 6) + 5] = blue;//blue
				//end of storing data

			}


			//VERTEX DELETION
			else if (processingClick == 2) {//Right click deletes vertices
				processingClick = 0;//Reset the click


				int vertToDelete = closestVert(&drawnshape, drawnshape.vertexcount, window);//Find the closest vertice to the click
				deleteVertice(&drawnshape, vertToDelete, drawnshape.vertexcount, 1);
				
			//END OF VERTEX DELETION
			}
			//END OF CLICK REGISTERING




			//CREATE SHAPEDATA IF WE CREATE A VERTEX
			if (drawnshape.vertexcount == 1) {//If we have our first point, create the actual data in opengl for it
				drawnshape = createShape(drawnshape.vertices, drawnshape.indices, drawnshape.vertexcount, drawnshape.indexcount, 1);
			}
			//OTHERWISE UPDATE THE SHAPE
			else if(drawnshape.vertexcount > 1) {//BUG Potential, if you place one point and try to move it drawnshape wont have the updated info, MAYBE
				glBufferData(GL_ARRAY_BUFFER, drawnshape.vertexcount * VERTEX_SIZE, drawnshape.vertices, GL_DYNAMIC_DRAW);
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
						

					for (int counter = 0; counter < drawnshape.vertexcount; counter++) {//Run through each entry and find the distance storing the closest one and its position in the array.

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
			


			if (processingClick == 1) {//left click, find the closest point and its array value and save an index to it
				processingClick = 0;


				//FIND CLOSEST POINT TO CLICK
				closestPointNumber = closestVert(&drawnshape, drawnshape.vertexcount, window);
				
								
				//Now we have the closest point, so make a temp index out of it.
				//BUG CURRENTLY SAVES INDICES EVEN IF NOT  FULLY CREATED. NEEDS TO GATHER THREE THEN SAVE ALL THREE AT ONCE
				tempIndices[indiceConnections] = closestPointNumber;
				indiceConnections++;
				if (indiceConnections == 3) {
					indiceConnections = 0;
					drawnshape.indexcount += 3;//we're adding three indices
					drawnshape.indices = realloc(drawnshape.indices, sizeof(unsigned int) * drawnshape.indexcount);//Grab more memory to store em in
					drawnshape.indices[drawnshape.indexcount - 3] = tempIndices[0];//then store all three
					drawnshape.indices[drawnshape.indexcount - 2] = tempIndices[1];
					drawnshape.indices[drawnshape.indexcount - 1] = tempIndices[2];
				}
				
					
				

				} 
			else if (processingClick == 2) {//Right click. Disconnects all connections to the point
					printf("Are you sure you wish to disconnect this point?\n");
					if (confirmationDialog(window, "Disconnected.\n\n", "Cancled.\n\n")) {
						deleteIndice(&drawnshape, closestVert(&drawnshape, drawnshape.vertexcount, window));
					}
				}

		}
		//END OF VERTEX CONNECTION


		//VERT COLOUR CHANGING
		else if (mode == VERT_COLOUR) {
		int closestPointNumber = 0;//The closest vertex entry
			//Left click paints colour
			if (processingClick == 1) {//left click, find the closest point and switch its colour to the currently active colour
				processingClick = 0;

				//FIND CLOSEST POINT TO CLICK
				closestPointNumber = closestVert(&drawnshape, drawnshape.vertexcount, window);
				//End of tracking left click, we have a point  now


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
			} 
			//Right click copies the nearest colour
			else if (processingClick == 2) {
				processingClick = 0;


				//FIND CLOSEST POINT TO CLICK

				int vertToCopy = closestVert(&drawnshape, drawnshape.vertexcount, window);

			
				float tred = drawnshape.vertices[(vertToCopy * VERTEX_LENGTH) + 3];
				float tgreen = drawnshape.vertices[(vertToCopy * VERTEX_LENGTH) + 4];
				float tblue = drawnshape.vertices[(vertToCopy * VERTEX_LENGTH) + 5];
				printf("Copied colour: %0.0f, %0.0f, %0.0f.\n", tred * 255, tgreen * 255, tblue * 255);
				if (confirmationDialog(window, "Colour copied.\n\n", "Colour unchanged.\n\n")) {
					red = tred;
					green = tgreen;
					blue = tblue;
				}
				

			}//END OF RIGHT CLICK



		}
		//END OF VERT COLOUR


		//MENU TO SELECT STUFF
		else if (mode == MENU) {

			//SELECT COLOUR
			if (lastPressedKey == '1' && handlingLastPress) {
				handlingLastPress = 0;

				int twoFiftyFive = 0;//Declare our variables
				char numInput[3];

				if(extendedConfirmationDialog(window, "New colour: ","Colour unchanged.\n","Confirm colour change?\n")){

					//GET RED
					typingLimited(window, 1, 0, numInput, 3);
					printf("   ");//add some space

					//Now we have a string with numbers in it, convert to number numbers and store in red and BOOM, done.
							 //100s										10s									1s
					twoFiftyFive = (int) (((numInput[0] - '0') * 100) + ((numInput[1] - '0') * 10) + (numInput[2] - '0'));//Convert the three char string into an int
					red = (float) twoFiftyFive / 255;//Then convert the int to a float and BOOM
					//END OF GET RED
					
					//GET GREEN
					typingLimited(window, 1, 0, numInput, 3);
					//Now we have a string with numbers in it, convert to number numbers and store in red and BOOM, done.
												 //100s										10s									1s
					twoFiftyFive = (int)(((numInput[0] - '0') * 100) + ((numInput[1] - '0') * 10) + (numInput[2] - '0'));//Convert the three char string into an int
					green = (float)twoFiftyFive / 255;//Then convert the int to a float and BOOM
					//END OF GET GREEN

					printf("   ");//add some space
					
					//GET BLUE
					typingLimited(window, 1, 0, numInput, 3);
					//Now we have a string with numbers in it, convert to number numbers and store in red and BOOM, done.
													 //100s										10s									1s
					twoFiftyFive = (int)(((numInput[0] - '0') * 100) + ((numInput[1] - '0') * 10) + (numInput[2] - '0'));//Convert the three char string into an int
					blue = (float)twoFiftyFive / 255;//Then convert the int to a float and BOOM
					//END OF GET BLUE


				}//End of switching colour




				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF SELECT COLOUR


			//CREATE LAYER
			else if (lastPressedKey == '2' && handlingLastPress) {
				handlingLastPress = 0;
				int layersRemain = -1;

				for (int counter = 0; counter < 9; counter++) {//check if we have any layers remaining, do so by seeing if any are un-named
					if (userLayerNames[counter].name == NULL) {
						layersRemain = counter;
						break;
					}
				}


				if (layersRemain >= 0) {//We have at least one layer remaining

					//NAMING THE LAYER
					printf("What would you like to name this layer? 24 Character Max.   ");

					userLayerNames[layersRemain].name = malloc(sizeof(char) * 25);//Give it some memory
					typing(window, 1, 1, userLayerNames[layersRemain].name);//Get some typing input and save it to the name
					printf("\n\n");//add some spacing
					//END OF NAMING THE LAYER


					printf("What depth would you like to make it? 0 - 9.    ");

					while (1) {//while loop because we might get invalid depth from user
						int validdepth = 1;//variable to see if we got a valid depth

						userLayerNames[layersRemain].depth = (float)keyReader(window, 1) - '0';//Get the number the user inputs as a number rather than a character
						userLayerNames[layersRemain].depth = userLayerNames[layersRemain].depth / DEPTH_ADJUSTER;//then adjust the number to get it really smol so as to not look 3d

						for (int counter = 0; counter < layersRemain; counter++) {//Run through all the other layers and see if one is already on this height
							if (fabs(userLayerNames[layersRemain].depth - userLayerNames[counter].depth) < 0.000000000001) {//if any are on the height invalidate the run
								validdepth = 0; 
								printf("Not a valid layer height, already taken.\n");
								break;//and break out of the for loop to save time
								
							}
						}
						if (validdepth == 1) {
							break;
						}
					}
					

					



				} else {
					printf("No more layers remain.\n");
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

				waitForKeyPress(window, 0);

				//We have the selected layer now (Hopefully) so we can select it
				currentLayer = (int) lastPressedKey - AdjustCharToArrayInt;//Since we are selecting a layer, copy the key pressed (1 = 0, 2 = 1, etc) to the layer (0-9)
				printf("Selected Layer: %s\n\n", userLayerNames[currentLayer].name);

				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF SWITCHING LAYERS


			//RENAME LAYER
			else if (lastPressedKey == '4' && handlingLastPress) {
				handlingLastPress = 0;//Handled
				int counter = 0;

				printf("What layer would you like to select?\n");
				while (userLayerNames[counter].name != NULL) {//Print the layers out
					printf("%d) %s, depth %.0f\n", counter + 1, userLayerNames[counter].name, userLayerNames[counter].depth * DEPTH_ADJUSTER);
					counter++;
				}

				handlingLastPress = 0;//Make sure its clear
				waitForKeyPress(window, 0);
				//Now we have a number keypress to select it

				currentLayer = (int)lastPressedKey - AdjustCharToArrayInt;//Since we are selecting a layer, copy the key pressed (1 = 0, 2 = 1, etc) to the layer (0-9)
				printf("%d\n", currentLayer);
				printf("Selected Layer: %s\n\n", userLayerNames[currentLayer].name);

				
				//RE-NAMING THE LAYER
				printf("What would you like to re-name this layer to?   ");
				typing(window, 1, 1, userLayerNames[currentLayer].name);
				handlingLastPress = 0;//Handled
				//END OF RE-NAMING THE LAYER



				//DO NOT re-height the layer

				mode = VERT_CREATE;//Back out of menu now that we are done here.
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF RENAME LAYER


			//DELETE LAYER
			else if (lastPressedKey == '5' && handlingLastPress) {
				handlingLastPress = 0;//Handled
				int counter = 0;

				if(extendedConfirmationDialog(window, "What layer would you like to delete?\n", "\n", "Are you sure  you wish to delete a layer?\n")){//Confirm that the user wishes to delete a layer

					while (userLayerNames[counter].name != NULL) {//Print the layers out
						printf("%d) %s, depth %.0f\n", counter + 1, userLayerNames[counter].name, userLayerNames[counter].depth * DEPTH_ADJUSTER);
						counter++;
					}

					waitForKeyPress(window, 1);
				
					//Now we have a number keypress to select it then delete it
					int currentlayer = lastPressedKey - AdjustCharToArrayInt;
					float layerheight = userLayerNames[currentlayer].depth;//The z value we need to remove points on

					for (int current = 0; current < drawnshape.vertexcount; current++) {//Run through each vertice and see if we need to delete it

						if (fabs(drawnshape.vertices[(current * VERTEX_LENGTH) + 2] - layerheight) < 0.0000001) {//Is this vertice on the correct z value to be on this layer?
							//if so we need to delete it, but then also pull back current so as to not skip any vertices (Because if we delete it all vertices get shifted back 1
							deleteVertice(&drawnshape, current, 0);
							current--;
						}

					}

					//Since we aren't reallocating in the delete vertice, as that would potentially be a crapton of updates, update now. 	
					glBufferData(GL_ARRAY_BUFFER, drawnshape.vertexcount * VERTEX_SIZE, drawnshape.vertices, GL_DYNAMIC_DRAW);
				
					counter = currentlayer;//Start at the given layer
					while (userLayerNames[counter].name != NULL) {//Overwrite the current layer and then delete the last
						if(userLayerNames[counter+1].name != NULL){
							userLayerNames[counter].name = userLayerNames[counter + 1].name;
							userLayerNames[counter].depth = userLayerNames[counter + 1].depth;
						} else {
							userLayerNames[counter].name = NULL;
							userLayerNames[counter].depth = -10;
						}
						counter++;
					}
				}

			}
			//END OF DELETE LAYER

			//SWITCH DRAW VERTICE MODE
			else if (lastPressedKey == '6' && handlingLastPress) {
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

			//OPTIONS
			else if (lastPressedKey == '7' && handlingLastPress) {
				handlingLastPress == 0;//As always reset this
				mode = VERT_CREATE;//This is a oneoff lock in path that shouldnt be looped through

				printf("1) Layers select upon creation. Currently %d\n2) Confirmations required to perform certain actions. Currently %d\n", AutoSelectLayers, Confirmations);

				char action = keyReader(window, 0);
				if (action == '1') {
					AutoSelectLayers = !AutoSelectLayers;
				} else if (action == '2') {
					Confirmations = !Confirmations;
				}
				handlingLastPress = 0;//extra safety
				

				
				printf("\nMode: Vertex Creation\n\n");
			}
			//END OF OPTIONS

		}
		//END OF MENU

		//S9 E1
		else if (mode == END_OF_CREATION) {
			printf("Are you sure you would like to end the shape creation? Y/N ");
			char yesno = keyReader(window, 0);
			if (yesno == 'y') {//We are ending the shape creation
				drawnshape.ShapeLayers = userLayerNames;//Copy the layer data over
				printf("\nWhat would you like to name the object? Max 50 characters. ");
				drawnshape.filename = malloc(sizeof(char)*51);
				typing(window, 1, 1, drawnshape.filename);
				saveShapeToFile(drawnshape);//Save it
			} else {
				printf("\nMode: Vertex Creation\n\n");
				mode == VERT_CREATE;
			}
		}
		//S9 E24


		//END OF MODES, ONTO UPDATES


		glEnable(GL_DEPTH_TEST);
		if (drawnshape.vertexcount > 0) {//Make sure we have stuff to draw...
			//Draw our new stuff
			//glPointSize(10.0f);
			glBindVertexArray(drawnshape.VAO);
			if (drawVertices == 1) {
				glDrawArrays(GL_POINTS, 0, drawnshape.vertexcount);
			}
			glDrawElements(GL_TRIANGLES, drawnshape.indexcount, GL_UNSIGNED_INT, drawnshape.indices);
			glBindVertexArray(0);
			//glfwSwapBuffers(window);
			//End of drawing new stuff
		}
		
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClearColor(0.0, 0.1, 0.9, 0.5);//clear the frame that was shown and just got pulled back
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		//MODE SELECTOR CODE
		if (mode != MENU) {
			//create/delete mode
			if (lastPressedKey == '1' && handlingLastPress) {
				handlingLastPress = 0;//Dealt with keypress
				if (mode != VERT_CREATE) {
					printf("\nMode: Vertex Creation\n\n");
					mode = VERT_CREATE;

					//cleanup variables
					selectedPoint = 0;
					indiceConnections = 0;
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
					indiceConnections = 0;
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
					indiceConnections = 0;
				}

			} //vertex connect mode

			//vertex colour change
			else if (lastPressedKey == '4' && handlingLastPress) {
				handlingLastPress = 0;
				time = glfwGetTime();
				if (time - timeAtLastPrintf > TIME_BETWEEN_MODES) {
					timeAtLastPrintf = time;
					mode = VERT_COLOUR;
					printf("\nMode: Vertex Colour Changing.\n\n");

					//cleanup variables
					selectedPoint = 0;
					indiceConnections = 0;
				}
			}

			//Old style below for things that arent characters
			//menu
			else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//Escape 'brings up menu'
				time = glfwGetTime();
				if (time - lastSpecialPressTime > TIME_BETWEEN_MODES) {
					if (mode == MENU) {
						mode = VERT_CREATE;
						printf("\nMode: Vertex Creation\n\n");

						selectedPoint = 0;
						indiceConnections = 0;
					} else {
						lastSpecialPressTime = time;
						printf("\nWhat would you like to do?\n1) Select colour.\n2) Create Layer\n3) Switch Layer\n4) Edit Layer\n5) Delete Layer\n6) Switch Vertex Rendering\n7) Options\n\n");
						mode = MENU;

						selectedPoint = 0;
						indiceConnections = 0;
					}
					
				}
			}
			//end creation
			else if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
				time = glfwGetTime();
				if (time - lastSpecialPressTime > TIME_BETWEEN_MODES) {
					lastSpecialPressTime = time;
					mode = END_OF_CREATION;

					//cleanup variables
					selectedPoint = 0;
					indiceConnections = 0;
				}
			}

		}
		
		
		//END OF MODE SELECTOR


		

		
	}
	

}