//------------------------------- PROJECT SETTINGS --------------------------------------------
#ifndef _common_h
#define _common_h
#define IS_WIN32 1

#define PROJECT_NAME "lab2"

#define INITIAL_WIDTH 1200

#define INITIAL_HEIGHT 720

// disable warnings concerning deprecation of string operations (Visual Studio)
#define _CRT_SECURE_NO_WARNINGS

//------------------------------- DEPENDENCIES -----------------------------------------------

#include <windows.h>

#include "glew.h"
#include "wglew.h"
#include "glm\gtx\rotate_vector.hpp"

#pragma comment(lib,"glew32.lib")	
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"opengl32.lib")	
#pragma comment(lib,"SOIL.lib")

#include <stdio.h>
#include <math.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <memory>
#include "SOIL.h"


//------------------------------- ROUTINES -----------------------------------------------
void PrintLog(char *message);
void ThrowException(char *msg);

#endif