#pragma once

#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL.h>
#include <GL/glew.h>
#endif

#include <vector>
#include <glm/glm.hpp>
#include <string>


class WindowHandler {
public:
	//This ensures that there can only be one WindowHandler object at any given time.
	static WindowHandler& getInstance() {
		static WindowHandler instance;
		return instance;
	}

	bool init();
	void close();

	glm::vec2 getScreenSize();
	float getAspectRatio();
	SDL_Window* getWindow();

	//This ensures that there can only be one WindowHandler object at any given time.
	WindowHandler(WindowHandler const& copy) = delete;
	void operator=(WindowHandler const& copy) = delete;
private:
	SDL_Window* window;
	SDL_GLContext context;

	std::string windowName;
	int windowXSize;
	int windowYSize;

	WindowHandler() {};

	bool initSDL();
	bool initGL();
};