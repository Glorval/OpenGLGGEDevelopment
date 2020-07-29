#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "GlorvGE_BaseV1.h"

struct ShaderStrings getShaderStrings() {




	//Vertex Shader Loading
	FILE* vertSourceFile = fopen("GlorvGEV1/shaders/VectSource.shader", "r");
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
	FILE* fragSourceFile = fopen("GlorvGEV1/shaders/FragSource.shader", "r");
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
	window = glfwCreateWindow(800, 600, "OpenGLTests", NULL, NULL);
	glfwMakeContextCurrent(window);
	//end of making the window

	//Load openGL
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//


	//Set the viewport
	glViewport(0, 0, 800, 600);
	//
	setupShaders();
	printf("Time taken to setup within SetupEverything = %f\n", glfwGetTime());
	return(window);
}