#pragma once
#include "ShaderHandler.h"
#include "GameObject.h"
#include "Camera.h"


class Mirror : public GameObject
{
public:
	Mirror(std::string path);
	~Mirror();

	GLuint getFrameBuffer() { return fbo; }
	GLuint getTexture() { return texture; }

	void draw(ShaderHandler::ShaderProgram* shaderProgram, Camera* camera);
	void setupFBO();

	void setCarTransform(Transform* ct) { carTransform = ct; }

private:
	Camera* m_camera;
	GLuint fbo, rbo, texture;

	Transform transform2;	// Transform for right mirror
	Transform* carTransform;
};

