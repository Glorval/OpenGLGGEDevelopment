#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GlorvGEV1/GlorvGE_BaseV1.h"




GLFWwindow* window;


//The main loop of the program but in function form wauw, needed it this way to not go INSANE when trying to register clicks
//Waiting on console stops the program and waits for input, loaded shapes is all shapes loaded currently, index count is how many indices to render, process input elsewhere disables the processing of input in here
//struct mainloopData mainLoop(struct mainloopData mainData);
int main()
{
   //SETUP

    window = NULL;
    struct Setupdata temp = setupEVERYTHING(window);
    window = temp.window;
    int shaderProgram = temp.shaderID;



    glEnable(GL_DEPTH_TEST);
    printf("Time taken to Final End of Setup %f\n", glfwGetTime());

    


    glClearColor(0.0, 0.1, 0.9, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
    glClearColor(0.0, 0.1, 0.9, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //END OF SETUP


    
    printf("Would you like to Load a shape to edit or make a New one?");
    char userInput = keyReader(window, 0);//get input dont display
    if (userInput == 'n') {
        drawShape(window, shaderProgram);//The drawshape saves within itself
    } else if (userInput == 'l') {
        char* filename = malloc(sizeof(char) * 25);
        typingLimited(window, 1, 1, filename, 25);
        struct ShapeData NewShape = createShapeFromFile(filename, 1);

        glBindVertexArray(NewShape.VAO);
        glDrawElements(GL_TRIANGLES, NewShape.indexcount, GL_UNSIGNED_INT, NewShape.indices);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        waitForKeyPress(window, 1);
    }



    glfwTerminate();
    return 0;
}