#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <project/fucntions.h>


// Window and Context
SDL_Window* MyWindow;
SDL_GLContext MyContext;
bool gQuit = false;

// VAO and VBO
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

// Graphics Pipleline
GLuint gGraphicsPipelineProgram = 0;
std::string gVertexShaderSource;
std::string gFragmentShaderSource;



int main() {

	intializer(640, 480);

	CreateGraphicsPipeline();

	CreateSpecs();
	
	mainloop();

	cleanup();
}