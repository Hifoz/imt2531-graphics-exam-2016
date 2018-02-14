#include "ShaderHandler.h"

const std::string shaderPath = "./resources/shaders/";

ShaderHandler::ShaderProgram* ShaderHandler::initializeShaders()
{
	this->shaders["phong"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("phong"));
	this->shaders["textured"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("textured"));
	this->shaders["skybox"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("skybox"));
	this->shaders["mirror"] = std::unique_ptr<ShaderProgram>(new ShaderProgram("mirror"));

	return this->shaders["phong"].get();
}

ShaderHandler::ShaderProgram* ShaderHandler::getShader(const std::string& shader)
{
	auto it = this->shaders.find(shader);
	if (it != this->shaders.end())
	{
		return this->shaders[shader].get();
	}

	return nullptr;
}

ShaderHandler::ShaderProgram::ShaderProgram(const std::string& shader)
{
	const std::string vertexSuffix = ".vs";
	const std::string fragmentSuffix = ".fs";

	this->programId = LoadShaders((shaderPath + shader + vertexSuffix).c_str(), (shaderPath + shader + fragmentSuffix).c_str());

	this->MVPId = glGetUniformLocation(this->programId, "MVP");
	this->projectionMatrixId = glGetUniformLocation(this->programId, "ProjectionMatrix");
	this->viewMatrixId = glGetUniformLocation(this->programId, "ViewMatrix");
	this->modelMatrixId = glGetUniformLocation(this->programId, "ModelMatrix");
	this->cameraPositionId = glGetUniformLocation(this->programId, "viewPos");
	this->textureId = glGetUniformLocation(this->programId, "textureBuffer");

	glUseProgram(this->programId);
}

ShaderHandler::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->programId);
}