#ifndef texture_H
#define texture_H

#include "common.h"

// klasa implementujaca obsluge obiektu tekstury w OpenGL
class glTexture
{

public:

glTexture(char *FileName); // domyslny konstruktor 
glTexture(const char *FileName, std::string newName); 
glTexture(std::vector<const GLchar*> faces);
~glTexture(); // domyslny destruktor
GLuint Bind(); // ustawia aktualny obiekt tekstury 

void SetName(std::string newName);
std::string GetName();

private: 
void LoadBMP(char *filename,int &_width, int &_height, char **pixels);
UINT TextureId; // id tekstury 
UINT SamplerId; // id samplera

std::string Name;

char _msg[1024];
};


#endif