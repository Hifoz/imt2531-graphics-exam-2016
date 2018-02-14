#pragma once
#include "Mesh.h"
#include "TextureHandler.h"
#include  "LightHandler.h"
#include "ShaderHandler.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(std::string meshPath, std::string textureDir, TextureHandler* tH);
	~Skybox();

	//void loadCubemap(TextureHandler* tH) {m_texture = tH->loadCubemapTexture("")}

	void draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera);
private:
	Mesh* m_mesh;
	GLuint m_texture;

};

