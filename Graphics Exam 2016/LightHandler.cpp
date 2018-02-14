#include "LightHandler.h"



void LightHandler::init()
{
	// Set default values for spotlights on the car
	SpotLight light;
	light.pos = glm::vec3(0, 1, 0);
	light.dir = glm::vec3(0, -1, 0);
	light.arc = glm::radians(35.0f);
	light.color = glm::vec3(1.0f, 0.7f, 0.1f);
	carLights.push_back(light);
	carLights.push_back(light);
	carLights.push_back(light);
	carLights.push_back(light);
	carLights.push_back(light);
	carLights.push_back(light);


	// Set default values for the skylight
	skyLight.dir = glm::vec3(0, sinf(glm::radians(20.0f)), cosf(glm::radians(20.0f)));

	// Doing this so dayilght-values don't have to be written here and in toggleNighttime()
	isNight = true;
	toggleNighttime();
}


void LightHandler::toggleNighttime()
{
	isNight = !isNight;

	if (isNight)
	{
		// Set nighttime settings
		skyLight.color = glm::vec3(0.4, 0.4, 0.5);
		skyLight.diffuse = glm::vec3(0.4);
		skyLight.specular = glm::vec3(0.6);
		for (auto i = 0; i < carLights.size(); ++i)
		{
			carLights[i].diffuse = glm::vec3(0.3, 0.3, 0.3);
			carLights[i].specular = glm::vec3(1.0, 1.0, 1.0);
		}
		return;
	}
	// Set daytime settings
	skyLight.color = glm::vec3(1, 0.9, 0.8);
	skyLight.diffuse = glm::vec3(0.9);
	skyLight.specular = glm::vec3(1.0);

	for (auto i = 0; i < carLights.size(); ++i)
	{
		carLights[i].diffuse = glm::vec3(0); 
		carLights[i].specular = glm::vec3(0);
	}
}

void LightHandler::updateCarLights(Transform* carTransform)
{
	for (auto i = 0; i < carLights.size(); ++i)
		carLights[i].dir = -carTransform->getFront();


	glm::mat3 carMMat = glm::mat3(carTransform->getModelMatrix());
	
	// Front-lights
	glm::vec3 frontLightYZ = carMMat[1] * 0.5f + carMMat[2] * 1.f;
	carLights[0].pos = carTransform->pos + carMMat[0] * 1.2f + frontLightYZ;
	carLights[1].pos = carTransform->pos + carMMat[0] * -1.2f + frontLightYZ;

	// Top-lights
	glm::vec3 topLightYZ = carMMat[1] * 2.8f + carMMat[2] * -3.3f;
	carLights[2].pos = carTransform->pos + carMMat[0] * 0.85f + topLightYZ;
	carLights[3].pos = carTransform->pos + carMMat[0] * 0.3f + topLightYZ;
	carLights[4].pos = carTransform->pos + carMMat[0] * -0.3f + topLightYZ;
	carLights[5].pos = carTransform->pos + carMMat[0] * -0.85f + topLightYZ;

}


void LightHandler::sendUniformData(ShaderHandler::ShaderProgram* shaderProgram)
{
	//Skylight
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "directionalLight.dir"), 1, &skyLight.dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "directionalLight.color"), 1, &skyLight.color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "directionalLight.diffuse"), 1, &skyLight.diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "directionalLight.specular"), 1, &skyLight.specular[0]);

	// Carlight front 1
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[0].dir"), 1,		 &carLights[0].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[0].pos"), 1,		 &carLights[0].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[0].color"), 1,	 &carLights[0].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[0].diffuse"), 1,	 &carLights[0].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[0].specular"), 1, &carLights[0].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[0].arc"),		  carLights[0].arc);
	
	// Carlight front 2
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[1].dir"), 1,		 &carLights[1].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[1].pos"), 1,		 &carLights[1].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[1].color"), 1,	 &carLights[1].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[1].diffuse"), 1,  &carLights[1].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[1].specular"), 1, &carLights[1].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[1].arc"),		  carLights[1].arc);


	// Carlight top 1
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[2].dir"), 1,		 &carLights[2].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[2].pos"), 1,		 &carLights[2].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[2].color"), 1,	 &carLights[2].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[2].diffuse"), 1,  &carLights[2].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[2].specular"), 1, &carLights[2].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[2].arc"),		  carLights[2].arc);


	// Carlight top 2
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[3].dir"), 1,		 &carLights[3].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[3].pos"), 1,		 &carLights[3].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[3].color"), 1,	 &carLights[3].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[3].diffuse"), 1,  &carLights[3].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[3].specular"), 1, &carLights[3].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[3].arc"),		  carLights[3].arc);


	// Carlight top 3
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[4].dir"), 1,		 &carLights[4].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[4].pos"), 1,		 &carLights[4].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[4].color"), 1,	 &carLights[4].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[4].diffuse"), 1,  &carLights[4].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[4].specular"), 1, &carLights[4].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[4].arc"),		  carLights[4].arc);


	// Carlight top 4
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[5].dir"), 1,		 &carLights[5].dir[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[5].pos"), 1,		 &carLights[5].pos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[5].color"), 1,	 &carLights[5].color[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[5].diffuse"), 1,  &carLights[5].diffuse[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram->programId, "spotLight[5].specular"), 1, &carLights[5].specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram->programId,  "spotLight[5].arc"),		  carLights[5].arc);


	
}