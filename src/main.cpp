#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <project/fucntions.h>


SDL_Window* MyWindow;
SDL_GLContext MyContext;
bool gQuit = false;
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;


int main() {

	intializer(640, 480);

	CreateVertexSpecs();

	mainloop();

	cleanup();
}