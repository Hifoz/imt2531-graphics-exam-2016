#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "WindowHandler.h"

#include <stdio.h>
#if defined(__linux__)						// If we are using linux.
#include <SDL2/SDL_image.h>

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64)	// If we are using a windows machine
#include <SDL_image.h>
#endif

bool WindowHandler::init() {
	windowName = "Graphics Exam 2016 - Michael Bråten";
	windowXSize = 1600;
	windowYSize = 900;

	if (!initSDL())
		return false;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	return true;
}

bool WindowHandler::initSDL() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowXSize, windowYSize, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create context
			context = SDL_GL_CreateContext(window);
			if (context == NULL) {
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK) {
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}
				if (SDL_GL_SetSwapInterval(1) < 0) {
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}
				if (!initGL()) {
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}
	return success;
}

bool WindowHandler::initGL() 
{
	//Success flag
	bool success = true;

	//// Enable culling
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_BACK);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return success;
}


void WindowHandler::close() {

	SDL_DestroyWindow(window);
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

glm::vec2 WindowHandler::getScreenSize() {
	glm::vec2 size;
	size.x = windowXSize;
	size.y = windowYSize;
	return size;
}

float WindowHandler::getAspectRatio()
{
	return windowXSize / (float)windowYSize;
}

SDL_Window* WindowHandler::getWindow() {
	return window;
}