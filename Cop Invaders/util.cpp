#include "util.h"

float rand0to1()
{
	return rand() / (float)RAND_MAX;
} 

float rand0to12()
{
	return rand() % 13;
}