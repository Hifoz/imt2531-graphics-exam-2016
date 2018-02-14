#pragma once

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <GL/glew.h>
	#include <SDL2/SDL_opengl.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#include <SDKDDKVer.h>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\glm.hpp>
#endif

#include <vector>
#include <string>

struct ModelData {
	int vertexOrder = 3;
	int colorDepth = 3;
	int normalOrder = 3;
	int uvOrder = 2;

	int numberOfVertexes = 0;
	std::vector<GLfloat> vertexPosition;
	std::vector<GLfloat> vertexColour;
	std::vector<GLfloat> vertexNormal;
	std::vector<GLfloat> vertexTexCoord;

	int numberOfIndices = 0;
	std::vector<GLuint> indices;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::string path, bool wantUVs = false);
	~Mesh();

	void draw();

	GLuint getVAO();
	std::vector<GLuint> getVBOs();
	int getDrawCount();

private:
	void createBufferObjects(ModelData& model);
	ModelData loadModelData(std::string modelPath, std::string materialDir, bool wantUVs);

	GLuint VAO, positionBuffer, colourBuffer, normalBuffer, uvBuffer, indexBuffer;
	int drawCount;
};

