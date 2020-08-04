#pragma once
#include "GlorvMath.h"

float distanceTwoD(float xOne, float xTwo, float yOne, float yTwo) {
	
	//return(powf(xOne - xTwo, 2) -powf(yOne - yTwo, 2));
	return(sqrt(((xOne - xTwo) * (xOne - xTwo)) + ((yOne - yTwo) * (yOne - yTwo))));
}