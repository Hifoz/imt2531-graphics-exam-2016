#include "Mirror.h"



Mirror::Mirror(std::string path) : GameObject (path, true)
{
	setupFBO();


	// Put the left mirror onto correct placement in car
	transform.rot.x = -90;
	transform.scale = glm::vec3(0.16);
	transform.pos += glm::vec3(1.923, 1.435, -1.245); //glm::vec3(1.923, 1.435, -1.245
	

	// Put the right mirror onto correct placement in car
	transform2.rot.x = -90;
	transform2.scale = glm::vec3(0.16);
	transform2.pos += glm::vec3(-1.923, 1.435, -1.245);
}


Mirror::~Mirror()
{
}


void Mirror::draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera) // SHOULD USE OWN CAMERA, NOT MAIN CAMERA
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * carTransform->getModelMatrix() * transform.getModelMatrix();
	glUniformMatrix4fv(shaderProgram->MVPId, 1, GL_FALSE, &mvp[0][0]);
	m_mesh->draw();
	


	mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * carTransform->getModelMatrix() * transform2.getModelMatrix();
	glUniformMatrix4fv(shaderProgram->MVPId, 1, GL_FALSE, &mvp[0][0]);
	m_mesh->draw();

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Mirror::setupFBO()
{

	//FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glm::vec2 windowSize = WindowHandler::getInstance().getScreenSize();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind texture to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//RBO
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Bind RBO to FOB
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}