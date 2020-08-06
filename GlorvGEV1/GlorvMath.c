#pragma once
#include "GlorvMath.h"
#define WINDOW_X 1440
#define WINDOW_Y 1080


float distanceTwoD(float xOne, float xTwo, float yOne, float yTwo) {
	
	//return(powf(xOne - xTwo, 2) -powf(yOne - yTwo, 2));
	return(sqrt(((xOne - xTwo) * (xOne - xTwo)) + ((yOne - yTwo) * (yOne - yTwo))));
}


double distanceTwoDD(double xOne, double xTwo, double yOne, double yTwo) {

	//return(powf(xOne - xTwo, 2) -powf(yOne - yTwo, 2));
	return(sqrt(((xOne - xTwo) * (xOne - xTwo)) + ((yOne - yTwo) * (yOne - yTwo))));
}