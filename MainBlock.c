#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GlorvGEV1/GlorvGE_BaseV1.h"

#define LENGTHOF(x)  (sizeof(x) / sizeof((x)[0]))
#define pi 3.14

struct mousedata {
    double xposOnClick;
    double yposOnClick;
};



void inputProc(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void mouseInteraction(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        xpos = 0;
        ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        printf("%f, %f\n", xpos, ypos);
    }
}





int main()
{
   //SETUP
    GLFWwindow* window = NULL;
    window = setupEVERYTHING(window);


    
    struct ShapeData shapeOne = createShapeFromFile("RectOne", "RectOne", 1);
    shapeOne.filename = calloc(50, sizeof(char));
    strcpy(&shapeOne.filename, "Test");
    saveShapeToFileStruct(shapeOne);
    printf("Time taken to Final End of Setup %f\n", glfwGetTime());





    glfwSetMouseButtonCallback(window, mouseInteraction);


    //END OF SETUP


    //float vertices[] = {
    //     0.25,  0.5, 0.0, 0.0, 1.0, 0.0, // top right
    //     0.5, -0.5, 0.0, 0.0, 1.0, 0.0,   // bottom right
    //    -0.5, -0.5, 0.0, 0.0, 1.0, 0.0,  // bottom left
    //    -0.5,  0.5, 0.0,  0.0, 1.0, 1.0  // top left 
    //};
    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    //CREATE OBJECT CODE
    //struct ShapeData shapeOne;
    //shapeOne = createShape(vertices, indices, sizeof(vertices), sizeof(indices));






    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        inputProc(window);//Process input

        //RENDERING

        //Clear previous

        glClearColor(0.5, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        //End of clearing


        //DRAW EVERYTHING

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        glBindVertexArray(shapeOne.VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //END OF DRAWING EVERYTHING









        glBindBuffer(GL_ARRAY_BUFFER, shapeOne.VBO);
        int position = 0;
        shapeOne.vertices[0] = shapeOne.vertices[0] + (sin(time+pi)*.01);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*position, sizeof(float), &shapeOne.vertices[0]);








        //END OF RENDERING



   

        glfwSwapBuffers(window);
        glfwPollEvents();

        //float finishtime = glfwGetTime();
        //printf("Time For Frame %f\n", finishtime - time);
    }











    //End
    glfwTerminate();
    return 0;
}