#include "GameLevel.h"
#include <iostream>
#include <string>

/*
** Sets default values for level
*/
GameLevel::GameLevel(TextureHandler* textureHandler)
{
	Mirror* mirror = new Mirror("resources/models/plane.obj");

	// Load skybox
	skybox = new Skybox("resources/models/cube.obj", "resources/textures/", textureHandler);


	// Load ground
	textureHandler->loadTexture("ground", "resources/textures/track01.png");
	ground = loadGameObject("resources/models/plane.obj", true);
	ground->setTexture(textureHandler->getTexture("ground"));
	ground->transform.scale = glm::vec3(100);
	ground->transform.pos.y -= 2.5;


	//Load trees
	loadTrees("resources/models/");
	placeTree(treeTypes[0], glm::vec3(   0, 1, -15 ));
	placeTree(treeTypes[3], glm::vec3(  60, 1,  10 ));
	placeTree(treeTypes[1], glm::vec3( -15, 1,  65 ));
	placeTree(treeTypes[2], glm::vec3(  42, 1,  15 ));
	placeTree(treeTypes[2], glm::vec3( -40, 1, -10 ));

	// Load car
	car = loadCar("resources/models/car.obj");
	car->setMirror(mirror);

}

/*
** Reads the game level from a filename as a string.  It will try to read files from the same location
*/
GameLevel::GameLevel(TextureHandler* textureHandler, std::string filename)
{
	// Load mirror
	Mirror* mirror = new Mirror("resources/models/plane.obj");
	// Load skybox
	skybox = new Skybox("resources/models/cube.obj", "resources/textures/", textureHandler);

	std::ifstream inputFile(filename);
	std::stringstream lineStream;
	std::string inputLine = "";
	printf("%s", filename.c_str());

	// Rest of file handling here.
	while (!inputFile.eof())
	{
		getline(inputFile, inputLine);
		if (inputLine.compare(0, 2, "c ") == 0) 
		{
			car = loadCar(inputLine.substr(2));
			car->setMirror(mirror);
		}
		else if (inputLine.compare(0, 2, "g ") == 0)
		{
			textureHandler->loadTexture("ground", inputLine.substr(2));
		}
		else if (inputLine.compare(0, 2, "p ") == 0)
		{
			ground = loadGameObject(inputLine.substr(2), true);
		}
		else if (inputLine.compare(0, 2, "t ") == 0)
		{
			loadTrees(inputLine.substr(2));
		}
		else if (inputLine.compare(0, 2, "tp") == 0)
		{
			int nextPos = inputLine.find(",");
			int prevPos = 3;
			std::string tempString = inputLine.substr(prevPos, nextPos);
			int treeType = std::stoi(tempString);

			prevPos = nextPos + 2;
			nextPos = inputLine.find(",", prevPos);
			tempString = inputLine.substr(prevPos, nextPos);
			float posX = std::stof(tempString);

			prevPos = nextPos + 2;
			nextPos = inputLine.find(",", prevPos);
			tempString = inputLine.substr(prevPos, nextPos);
			float posZ = std::stof(tempString);

			placeTree(treeTypes[treeType], glm::vec3(posX, 0, posZ));
		}
	}

	ground->setTexture(textureHandler->getTexture("ground"));
	ground->transform.scale = glm::vec3(100);
	ground->transform.pos.y -= 2.5;



}

GameObject* GameLevel::loadGameObject(std::string filename, bool wantTexture)
{
	std::ifstream inputFile(filename);
	return new GameObject(filename, wantTexture);
}

Car* GameLevel::loadCar(std::string filename)
{
	std::ifstream inputFile(filename);
	return new Car(filename);
}

Car* GameLevel::getCar()
{
	return car;
}

void GameLevel::loadTrees(std::string fileDir)
{
	for (auto i = 1; i < 5; i++)
	{
		std::string filepath = fileDir + "tree" + std::to_string(i) + ".obj";
		treeTypes.push_back(new Mesh(filepath));
	}
}

void GameLevel::placeTree(Mesh* mesh, glm::vec3 position)
{
	GameObject* tree = new GameObject(mesh, position);
	trees.push_back(tree);
}

void GameLevel::draw(ShaderHandler* shaderHandler, Camera* camera)
{


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, car->getMirror()->getFrameBuffer());
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now

	renderScene(shaderHandler, camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	renderScene(shaderHandler, camera);

	ShaderHandler::ShaderProgram* shaderProgram = shaderHandler->getShader("mirror");
	glUseProgram(shaderProgram->programId);
	car->getMirror()->draw(shaderProgram, camera);


}

void GameLevel::renderScene(ShaderHandler* shaderHandler, Camera* camera)
{
	ShaderHandler::ShaderProgram* shaderProgram;

	// Draw skybox
	shaderProgram = shaderHandler->getShader("skybox");
	glUseProgram(shaderProgram->programId);
	skybox->draw(shaderProgram, camera);

	// Draw non-textured objects
	shaderProgram = shaderHandler->getShader("phong");
	glUseProgram(shaderProgram->programId);
	LightHandler::getInstance().sendUniformData(shaderProgram);

	car->draw(shaderProgram, camera);
	for (auto tree : trees)
		tree->draw(shaderProgram, camera);

	// Draw textured objects
	shaderProgram = shaderHandler->getShader("textured");
	glUseProgram(shaderProgram->programId);
	LightHandler::getInstance().sendUniformData(shaderProgram);
	ground->draw(shaderProgram, camera);

}