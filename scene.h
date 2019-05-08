//--------------------------------------------------------------------------------------------
//
// File: 	scene.hpp
// Author:	P. Katarzynski (CCE)
//
// Description: Plik naglowkowy dla klasy OpenGL
//
//--------------------------------------------------------------------------------------------

#ifndef _scene_h

#define _scene_h

#include "common.h"
#include "globject.h"
#include "texture.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//----------------------------------- DEFINES -------------------------------------------------
#define ESCAPE 27

//--------------------------------------------------------------------------------------------
// 								CLASS INTERFACE
//--------------------------------------------------------------------------------------------
class Scene
{

public:

Scene(int new_width,int new_height); //  domyslny konstruktor
~Scene(); // domyslny destruktor 

void Draw(); // rysuje zawartosc sceny

void Resize(int new_width, int new_height); // zmienia rozmiar sceny 

void Init(); // procedura inicjujaca przetwarzanie

void Animate(); // przeprowadza animacje sceny 

void KeyPressed(unsigned char key, int x, int y); // obsluga klawiatury

GLuint LoadShader(GLenum type,const char *file_name); // laduje program shadera z zewnetrznego pliku 

void IncreaseAlpha(float additionalAlpha);  // przeźroczystość HUD

void ToggleWireframe();

void RuszMysz(int x, int y);

private:


glm::vec3 duchpos = glm::vec3(0.0, 0.0, 0.0);

float FogDensity = 1.0;
float rot_x; //  obrot obiektu wzgledem X
float rot_y; //  obrot obiektu wzgledem Y

float Scale; // skala odwzorowania obiektu 

float move;
float angleX;
float angleY;
float j;
int s;
float Alpha;

float katFurtki;

glm::vec3 position;
glm::vec3 direction;
glm::vec3 look;

glm::vec3 camPos;
glm::vec3 camLookAt;

glObject *Skybox;

GLuint tex_2d;

boolean wireframeOn;

std::vector< glObject* > Objects;
std::vector< glTexture* > Textures;
std::vector< glm::vec3 > LightPos;
std::vector< glm::vec3 > LightColor;

void PreparePrograms(); // przygotowuje programy przetwarzania 
void PrepareObjects(); // przygotowuje obiekty do wyswietlenia 
void PrepareLights();


// rozmiary sceny 
int width;
int height;

int prewx = 0;
int prewy = 0;

float camrotx=0, camroty=0;
float xwalk = 0, zwalk = 0, R = 0, WS=0.0, AD = 0.0, szybkosc = 2.0;


GLuint skyboxVAO, skyboxVBO;
GLuint cubemapTexture;

glm::mat4 mProjection;
glm::mat4 mModelView;

// zasoby programu przetwarzania
GLuint program;
GLuint vertex_shader;
GLuint fragment_shader;

// komunikaty diagnostyczne 
char _msg[1024];

bool chodzi; 

UINT SamplerId; // id samplera
};

#endif
//------------------------------- KONIEC PLIKU -----------------------------------------------
