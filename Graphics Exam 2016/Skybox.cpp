#include "Skybox.h"



Skybox::Skybox(std::string meshPath, std::string textureDir, TextureHandler* tH)
{
	std::vector<std::string> paths;
	paths.push_back(textureDir + "Daylight Box_Right.bmp");
	paths.push_back(textureDir + "Daylight Box_Left.bmp");
	paths.push_back(textureDir + "Daylight Box_Top.bmp");
	paths.push_back(textureDir + "Daylight Box_Bottom.bmp");
	paths.push_back(textureDir + "Daylight Box_Front.bmp");
	paths.push_back(textureDir + "Daylight Box_Back.bmp");
	
	tH->loadCubemapTexture("skybox", paths);

	m_texture = tH->getTexture("skybox");

	m_mesh = new Mesh(meshPath);
}


Skybox::~Skybox()
{
}


void Skybox::draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera)
{
	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glm::mat4 vp = camera->getProjectionMatrix() * glm::mat4(glm::mat3(camera->getViewMatrix()));
	bool isNight = LightHandler::getInstance().checkNight();

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->programId, "VP"), 1, GL_FALSE, &vp[0][0]);
	glUniform1i(glGetUniformLocation(shaderProgram->programId, "isNight"), isNight);
	

	m_mesh->draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthMask(GL_TRUE);

}