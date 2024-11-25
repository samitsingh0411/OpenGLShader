#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include "project/fucntions.h"
#include <vector>

extern SDL_Window* MyWindow;
extern SDL_GLContext MyContext;
extern bool gQuit;
extern GLuint gVertexArrayObject;
extern GLuint gVertexBufferObject;

// Initializer Functions
void intializer(int height, int width) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Failed to initialize SDL" << std::endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	MyWindow = SDL_CreateWindow("Hello World!", 100, 100, height, width, SDL_WINDOW_OPENGL);
	if (!MyWindow) {
		std::cout << "Window Creation Failed" << std::endl;
		exit(-1);
	}


	MyContext = SDL_GL_CreateContext(MyWindow);
	if (!MyContext) {
		std::cout << "Context creation failed" << std::endl;
		SDL_DestroyWindow(MyWindow);
		exit(-1);
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "Glad failed to initialize" << std::endl;
	}

	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
}


// Creating Vertex Specs
void CreateVertexSpecs() {

	// Declaring our vertex data
	const std::vector<GLfloat> VertexPositionList = {
		//x	 //y   //z
		0.8f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	// Generating and selecting vertex array object, this determines how we move through VBO
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);
	
	// Generating and selecting vertex buffer object, this is what will hold our data
	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	// Setting VBO and VAO 

		// Setting VBO
	glBufferData(GL_ARRAY_BUFFER, VertexPositionList.size() * sizeof(GLfloat), VertexPositionList.data(), GL_STATIC_DRAW); 
		// Setting VAO
	glEnableVertexAttribArray(0); // This enables vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	

	// Unbinding and Diabling stuff so that we don't write to them again
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
}


// Main loop functions
void mainloop() {
	while (!gQuit) {

		Input();

		PreDraw();

		Draw();

		// Swaps Buffer Window with displaying window
		SDL_GL_SwapWindow(MyWindow);

	}

}


void Input() {
	SDL_Event e;

	// We put events which we want to happen every frame regardless of user input here too.
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			std::cout << "GoodBye" << std::endl;
			gQuit = true;
		}
	}
}

void PreDraw() {}

void Draw() {}

// Cleanup Functions
void cleanup() {
	SDL_DestroyWindow(MyWindow);
	SDL_Quit();
}

