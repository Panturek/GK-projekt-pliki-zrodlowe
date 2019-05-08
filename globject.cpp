#include "globject.h"

int CalcNormal(float A[], float B[], float C[], float *N)
{
const int x = 0;
const int y = 1;
const int z = 2;
float U[3];
float V[3];
// oblicz wspolrzedne wektorow U oraz V
U[x] = A[x] - B[x];
U[y] = A[y] - B[y];
U[z] = A[z] - B[z];
V[x] = B[x] - C[x];
V[y] = B[y] - C[y];
V[z] = B[z] - C[z];
// wyznacz wspolrzedne normalnej
N[x] = U[y]*V[z] - U[z]*V[y];
N[y] = U[z]*V[x] - U[x]*V[z];
N[z] = U[x]*V[y] - U[y]*V[x];
return 1;
}

int Normalize(float *N)
{
const int x = 0;
const int y = 1;
const int z = 2;
// oblicz dlugosc wektora
float L = (float)sqrt(N[x]*N[x] + N[y]*N[y] + N[z]*N[z]);
if (L<0.01f) L=0.01f;
// wyznacz wspolrzedne normalnej
N[x] /= L;
N[y] /= L;
N[z] /= L;
return 1;
}
//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glObject::glObject()
{			
	Coords = (float *) malloc(sizeof(float));	
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));
	TexCoords = (float *) malloc(sizeof(float));
	nx = 1.0;
	ny = 0.0;
	nz = 0.0;
	col_r = 0.0;
	col_g = 0.0;
	col_b = 0.0;
	lVAO = 0;
	Position = glm::vec3(0.0, 0.0, 0.0);
	Rotation = glm::vec4(0.0f, 0.0, 0.0, 0.0);
	originPos = glm::vec3(0.0, 0.0, 0.0);
	Scale = glm::vec3(1.0, 1.0, 1.0);
	texturesOn = true;
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glObject::~glObject()
{	
	if (Coords) free(Coords);
	if (Cols) free(Cols);		
	if (Normals) free(Normals);
	if (TexCoords) free(TexCoords);
}
//--------------------------------------------------------------------------------------------
// ustawia aktualny kolor rysowania prymitywu 
void glObject::SetColor(float r, float g, float b)
{
	col_r = r;
	col_g = g; 
	col_b = b;
}
//--------------------------------------------------------------------------------------------
// ustawia aktualna normalna 
void glObject::SetNormal(float _nx, float _ny, float _nz)
{
	nx = _nx;
	ny = _ny; 
	nz = _nz;
}
//--------------------------------------------------------------------------------------------
// usuwa alokowane atrybuty
void glObject::CleanUp()
{
	lVAO = 0; 
}
//--------------------------------------------------------------------------------------------
// rozpoczyna tworzenie tablicy VAO dla danego prymitywu
void glObject::BeginObject(GLenum P,GLuint TextureId)
{	
	lVAO++;
	// przypisz rodzaj prymitywu do narysowania VAO
	Primitives[lVAO-1] = P;

	// przypisz Id tekstury do narysowania VAO
	Textures[lVAO - 1] = TextureId;

	// wyzeruj licznik wspolrzednych 
	lCoords[lVAO-1] = 0;
	Coords = (float *) malloc(sizeof(float));
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));

	if (lVAO > MAX_VAO) ThrowException("Przekroczono maksymalna liczbe VAO w glObject");
	
	GLuint VAO_id[1];	
	// przygotuj tablice VAO
	glGenVertexArrays(1, VAO_id); 
	VAO[lVAO-1] = VAO_id[0];

	glBindVertexArray(VAO[lVAO-1]);

	GLuint VBO_id[4];	
	// przygotuj bufory VBO
	glGenBuffers(4, VBO_id);
	
	VBO[4*lVAO-4] = VBO_id[0];
	VBO[4*lVAO-3] = VBO_id[1];
	VBO[4*lVAO-2] = VBO_id[2];
	VBO[4*lVAO-1] = VBO_id[3];
	
}
//--------------------------------------------------------------------------------------------
// dodaje wierzcholek do listy ze wsp. tekstury 
void glObject::AddVertex(float x, float y, float z, float u, float v)
{
	lCoords[lVAO-1] +=3;
	Coords = (float *) realloc(Coords, lCoords[lVAO-1]*sizeof(float));
	if (Coords == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Coords[lCoords[lVAO-1]-3] = x;
	Coords[lCoords[lVAO-1]-2] = y;
	Coords[lCoords[lVAO-1]-1] = z;

	Cols = (float *) realloc(Cols, lCoords[lVAO-1]*sizeof(float));
	if (Cols == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Cols[lCoords[lVAO-1]-3] = col_r;
	Cols[lCoords[lVAO-1]-2] = col_g;
	Cols[lCoords[lVAO-1]-1] = col_b;	

	Normals = (float *) realloc(Normals, lCoords[lVAO-1]*sizeof(float));
	if (Normals == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Normals[lCoords[lVAO-1]-3] = nx;
	Normals[lCoords[lVAO-1]-2] = ny;
	Normals[lCoords[lVAO-1]-1] = nz;	

	TexCoords = (float *) realloc(TexCoords, lCoords[lVAO-1]*sizeof(float));
	if (TexCoords == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	TexCoords[lCoords[lVAO-1]-3] = u;
	TexCoords[lCoords[lVAO-1]-2] = v;	
	TexCoords[lCoords[lVAO-1]-1] = 0.0;	
}
//--------------------------------------------------------------------------------------------
void glObject::EndObject()
{
	// podlacz pierwszy obiekt z VAOs
	glBindVertexArray(VAO[lVAO-1]);
	// podlacz pierwszy bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-4]);	
	// wypelnij bufor wspolrzednymi wierzcholka 

	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Coords, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 0 (wskazany w shaderze)
	glEnableVertexAttribArray(0); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz drugi bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-3]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Cols, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 1 (wskazany w shaderze)
	glEnableVertexAttribArray(1); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz trzeci bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-2]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Normals, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 2 (wskazany w shaderze)
	glEnableVertexAttribArray(2); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz czwarty bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-1]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), TexCoords, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 3 (wskazany w shaderze)
	glEnableVertexAttribArray(3); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	glBindVertexArray(0);

}
//--------------------------------------------------------------------------------------------
void glObject::Draw()
{
	for (int i = 0; i < lVAO; i++)
	{
		if (Textures[i] == 0)
		{
			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			if (texturesOn)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Textures[i]);
				glEnable(GL_TEXTURE_2D);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
			}
		}
		glBindVertexArray(VAO[i]);
		glDrawArrays(Primitives[i], 0, lCoords[i] / 3);
		glBindVertexArray(0);
	}
}
//--------------------------------------------------------------------------------------------
void glObject::SetPosition(glm::vec3 newPosition)
{
	Position = newPosition;
}
//--------------------------------------------------------------------------------------------
void glObject::SetRotation(glm::vec4 newRotation)
{
	Rotation = newRotation;
}
//--------------------------------------------------------------------------------------------
void glObject::SetOrigin(glm::vec3 newOrigin)
{
	originPos = newOrigin;
}
//--------------------------------------------------------------------------------------------
void glObject::SetScale(glm::vec3 newScale)
{
	Scale = newScale;
}
//--------------------------------------------------------------------------------------------
void glObject::ImportOBJ(const char* objfilename, const char* mtlfilename, std::vector< glTexture* > Textures)
{
	std::string line, line2;

	std::ifstream in(mtlfilename, std::ios::in);

	std::vector< std::string > temp_name;

	while (std::getline(in, line))					// wczytywanie tekstur do wektora Textures i nadawanie im nazw
	{
		std::string lineChar = line.substr(0, 6);

		if (lineChar == "newmtl")
		{
			char name[50];
			line[0] = ' '; line[1] = ' '; line[2] = ' '; line[3] = ' '; line[4] = ' '; line[5] = ' ';
			std::sscanf(line.c_str(), "%s", name);
			temp_name.push_back(name);
		}
		else if (lineChar == "map_Kd")
		{
			char path[50];
			line[0] = ' '; line[1] = ' '; line[2] = ' '; line[3] = ' '; line[4] = ' '; line[5] = ' ';
			sscanf(line.c_str(), "%s", path);
			std::string path_str = std::string(path);
			path_str = "textures//" + path_str;
			Textures.push_back(new glTexture(path_str.c_str(), temp_name.back()));
		}
	}

	temp_name.clear();

	std::ifstream in2(objfilename, std::ios::in);

	std::vector< glm::vec3 > temp_vectors;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec2 > temp_objects;
	std::vector< std::string > temp_texName;

	int begin = 0;
	int end = -1;

	boolean firstRun = true;

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

	while (std::getline(in2, line2))
	{
		std::string lineChar = line2.substr(0, 2);

		if (lineChar == "v ")						// wczytywanie wierzcholkow
		{
			glm::vec3 vector;
			line2[0] = ' '; // Set first character to 0. This will allow us to use sscanf

			sscanf(line2.c_str(), "%f %f %f ", // Read floats from the line: v X Y Z
				&vector.x,
				&vector.y,
				&vector.z);
			temp_vectors.push_back(vector);
		}
		else if (lineChar == "vt")					// wczytywanie polozenia tekstur UV
		{
			glm::vec2 uv;

			line2[0] = ' '; // Set first character to 0. This will allow us to use sscanf
			line2[1] = ' '; // Set first character to 0. This will allow us to use sscanf

			sscanf(line2.c_str(), "%f %f ", // Read floats from the line: v X Y Z
				&uv.x,
				&uv.y);
			temp_uvs.push_back(uv);
		}
		else if (lineChar == "vn")					// wczytywanie wektorow normalnych
		{
			glm::vec3 normal;

			line2[0] = ' '; // Set first character to 0. This will allow us to use sscanf
			line2[1] = ' '; // Set first character to 0. This will allow us to use sscanf

			sscanf(line2.c_str(), "%f %f %f ", // Read floats from the line: v X Y Z
				&normal.x,
				&normal.y,
				&normal.z);
			temp_normals.push_back(normal);
		}
		else if (lineChar == "f ")					// wczytywanie trojkatow
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

				end++;

			line2[0] = ' '; // Set first character to 0. This will allow us to use sscanf

			sscanf(line2.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if (lineChar == "us")					// jaka tekstura na jaki obiekt
		{
			char name[50];

			line2[0] = ' '; line2[1] = ' '; line2[2] = ' '; line2[3] = ' '; line2[4] = ' '; line2[5] = ' ';
			std::sscanf(line2.c_str(), "%s", name);
			temp_texName.push_back(name);


			if (!firstRun)
			{
				temp_objects.push_back(glm::vec2(begin, end));
				begin = end;
				begin++;
			}
			else
			{
				firstRun = false;
			}

		}
		else if (line2[0] == '#')
		{
			// ignoring this line 
		}
		else
		{
			// ignoring this line 
		}
	}
	temp_objects.push_back(glm::vec2(begin, end));


	for (int i = 0; i < temp_objects.size(); i++)
	{
		for (int j = 0; j < Textures.size(); j++)
		{
			if (Textures[j]->GetName().compare(temp_texName[i]) == 0)		// wybor tekstury na obiekt
			{
				BeginObject(GL_TRIANGLES, Textures[j]->Bind());
				break;
			}
		}

		for (int k = temp_objects[i].x; k < temp_objects[i].y + 1; k++)		// rysowanie trojkatow
		{
			SetNormal(temp_normals[normalIndices[k * 3] - 1].x, temp_normals[normalIndices[k * 3] - 1].y, temp_normals[normalIndices[k * 3] - 1].z);
			AddVertex(temp_vectors[vertexIndices[k * 3] - 1].x, temp_vectors[vertexIndices[k * 3] - 1].y, temp_vectors[vertexIndices[k * 3] - 1].z, temp_uvs[uvIndices[k * 3] - 1].x, temp_uvs[uvIndices[k * 3] - 1].y);
			SetNormal(temp_normals[normalIndices[k * 3 + 1] - 1].x, temp_normals[normalIndices[k * 3 + 1] - 1].y, temp_normals[normalIndices[k * 3 + 1] - 1].z);
			AddVertex(temp_vectors[vertexIndices[k * 3 + 1] - 1].x, temp_vectors[vertexIndices[k * 3 + 1] - 1].y, temp_vectors[vertexIndices[k * 3 + 1] - 1].z, temp_uvs[uvIndices[k * 3 + 1] - 1].x, temp_uvs[uvIndices[k * 3 + 1] - 1].y);
			SetNormal(temp_normals[normalIndices[k * 3 + 2] - 1].x, temp_normals[normalIndices[k * 3 + 2] - 1].y, temp_normals[normalIndices[k * 3 + 2] - 1].z);
			AddVertex(temp_vectors[vertexIndices[k * 3 + 2] - 1].x, temp_vectors[vertexIndices[k * 3 + 2] - 1].y, temp_vectors[vertexIndices[k * 3 + 2] - 1].z, temp_uvs[uvIndices[k * 3 + 2] - 1].x, temp_uvs[uvIndices[k * 3 + 2] - 1].y);
		}
		EndObject();
	}


	
}
glm::vec3 glObject::GetPosition()
{
	return Position;
}
glm::vec4 glObject::GetRotation()
{
	return Rotation;
}
glm::vec3 glObject::GetOriginPos() 
{
	return originPos;
}
glm::vec3 glObject::GetScale()
{
	return Scale;
}
void glObject::ToggleTextures()
{
	if (texturesOn)
	{
		texturesOn = false;
	}
	else
	{
		texturesOn = true;
	}
}
// the end