#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Mesh.h"
#include "Camera.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"

struct Transform
{
	glm::vec3 pos, rot, scale;

	glm::mat4 getModelMatrix()
	{
		return glm::translate(pos) * glm::mat4_cast(glm::fquat(glm::radians(rot))) * glm::scale(scale);
	}
	glm::vec3 getFront()
	{
		return glm::vec3(glm::sin(glm::radians(rot.y)), 0.0f, glm::cos(glm::radians(rot.y)));
	}
};


class GameObject
{
public:
	Transform transform;

	GameObject(std::string meshPath, bool wantTexture = false);
	GameObject(Mesh* mesh, glm::vec3 position, bool wantTexture = false);
	~GameObject();

	void draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera);

	void setTexture(GLuint texture);
	
	glm::mat4 getModelMatrix();
	Mesh* getMesh();

protected:
	Mesh* m_mesh;
	GLuint m_texture;
	std::string m_shader;
};

