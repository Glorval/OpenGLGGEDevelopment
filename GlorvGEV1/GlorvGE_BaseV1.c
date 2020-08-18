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


int setupShaders() {
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
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//clear the unhooked shaders to free memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//setup openGLs ability to read inputs

	glUseProgram(shaderProgram);
	return(shaderProgram);
}





struct Setupdata setupEVERYTHING(GLFWwindow* window) {
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

	struct Setupdata returndata;
	returndata.shaderID = setupShaders();
	returndata.window = window;

	//Initialization of Global Variables
	processingClick = 0;
	lastPressedKey = 0;
	lastPressedTime = 0;
	lastSpecialPressTime = 0;
	handlingLastPress = 0;
	specialPress = 0;
	//End of Initialization of Global Variables






	
	
	printf("Time taken to setup within SetupEverything = %f\n", glfwGetTime());
	return(returndata);
}



//USER INPUT BLOCK



//Sets global vars for left/right click
//int processingClick = 0;//Sets to 1 for left click, sets to 2 for right click. ALWAYS SET TO ZERO IF YOURE GOING TO CHECK IT
void mouseclickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		processingClick = 1;
		//glfwGetCursorPos(window, &mouseX, &mouseY);
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//glfwGetCursorPos(window, &mouseX, &mouseY);
		processingClick = 2;
	}
}



//Is used to detect a keypress, also saves the pressed key to a varaible for later use
void character_callback(GLFWwindow* window, unsigned int codepoint) {
	if (lastPressedTime < glfwGetTime() - .05) {
		handlingLastPress = 1;
		lastPressedKey = codepoint;
		lastPressedTime = glfwGetTime();
	}
}


//Is used to detect when the enter key is hit, useful for end of typing. Also detects backspace
void specialKeyDetector(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ENTER) {
		if (lastSpecialPressTime < glfwGetTime() - .2) {//prevents enter spamming between calls
			lastSpecialPressTime = glfwGetTime();
			specialPress = 1;
		}
	} else if (key == GLFW_KEY_BACKSPACE) {
		if (lastSpecialPressTime < glfwGetTime() - .2) {//prevents enter spamming between calls
			lastSpecialPressTime = glfwGetTime();
			specialPress = 2;
		}
	} else if (key == GLFW_KEY_ESCAPE) {
		if (lastSpecialPressTime < glfwGetTime() - .2) {//prevents enter spamming between calls
			lastSpecialPressTime = glfwGetTime();
			specialPress = 3;
		}
	} else if (key == GLFW_KEY_END) {
		if (lastSpecialPressTime < glfwGetTime() - .2) {//prevents enter spamming between calls
			lastSpecialPressTime = glfwGetTime();
			specialPress = 4;
		}
	}
}





void waitForKeyPress(GLFWwindow* window, int display) {
	handlingLastPress = 0;
	while (1) {
		glfwPollEvents();
		if (handlingLastPress) {
			if (display) {
				printf("%c", lastPressedKey);
			}
			break;
		}
	}
}




//Warning: Will stay in function until keypress. DO NOT USE FOR ANYTHING BUT OTHER FUNCTIONS AND DIRECT COMMANDS, NO BACKGROUND COMMANDS
//Used as a singluar function to return individual pressed keys until enter is hit, display is whether or not to print to console as typing.
//'\0' returned on enter, 2 returned on backspace
char keyReader(GLFWwindow* window, int display) {
	glfwSetCharCallback(window, character_callback);//For tippy typing registration
	glfwSetKeyCallback(window, specialKeyDetector);
	glfwPollEvents();//Do this to trigger any remnants of enter pressing
	specialPress = 0;//Clear this before we head in
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
		if (specialPress == 1) {//Enter means we're done here
			specialPress = 0;
			glfwSetKeyCallback(window, NULL);//We're done so reset the callback
			return('\0');
		} else if (specialPress == 2) {//Backspace usually means to delete the latest entry.
			return(2);
		}
	}
	
}

//used to get a 1 for yes and 0 for no with text all in one function
int confirmationDialog(GLFWwindow* window, char* confirmedMessage, char* failedMessage) {
	printf("Right click to confirm, esc to cancel.\n");
	glfwSetKeyCallback(window, specialKeyDetector);
	processingClick = 0;
	while (1) {
		glfwPollEvents();
		if (processingClick == 2) {
			processingClick = 0;
			printf("%s", confirmedMessage);
			glfwSetKeyCallback(window, NULL);
			return(1);
		} else if (specialPress == 2 || specialPress == 3) {
			specialPress = 0;
			glfwSetKeyCallback(window, NULL);
			printf("%s", failedMessage);
			return(0);
		}
	}
}

int extendedConfirmationDialog(GLFWwindow* window, char* confirmedMessage, char* failedMessage, char* initialMessage) {
	printf("%s", initialMessage);
	glfwSetKeyCallback(window, specialKeyDetector);
	glfwPollEvents();
	specialPress = 0;
	processingClick = 0;
	while (1) {
		glfwPollEvents();
		if (processingClick == 2) {
			processingClick = 0;
			printf("%s", confirmedMessage);
			glfwSetKeyCallback(window, NULL);
			return(1);
		} else if (specialPress == 2 || specialPress == 3) {
			specialPress = 0;
			glfwSetKeyCallback(window, NULL);
			printf("%s", failedMessage);
			return(0);
		}
	}
}

//Warning, will stay in function until enter is hit
//Used to get typing input reliably, display displays it, nullterm keeps the \0 at the end of the string.
void typing(GLFWwindow* window, int display, int nullterm, char* string) {
	glfwPollEvents();
	char typedchar;
	unsigned int current = 0;//keep track of where we are in our array
	handlingLastPress = 0;//Clear this just in case
	while (1) {//While we dont have an enter press loop again and again
		typedchar = keyReader(window, display);//get the next character
		if (typedchar == '\0') {//if they hit enter it returns null term.
			if (nullterm) {//We want the null term so copy this last character
				string[current] = typedchar;
				break;//then once we save it break out of the loop because we're done here
			} else {//We dont want the null term so break before saving
				break;
			}
		} else if (typedchar == 2) {//backspace, delete the last character
			typedchar = 0;
			current--;//Back up one
			if (nullterm) {//If we want a nullterm to finish the string theres likely no problem in deleting a char with	this
				string[current] = '\0';
			} else {//If we dont want a nullterm then just set outright to null
				string[current] = NULL;
			}

			if (display) {//if we want to display then we gotta do that real quick, so backspace and overwrite with blank temporarily, then backspace again to be over the blank space
				printf("\b \b");
			}

		} else {//No backspace or enter, so just copy the data over
			string[current] = typedchar;//copy the data over
			current++;//keep track of where we are
		}
		//returnText = realloc(returnText, sizeof(char) * current);//Give more memory
	}
}


//Will stay in function until enter or the max limit has been hit, limit being the max number of characters in the string, beware of truncation if using nullterm
//Used to get typing input reliably, display displays it, nullterm keeps the \0 at the end of the string.
void typingLimited(GLFWwindow* window, int display, int nullterm, char* string, int limit) {
	glfwPollEvents();
	char typedchar;
	unsigned int current = 0;//keep track of where we are in our array
	handlingLastPress = 0;//Clear this just in case
	while (1) {//While we dont have an enter press loop again and again
		typedchar = keyReader(window, display);//get the next character
		if (typedchar == '\0') {//if they hit enter it returns null term.
			if (nullterm) {//We want the null term so copy this last character
				string[current] = typedchar;
				break;//then once we save it break out of the loop because we're done here
			} else {//We dont want the null term so break before saving
				break;
			}
		} else if (typedchar == 2) {//backspace, delete the last character
			typedchar = 0;
			if (current > 0) {//This is to check for INSTANT backspace
				current--;//Back up one
			}
			if (nullterm) {//If we want a nullterm to finish the string theres likely no problem in deleting a char with	this
				string[current] = '\0';
			} else {//If we dont want a nullterm then just set outright to null
				string[current] = NULL;
			}

			if (display) {//if we want to display then we gotta do that real quick, so backspace and overwrite with blank temporarily, then backspace again to be over the blank space
				printf("\b \b");
			}

		} else {//No backspace or enter, so just copy the data over
			string[current] = typedchar;//copy the data over
			current++;//keep track of where we are
		}

		if (nullterm) {//The limited portion, if we want a nullterm check and see if we are one off the last char that we want in the string
			if (current == limit - 1) {//if we are, fill it with nullterm and then end
				string[current] = '\0';
				return;
			}
		} else if(current == limit){//Otherwise, are we at the limit?
			return;//if so end
		}


	}//Effective end of function
}