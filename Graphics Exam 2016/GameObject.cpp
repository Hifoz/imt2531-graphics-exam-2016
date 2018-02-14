#include "GameObject.h"



GameObject::GameObject(std::string meshPath, bool wantTexture)
{

	m_mesh = new Mesh(meshPath, wantTexture);
	transform.pos = glm::vec3();
	transform.rot = glm::vec3();
	transform.scale = glm::vec3(1.0);
}

GameObject::GameObject(Mesh* mesh, glm::vec3 position, bool wantTexture)
{
	m_mesh = mesh;
	transform.pos = position;
	transform.rot = glm::vec3();
	transform.scale = glm::vec3(1.0);
}


GameObject::~GameObject()
{
}


void GameObject::draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera)
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * transform.getModelMatrix();
	glUniformMatrix4fv(shaderProgram->MVPId, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(shaderProgram->modelMatrixId, 1, GL_FALSE, &transform.getModelMatrix()[0][0]);
	glUniformMatrix4fv(shaderProgram->viewMatrixId, 1, GL_FALSE, &camera->getViewMatrix()[0][0]);

	glm::vec3 camPos = camera->getPosition();

	glUniform3f(shaderProgram->cameraPositionId, camPos.x, camPos.y, camPos.z);

	m_mesh->draw();
}

void GameObject::setTexture(GLuint texture)
{
	m_texture = texture;
}



glm::mat4 GameObject::getModelMatrix()
{
	return transform.getModelMatrix();
}

Mesh* GameObject::getMesh()
{
	return m_mesh;
}