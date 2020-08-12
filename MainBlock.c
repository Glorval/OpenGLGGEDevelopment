#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GlorvGEV1/GlorvGE_BaseV1.h"


//#define LENGTHOF(x)  (sizeof(x) / sizeof((x)[0]))
#define pi 3.14






struct consoleReturns {
    struct ShapeData consoleShape;
    int returningShape;
};



//Used to send input data
struct inputReturns {
    int success;
    struct MouseData mouseInput;
};








GLFWwindow* window;






struct inputReturns inputProc() {
    struct inputReturns returnData;
    returnData.success = 1;
    /*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }*/
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
        glfwGetCursorPos(window, &returnData.mouseInput.xposOnClick, &returnData.mouseInput.yposOnClick);
        printf("click\n");
        //returnData.mouseInput.processingClick;
    }

    return(returnData);
}





struct consoleReturns consoleInput(char input[100], struct mainloopData* alldata) {//Accepts user input and goes down a tree to find out what the hell we're doing
    struct consoleReturns Returns;
    if (strstr(input, "create") != NULL) {//Create tree


        if (strstr(input, "shape")) {//Creating a shape
            Returns.consoleShape = drawShape(window, *alldata);
            Returns.returningShape = 1;
        }


        //end of create tree

    }
    else {
        Returns.returningShape = 0;
    }



    return(Returns);
}


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

    
    /*struct ShapeData shapeOne = createShapeFromFile("RectOne", "RectOne", 1);
    shapeOne.filename = calloc(50, sizeof(char));
    strcpy(&shapeOne.filename, "Test");
    saveShapeToFileStruct(shapeOne);*/


    glEnable(GL_DEPTH_TEST);
    printf("Time taken to Final End of Setup %f\n", glfwGetTime());


    Vector4 veca = {1.0f, 1.0f, 1.0f, 1.0f};



   

    
   

    //END OF SETUP
    
    struct mainloopData alldata;
    alldata.AllLoadedShapes = malloc(sizeof(struct ShapeData));
    alldata.sizeofshapes = 0;
    alldata.proecessinputelsewhere = 0;
    alldata.totalindices = 0;
    alldata.totalshapes = 0;
    alldata.waitingonconsole = 0;
    glPointSize(10.0f);
    //DEBUG STUFF
    alldata.AllLoadedShapes = realloc(alldata.AllLoadedShapes, 1000);
    alldata.AllLoadedShapes[0] = createShapeFromFile("RectOne",1);
    alldata.totalshapes = 1;
    glUseProgram(shaderProgram);
    //glGetUniformfv(shaderProgram, "offset", &offsetlocation);
    int offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    float offsets[] = {0.5, 0.0, 0.0, 0.0};
    glUniform4fv(offsetLocation, 1, offsets);


    //END OF DEBUG STUFF
    while (!glfwWindowShouldClose(window)) {
        alldata = mainLoop(alldata);
        if (alldata.ending == 1) {
            break;
        }
        alldata.waitingonconsole = 0;
    }

    glfwTerminate();
    return 0;
}








struct mainloopData mainLoop(struct mainloopData mainData) {
    //check to see if we need to close
    if (glfwWindowShouldClose(window)) {
        glfwTerminate();
        struct mainloopData ending;
        ending.ending = 1;
        return(ending);
    }
    float timeAtStart = glfwGetTime();
    if (mainData.proecessinputelsewhere == 0) {//Process current input IF WE ARE ALLOWED
        inputProc(window);
        glfwPollEvents();
    }

   
    
    


   /* glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/

        
    //GL_TRIANGLE_FAN

    for(int counter = 0; counter < mainData.totalshapes; counter ++){


        //glUseProgram(shaderProgram);
        

        glBindVertexArray(mainData.AllLoadedShapes[counter].VAO);
        glDrawElements(GL_TRIANGLES, mainData.AllLoadedShapes[0].indexcount, GL_UNSIGNED_INT, mainData.AllLoadedShapes[counter].indices);
        glDrawArrays(GL_POINTS, 0, mainData.AllLoadedShapes[counter].vertexcount);


    }



    //glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, mainData.totalindices);
    glfwSwapBuffers(window);


    glClearColor(0.0, 0.1, 0.9, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    char userInput[100];
    //memset(userInput, "\0", 100);
    if (mainData.waitingonconsole == 1) {
        mainData.waitingonconsole = 0;
        gets(userInput);
        struct consoleReturns consoledata = consoleInput(userInput, &mainData);
        if (consoledata.returningShape == 1) {
            mainData.sizeofshapes += consoledata.consoleShape.vertexcount + consoledata.consoleShape.indexcount + sizeof(struct ShapeData);
            mainData.totalshapes++;
            mainData.AllLoadedShapes = realloc(mainData.AllLoadedShapes, mainData.sizeofshapes);
            mainData.AllLoadedShapes[mainData.totalshapes - 1] = consoledata.consoleShape;
            mainData.totalindices = consoledata.consoleShape.indexcount/sizeof(unsigned int);
        }
    }






    //printf("Time for frame: %f\n", glfwGetTime() - timeAtStart);
    return(mainData);
}


