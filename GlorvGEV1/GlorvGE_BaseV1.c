#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "GlorvGE_BaseV1.h"

extern int Window_X = 1440;
extern int Window_Y = 1080;











struct ShaderStrings getShaderStrings() {




	//Vertex Shader Loading
	FILE* vertSourceFile = fopen("GlorvGEV1/shaders/VectSource.glsl", "r");
	char* vectSource;
	vectSource = (char*)calloc(100, sizeof(char));

	int length = 0;
	int freespaces = 100;
	while (1) {
		char passalong[100];
		if (fgets(passalong, 100, vertSourceFile) == NULL) {//Read entry from file
			break;//If end of file stop reading
		}
		
		strcat(vectSource, passalong);
		length = length + strlen(passalong);
		freespaces = freespaces - length;//Keep track of how many spaces we have left
		if (freespaces < 100) {//No more spaces = ADD MOAR SPACE
			freespaces = 100;
			vectSource = (char*)realloc(vectSource, (100 + length) * sizeof(char));//Hush, nobody cares if it fails
		}
	}

	printf("Had to allocate %d bytes of memory for Vector Shader Loading.\n", length);
	int lengthOfShader = length * sizeof(char);
	vectSource = (float*)realloc(vectSource, lengthOfShader + 1);//+1 for the end of file
	fclose(vertSourceFile);





	//Fragment Shader Loading
	FILE* fragSourceFile = fopen("GlorvGEV1/shaders/FragSource.glsl", "r");
	char* fragSource;
	fragSource = (char*)calloc(100, sizeof(char));

	length = 0;
	freespaces = 100;
	while (1) {
		char passalong[100];
		if (fgets(passalong, 100, fragSourceFile) == NULL) {//Read entry from file
			break;//If end of file stop reading
		}

		strcat(fragSource, passalong);
		length = length + strlen(passalong);
		freespaces = freespaces - length;//Keep track of how many spaces we have left
		if (freespaces < 100) {//No more spaces = ADD MOAR SPACE
			freespaces = 100;
			fragSource = (char*)realloc(fragSource, (100 + length) * sizeof(char));//Hush, nobody cares if it fails
		}
	}

	printf("Had to allocate %d bytes of memory for Fragment Shader Loading.\n", length);
	lengthOfShader = length * sizeof(char);
	fragSource = (float*)realloc(fragSource, lengthOfShader + 1);//+1 for the end of file
	fclose(fragSourceFile);







	struct ShaderStrings returnStrings;
	returnStrings.vertexS = vectSource;
	returnStrings.fragS = fragSource;
	return (returnStrings);
}


void setupShaders() {
	struct ShaderStrings shaderStrings = getShaderStrings();
	//Vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderStrings.vertexS, NULL);
	glCompileShader(vertexShader);

	//Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderStrings.fragS, NULL);
	glCompileShader(fragmentShader);

	//Link the two shaders
	//unsigned int shaderProgram;
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//clear the unhooked shaders to free memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//setup openGLs ability to read inputs

	glUseProgram(shaderProgram);
}





GLFWwindow* setupEVERYTHING(GLFWwindow* window) {
	float SetupTime = 0;
	//Load GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//


	//Setup opengl
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//end of setup


	//make a window
	window = glfwCreateWindow(Window_X, Window_Y, "OpenGLTests", NULL, NULL);
	glfwMakeContextCurrent(window);
	//end of making the window

	//Load openGL
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//


	//Set the viewport
	glViewport(0, 0, Window_X, Window_Y);
	//
	setupShaders();


	//Initialization of Global Variables

	lastPressedKey = 0;
	lastPressedTime = 0;
	handlingLastPress = 0;
	enterDetection = 0;
	//End of Initialization of Global Variables







	printf("Time taken to setup within SetupEverything = %f\n", glfwGetTime());
	return(window);
}



//USER INPUT BLOCK

//Is used to detect a keypress, also saves the pressed key to a varaible for later use
void character_callback(GLFWwindow* window, unsigned int codepoint) {
	if (lastPressedTime < glfwGetTime() - .05) {
		handlingLastPress = 1;
		lastPressedKey = codepoint;
		lastPressedTime = glfwGetTime();
	}
}


//Is used to detect when the enter key is hit, useful for end of typing
void enterDetector(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ENTER) {
		enterDetection = 1;
	}
}


char keyReader(GLFWwindow* window, int display) {
	glfwSetKeyCallback(window, enterDetector);
	handlingLastPress = 0;//Clear this before we head in
	while (1) {//Keep looping here until we get a keypress, then report it
		glfwPollEvents();
		if (handlingLastPress) {
			handlingLastPress = 0;
			if (display) {
				printf("%c", lastPressedKey);
			}
			glfwSetKeyCallback(window, NULL);//We're done so reset the callback
			return(lastPressedKey);
		}
		if (enterDetection) {//Enter means we're done here
			enterDetection = 0;
			glfwSetKeyCallback(window, NULL);//We're done so reset the callback
			return('\0');
		}
	}
	
}