#pragma once

//Linux
#if defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
//Windows
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#include <SDKDDKVer.h>

	#include <SDL.h>
	#include <SDL_image.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "ShaderHandler.h"
#include "WindowHandler.h"
#include "LightHandler.h"
#include "TextureHandler.h"
#include "Camera.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Car.h"
#include "Mirror.h"
#include "Skybox.h"

class GameLevel
{
public:
	GameLevel(TextureHandler* textureHandler);
	GameLevel(TextureHandler* textureHandler, std::string filename);
	~GameLevel() {};

	GameObject* loadGameObject(std::string filename, bool wantTexture = false);
	Car* loadCar(std::string filename);

	void loadTrees(std::string fileDir); // load the trees from a collected file

	void placeTree(Mesh* mesh, glm::vec3 position);

	
	Car* getCar();

	void draw(ShaderHandler* shaderHandler, Camera* camera);

private:
	Car* car;
	GameObject* ground;
	Skybox* skybox;

	std::vector<Mesh*> treeTypes;
	std::vector<GameObject*> trees;

	void renderScene(ShaderHandler* shaderHandler, Camera* camera);
};

