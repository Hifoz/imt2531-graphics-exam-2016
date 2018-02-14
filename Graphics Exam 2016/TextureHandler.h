#pragma once

#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL_image.h>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif

#include <map>
#include <string>
#include <vector>

class TextureHandler
{
public:
	TextureHandler() {};
	~TextureHandler() {};


	/**
	* Loads an image and creates an OpenGL texture buffer.
	* @param name Name of the texture
	* @param path The path to the image we are loading.
	* @returns The texture identifier for use with OpenGL.
	*/
	void loadTexture(const std::string& name, const std::string& path);
	void loadCubemapTexture(const std::string& name, std::vector<std::string> paths);

	GLuint getTexture(const std::string& name);

private:
	std::map<std::string, GLuint> m_textures;


	GLenum getCorrectTexelFormat(const SDL_Surface* textureSurface);
};