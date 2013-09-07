///
///	Author: Christian Magnerfelt
///	
///	Description: Mini Miner is a simple tile-matching puzzle game similar to Midas Miner
///
///
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "logger.hpp"
#include "render_manager.hpp"

#include <cstdint>

bool initializeSDL(
	const char * windowTitle, 
	int32_t screenWidth, 
	int32_t screenHeight, 
	SDL_Window * & window,
	SDL_GLContext & context);
void render(SDL_Window * window, MiniMiner::RenderManager renderManager);

/// Main entry point
int main( int argc, char* argv[] )
{
	Logger::LoggerManager::getInstance().init("debug.log");

	const char * windowTitle = "MiniMiner";
	SDL_Window * window;
	SDL_GLContext context;
	int32_t screenWidth = 800;
	int32_t screenHeight = 600;
	bool isRunning = true;
	
	if(!initializeSDL(windowTitle, screenWidth, screenHeight, window, context))
		return 1;

	// Initialize SDL Image library
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

	MiniMiner::RenderManager renderManager;
	if(!MiniMiner::renderManager::init(renderManager, screenWidth, screenHeight))
		return -1;
	
	// Load background and push to render manager buffer
	GLuint bgId = MiniMiner::renderManager::imageFileToGLTexture(renderManager, "assets/BackGround.jpg");
	MiniMiner::Vec2 bgPos;
	bgPos.x = 0.0f;
	bgPos.y = 0.0f;
	MiniMiner::renderManager::setBackground(renderManager, bgId, bgPos);

	// Enter main loop
	SDL_Event event;
	bool mouseLeft = false;
	int32_t mouseX = 0;
	int32_t mouseY = 0;
	while(isRunning)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_MOUSEBUTTONDOWN :	
					mouseLeft = SDL_BUTTON_LEFT;
					if(mouseLeft)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						// TODO: Send mouse down event to input manager
					}
					break;
				case SDL_MOUSEBUTTONUP :
					mouseLeft = SDL_BUTTON_LEFT;
					if(mouseLeft)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						// TODO: Send mouse up event to input manager
					}
					break;
				default :
					break;
			}
		}
		render(window, renderManager);
	}
	
	MiniMiner::renderManager::releaseTextures(renderManager);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
    SDL_Quit();

	Logger::LoggerManager::getInstance().finalize();
	return 0;
}
bool initializeSDL(
	const char * windowTitle, 
	int32_t screenWidth, 
	int32_t screenHeight, 
	SDL_Window * & window,
	SDL_GLContext & context)
{
	SDL_Init(SDL_INIT_VIDEO);

	// Create an application window with the following settings:
	window = SDL_CreateWindow( 
		windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_OPENGL
	);

	// Set framebuffer pizel format and attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        32);

	if(window == nullptr)
	{
		LOGGER_ERROR("Window creation failed.");
		return false;
	}
	// Create open gl context
	context = SDL_GL_CreateContext(window);

	// This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	return true;
}
void render(SDL_Window * window, MiniMiner::RenderManager renderManager)
{
	MiniMiner::renderManager::renderDrawables(renderManager);
	SDL_GL_SwapWindow(window);
}