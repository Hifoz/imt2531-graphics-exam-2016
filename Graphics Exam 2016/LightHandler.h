#pragma once

#include <string>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderHandler.h"
#include "GameObject.h"

struct SpotLight
{
	glm::vec3 pos, dir, color, diffuse, specular;
	float arc;
};

struct DirectionalLight
{
	glm::vec3 dir, color, diffuse, specular;
};


class LightHandler
{
public:
	static LightHandler& getInstance() {
		static LightHandler instance;
		return instance;
	}
	LightHandler(LightHandler const& copy) = delete;
	void operator=(LightHandler const& copy) = delete;

	void toggleNighttime();
	bool checkNight() { return isNight; }

	void updateCarLights(Transform* carTransform);

	void sendUniformData(ShaderHandler::ShaderProgram* shaderProgram);

	void init();

private:
	
	bool isNight;

	DirectionalLight skyLight;
	std::vector<SpotLight> carLights;


	LightHandler() {};
};

