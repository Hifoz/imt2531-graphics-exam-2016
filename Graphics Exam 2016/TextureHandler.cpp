#include "TextureHandler.h"

void TextureHandler::loadTexture(const std::string& name, const std::string& path)
{
	// SDL Image is used to load the image and save it as a surface.
	SDL_Surface* textureSurface = IMG_Load(path.c_str());
	if (!textureSurface)
	{
		printf("Failed to load texture \"%s\": %s\n", path.c_str(), IMG_GetError());
		return;
	}

	// Check if the texture has an alpha channel.
	GLenum textureFormat = GL_RGB;
	if (textureSurface->format->BytesPerPixel == 4)
		textureFormat = GL_RGBA;


	// Generate and bind the texture buffer
	GLuint textureBuffer = 0;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);

	
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureSurface->w, textureSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, textureSurface->pixels);


	// We tell OpenGL what to do when it needs to render the texture at a higher or lower resolution.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_FreeSurface(textureSurface);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_textures.insert(std::pair<std::string, int>(name, textureBuffer));
}

void TextureHandler::loadCubemapTexture(const std::string& name, std::vector<std::string> paths)
{
	// SDL Image is used to load the image and save it as a surface.
	SDL_Surface* textureSurface;


	GLuint textureBuffer;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureBuffer);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (auto i = 0; i < paths.size(); i++)
	{
		textureSurface = IMG_Load(paths[i].c_str());
		if (!textureSurface)
		{
			printf("Failed to load texture \"%s\": %s\n", paths[i].c_str(), IMG_GetError());
			return;
		}
		// Check if the texture has an alpha channel.
		GLenum textureFormat = textureSurface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat, textureSurface->w, textureSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
	}


	m_textures.insert(std::pair<std::string, int>(name, textureBuffer));
}


GLuint TextureHandler::getTexture(const std::string& name)
{
	if (m_textures.count(name))
		return m_textures[name];

	printf("ERROR: No texture with the name \"%s\" could be found.\n", name.c_str());
	return 0;
}


GLenum TextureHandler::getCorrectTexelFormat(const SDL_Surface *textureSurface)
{
	/*
	*  SDL_image will load images in the BGR(A) format on some systems.
	*  Here we check if the blue color is the first color and sets the BGR(A) format if it is.
	*/

	//Finds the right format of the loaded picture
	GLenum texelFormat = GL_RGB;
	if (textureSurface->format->Bmask == 0xFF)
	{
		texelFormat = GL_BGR;
		if (textureSurface->format->BytesPerPixel == 4)
		{
			texelFormat = GL_BGRA;
		}
	}
	else
	{
		if (textureSurface->format->BytesPerPixel == 4)
		{
			texelFormat = GL_RGBA;
		}
	}
	return texelFormat;
}