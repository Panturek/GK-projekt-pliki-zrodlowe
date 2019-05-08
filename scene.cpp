//--------------------------------------------------------------------------------------------
//
// File: 	scene.cpp
// Author:	P. Katarzynski (CCE)
//
// Description: Implementacja klasy sceny OpenGL
//
//--------------------------------------------------------------------------------------------
//									ZALEZNOSI 
//--------------------------------------------------------------------------------------------

#include "scene.h"

//--------------------------------------------------------------------------------------------
// zglasza wyjatek z komunikatem do debuggowania 

//--------------------------------------------------------------------------------------------
Scene::Scene(int new_width,int new_height)
{	
	width = new_width;
	height = new_height;	
	wireframeOn = false;

	chodzi = false;
	rot_x = 0.0;
	rot_y = 0.0;
	Scale = 1.0;	
	Alpha = 0.75f;

	katFurtki = 0.5f;

	position = glm::vec3(0.0f, 5.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	look = position + direction;

	camPos = glm::vec3(9.30f, 10.50f, 9.30f); 
	camLookAt = glm::vec3(0.0f, 3.0f, 0.0f);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE_ARB);
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	// usun program przetwarzania 
	if (glIsProgram(program)) glDeleteProgram(program);

}
//--------------------------------------------------------------------------------------------
// przygotowuje programy cienionwania 
void Scene::PreparePrograms()
{
	program  = glCreateProgram();
	if (!glIsProgram(program)) ThrowException("Nie udalo sie utworzyc programu");
	
	vertex_shader = LoadShader(GL_VERTEX_SHADER,"vs.glsl");
	glAttachShader(program,vertex_shader);

	fragment_shader = LoadShader(GL_FRAGMENT_SHADER,"fs.glsl");
	glAttachShader(program,fragment_shader);
	
	// linkowanie programu 
	glLinkProgram(program);

	GLint link_status; 
    glGetProgramiv( program, GL_LINK_STATUS, &link_status); 
    if( link_status == GL_FALSE ) 
    { 
           GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
		ThrowException("Blad linkowania programu");
    }
	else
		PrintLog("Program zlinkowany");

	// walidowanie programu 
	glValidateProgram(program); 
	GLint validate_status;
    // sprawdzenie poprawno彡i walidacji obiektu programu 
    glGetProgramiv( program, GL_VALIDATE_STATUS, &validate_status ); 
    if( validate_status == GL_FALSE ) 
    { 
        GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
        ThrowException("Blad walidacji programu");
    } 
	else
		PrintLog("Program prawidlowy");
	
	glUseProgram(program);
}
//--------------------------------------------------------------------------------------------
void Scene::PrepareObjects()
{  

	Objects.clear();
	Textures.clear();
	LightPos.clear();
	LightColor.clear();

	LightPos.push_back(glm::vec3(0.32f, 6.48f, 15.36f));
	LightColor.push_back(glm::vec3(10.0f, 10.0f, 10.0f));

	LightPos.push_back(glm::vec3(-26.0f, 2.0f, 5.0f));
	LightColor.push_back(glm::vec3(3.0f, 6.0f, 1.0f));

	LightPos.push_back(glm::vec3(26.0f, 0.0f, 5.0f));
	LightColor.push_back(glm::vec3(10.0f, 2.0f, 1.0f)); 
	
	LightPos.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	LightColor.push_back(glm::vec3(10.0f, 2.0f, 1.0f));

	Skybox = new glObject();
	Skybox->ImportOBJ("skybox.obj", "skybox.mtl", Textures);
	Skybox->SetPosition(glm::vec3(0.0, -90.0, 0.0));
	Skybox->SetScale(glm::vec3(4.0, 4.0, 4.0));

	Objects.push_back(new glObject());
	Objects[0]->ImportOBJ("ufolot.obj", "ufolot.mtl", Textures);


	Objects.push_back(new glObject());
	Objects[1]->ImportOBJ("ognisko.obj", "ognisko.mtl", Textures);
	Objects[1]->SetPosition(LightPos[2]);

	
//	Objects.push_back(new glObject());
//	Objects[3]->ImportOBJ("pilka.obj", "pilka.mtl", Textures);

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("kamien.obj", "kamien.mtl", Textures);

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("pajonk.obj", "pajonk.mtl", Textures);
	Objects.back()->SetRotation(glm::vec4(30, glm::vec3(0.0, 1.0, 0.0)));
	Objects.back()->SetPosition(glm::vec3(8.0, 0.0, -6.0));
	
	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("plot.obj", "plot.mtl", Textures);


	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("drzewo.obj", "drzewo.mtl", Textures);
	Objects.back()->SetPosition(glm::vec3(-15.0f, 0.0f, -14.0f));

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("liscie.obj", "liscie.mtl", Textures);
	Objects.back()->SetPosition(glm::vec3(-15.0f, 0.0f, -14.0f));

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("feuer.obj", "feuer.mtl", Textures);
	Objects.back()->SetPosition(LightPos[2]);

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("ghost.obj", "ghost.mtl", Textures);
	Objects.back()->SetRotation(glm::vec4(130.0, 0.0, 1.0, 0.0));
	Objects.back()->SetPosition(glm::vec3(-5.516f, 0.0f, 18.875f));

	
	Objects.push_back(new glObject());
	Objects.back() ->ImportOBJ("lawka.obj", "lawka.mtl", Textures);
	//Objects.push_back(new glObject());
	//Objects.back()->ImportOBJ("sciezka.obj", "sciezka.mtl", Textures);

	Objects.push_back(new glObject());
	Objects.back()->ImportOBJ("floor.obj", "floor.mtl", Textures);


	
	

}
//--------------------------------------------------------------------------------------------
// Odpowiada za skalowanie sceny przy zmianach rozmiaru okna
void Scene::Resize(int new_width, int new_height)
{
	// przypisz nowe gabaryty do pol klasy 
	width = new_width;
	// uwzgledniaj obecnosc kontrolki wizualnej 
	height = new_height; 	
  	// rozszerz obszar renderowania do obszaru o wymiarach 'width' x 'height'
	glViewport(0, 0, width, height);	
	
	mProjection = glm::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
}
//--------------------------------------------------------------------------------------------
// laduje program shadera z zewnetrznego pliku 
GLuint Scene::LoadShader(GLenum type,const char *file_name)
{  
  // zmienna plikowa 
  FILE *fil = NULL; 
  // sproboj otworzyc plik 
  fil = fopen(file_name,"rb");
  // sprawdz, czy plik sie otworzyl  
  sprintf(_msg,"Nie mozna otworzyc %s",file_name);
  if (fil == NULL)  ThrowException(_msg);

  // okresl rozmiar pliku
  fseek( fil, 0, SEEK_END );
  long int file_size = ftell(fil); 
  // przewin na poczatek 
  rewind(fil); 
  // utworzenie bufora na kod zrodlowy programu shadera
  GLchar *srcBuf = new GLchar[(file_size+1)*sizeof(GLchar)];

  // przeczytanie kodu shadera z pliku 
  fread(srcBuf,1,file_size,fil);

  // zamknij plik 
  fclose(fil);

  // tekst programu MUSI miec NULL na koncu
  srcBuf[file_size] = 0x00;

  // utworzenie obiektu shadera
  GLuint shader = glCreateShader(type);

  // przypisanie zrodla do shadera 
  glShaderSource(shader,1,const_cast<const GLchar**>(&srcBuf),NULL);

  // sprzatanie 
  delete[] srcBuf;

  // proba skompilowania programu shadera 
  glCompileShader(shader);

  // sprawdzenie czy sie udalo 
  GLint compile_status;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);

  if (compile_status != GL_TRUE) // nie udalo sie 
  {
	    GLint logLength; 
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetShaderInfoLog( shader, logLength, NULL, log ); 
		sprintf(_msg,"Blad kompilacji pliku shadera %s",file_name);
		PrintLog(_msg);
        PrintLog(log);
		ThrowException("Blad kompilacji shadera");
	    delete []log;
  }
  else
  {
	  sprintf(_msg,"Plik shadera %s skompilowany",file_name);
	  PrintLog(_msg);
  }

  return shader; // zwroc id shadera 
}
//--------------------------------------------------------------------------------------------
// inicjuje proces renderowania OpenGL
void Scene::Init()
{
	// inicjalizacja moduｳu glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		sprintf(_msg, "GLew error: %s\n", glewGetErrorString(err));	
		ThrowException(_msg);
	}
	// pobierz informacje o wersji openGL 
	sprintf(_msg,"OpenGL vendor: ");
	strcat(_msg,(const char*)glGetString( GL_VENDOR ));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL renderer: ");
	strcat(_msg,(const char*)glGetString( GL_RENDERER));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL version: ");
	strcat(_msg,(const char*)glGetString( GL_VERSION));
	PrintLog(_msg);

	//  ustaw kolor tla sceny (RGB Z=1.0)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// przygotuj programy shaderow
	PreparePrograms();

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------------------
//
void Scene::IncreaseAlpha(float additionalAlpha)
{
	if (additionalAlpha > 0 && Alpha < 1.0f) 
	{
		Alpha += additionalAlpha;
	}
	else if (additionalAlpha < 0 && !Alpha == 0.0f)
	{
		Alpha += additionalAlpha;
	}

}
//--------------------------------------------------------------------------------------------
//
void Scene::ToggleWireframe()
{
	if (wireframeOn)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		wireframeOn = false;
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		wireframeOn = true;
	};
}
//--------------------------------------------------------------------------------------------
// kontrola naciskania klawiszy klawiatury
void Scene::KeyPressed(unsigned char key, int x, int y)
{
	if (key == ESCAPE) ThrowException("Zatrzymaj program");
	switch (key)
	{
	case (int)'T':{
					  xwalk += 1.0;
					  R += 0.36;
					  break;
	}
	case (int)'Y':{
					  xwalk += 1.0;
					  R += 0.36;
					  break;
	}
	case (int)'W':{
					  duchpos.x += 1.0;

					  break;
	}
	case (int)'S':{
					  duchpos.x -= 1.0;

					  break;
	}
	case 49: {
				 chodzi = true;
				 camPos = glm::vec3(1.00f, 10.0f, 1.00f);
				 camLookAt = glm::vec3(9.0f, 5.0f, 33.0f);
				 break;
	}
	case 50: {
				 chodzi = false;
				 AD = 0;
				 xwalk = 0;
				 camPos = glm::vec3(-8.30f, 5.50f, -0.30f);
				 camLookAt = glm::vec3(0.0f, 5.0f, -24.0f);
				 break;
	}
	case 51: {
				 chodzi = false;
				 AD = 0;
				 xwalk = 0;
				 camPos = glm::vec3(9.30f, 6.50f, -9.30f);
				 camLookAt = glm::vec3(0.0f, 5.0f, 0.0f);
				 break;
	}
	case 52: {
				 chodzi = false;
				 AD = 0;
				 xwalk = 0;
				 camPos = glm::vec3(4.30f, 10.00f, 60.00f);
				 camLookAt = glm::vec3(0.0f, 5.0f, 0.0f);
				 break;
	}

	case 65: {
				 duchpos.z -= 1.0;
				 break;
	}
	case 68: {
				 duchpos.z += 1.0;
				 break;
	}

	case 32: {
				 katFurtki = -katFurtki;
				 break;
	}
	case 90: { ToggleWireframe(); break; }
	case 88: {if (Alpha == 0.5f)
	{
				 Alpha = 1.0f;
	}
			 else
			 {
				 Alpha = 0.5f;
			 }
			 break;
	}
	case 116: {FogDensity -= 0.05; break; } //F5		
	case 117: {FogDensity += 0.05; break; } //F6	
	}
	if (R >= 6.243) R = 0;
}
//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::Draw()
{	

	glUniform1i(glGetUniformLocation(program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(program, "material.specular"), 1);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), 10.0f);
	glUniform1i(glGetUniformLocation(program, "disableLighting"), 0);
	glUniform1f(glGetUniformLocation(program, "Alpha"), 1.0f);

	GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
	glUniform3f(viewPosLoc, position.x, position.y, position.z);

	// czyscimy bufor kolorow
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);

	PrepareLights();

	

	int _View = glGetUniformLocation(program, "viewMatrix");
	int _Projection = glGetUniformLocation(program, "projectionMatrix");
	int _NormalMatrix = glGetUniformLocation(program, "normalMatrix");
	int _ModelMatrix = glGetUniformLocation(program, "modelMatrix");


	//direction = glm::vec3(0.5 * cos(rot_x), 1.0 * angleY, 0.5 * sin(rot_x)); // chodzenie 
	glm::mat4 mView = glm::lookAt(camPos, camLookAt, glm::vec3(0.0f, 1.0f, 0.0f)); 
	mView = glm::translate(mView, glm::vec3(0.0, 0.3*sin(R), xwalk));
	mView = glm::rotate(mView, AD, glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 mTransform = glm::mat4(1.0);

	glUniformMatrix4fv(_Projection, 1, GL_FALSE, glm::value_ptr(mProjection));
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(mView));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, 
			glm::value_ptr(glm::transpose(glm::inverse(mTransform))));	




	glm::mat4 model;

	glUniform1i(glGetUniformLocation(program, "disableLighting"), 1);
	model = glm::translate(model, Skybox->GetPosition());
	model = glm::scale(model, Skybox->GetScale());
	glUniformMatrix4fv(_ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
	Skybox->Draw();
	glUniform1i(glGetUniformLocation(program, "disableLighting"), 0);

	glEnable(GL_BLEND);										// Przezroczystosc
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < Objects.size() - 1; i++)
	{	
		model = glm::mat4();
		model = glm::translate(model, Objects[i]->GetPosition());
		if (Objects[i]->GetRotation()[0] != 0.0f)
		{
			model = glm::rotate(model, Objects[i]->GetRotation()[0], glm::vec3(Objects[i]->GetRotation()[1], Objects[i]->GetRotation()[2], Objects[i]->GetRotation()[3]));
		}
		model = glm::scale(model, Objects[i]->GetScale());

		if (i == 8)
		{
			glUniform1f(glGetUniformLocation(program, "Alpha"), Alpha);
		}
		if (i == 7)
		{
			glUniform1f(glGetUniformLocation(program, "Alpha"), 0.3);
		}
		glUniformMatrix4fv(_ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
		Objects[i]->Draw();		

		if (i == 8 || i == 7)
		{
			glUniform1f(glGetUniformLocation(program, "Alpha"), 1.0f);
		}
	}


	model = glm::mat4();
	model = glm::translate(model, Objects.back()->GetPosition());
	model = glm::scale(model, Objects.back()->GetScale());
	glUniformMatrix4fv(_ModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
	Objects.back()->Draw();
	glDisable(GL_BLEND);
int _FogDensity = glGetUniformLocation(program, "FogDensity");
	glUniform1f(_FogDensity, FogDensity);


}	
//--------------------------------------------------------------------------------------------
//Ustawienia swiatla
void Scene::PrepareLights()
{

	glUniform3f(glGetUniformLocation(program, "dirLight.direction"), 1.2f, -1.0f, 1.2f);
	glUniform3f(glGetUniformLocation(program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(program, "dirLight.specular"), 1.5f, 1.5f, 1.5f);


	// Point light 1
	glUniform3f(glGetUniformLocation(program, "pointLights[0].position"), LightPos[0].x, LightPos[0].y, LightPos[0].z);
	glUniform3f(glGetUniformLocation(program, "pointLights[0].ambient"), LightColor[0].x, LightColor[0].y, LightColor[0].z);
	glUniform3f(glGetUniformLocation(program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].linear"), 0.9);
	glUniform1f(glGetUniformLocation(program, "pointLights[0].quadratic"), 0.032);

	// Point light 2
	glUniform3f(glGetUniformLocation(program, "pointLights[1].position"), Objects[0]->GetPosition().x, Objects[0]->GetPosition().y, Objects[0]->GetPosition().z);
	glUniform3f(glGetUniformLocation(program, "pointLights[1].ambient"), 10*LightColor[1].x, 10*LightColor[1].y, 10*LightColor[1].z);
	glUniform3f(glGetUniformLocation(program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].linear"), 0.9);
	glUniform1f(glGetUniformLocation(program, "pointLights[1].quadratic"), 0.032);

	// Point light 3
	glUniform3f(glGetUniformLocation(program, "pointLights[2].position"), LightPos[2].x, LightPos[2].y, LightPos[2].z);
	glUniform3f(glGetUniformLocation(program, "pointLights[2].ambient"), LightColor[2].x+2*sin(rot_x*31.4/360), LightColor[2].y, LightColor[2].z);
	glUniform3f(glGetUniformLocation(program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[2].linear"), 0.9);
	glUniform1f(glGetUniformLocation(program, "pointLights[2].quadratic"), 0.032);

	// Point light 3
	glUniform3f(glGetUniformLocation(program, "pointLights[3].position"), Objects[8]->GetPosition().x, Objects[8]->GetPosition().y, Objects[8]->GetPosition().z);
	glUniform3f(glGetUniformLocation(program, "pointLights[3].ambient"), 0.3f, 0.6f, 0.1f);
	glUniform3f(glGetUniformLocation(program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLights[3].linear"), 0.9);
	glUniform1f(glGetUniformLocation(program, "pointLights[3].quadratic"), 0.032);

}
//--------------------------------------------------------------------------------------------
//Animacja sceny
void Scene::Animate()
{
	glm::vec3 poz = Objects[0]->GetPosition();
	rot_x++;
	Objects[0]->SetRotation(glm::vec4(rot_x, 0.0, 1.0, 0.0));
	Objects[0]->SetPosition(glm::vec3(0.0, poz.y, 0.0));
	if ((poz.y < 100.0f && katFurtki > 0) || (poz.y > 0.0f && katFurtki < 0))
	{
		Objects[0]->SetPosition(glm::vec3(poz.x, poz.y + katFurtki, poz.z));
	}
	if (rot_x>6282) rot_x = 0;
	duchpos.y = 1.0+ sin(rot_x/100);
	Objects[8]->SetPosition(duchpos);
	
}
//ruszmysz
void Scene::RuszMysz(int x, int y){
	

	if (chodzi)
	{
		AD += x;
		zwalk += y;
	}

}

//------------------------------- KONIEC PLIKU -----------------------------------------------