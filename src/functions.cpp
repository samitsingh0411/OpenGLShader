#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include "project/fucntions.h"
#include <vector>

// WIndow and Context
extern SDL_Window* MyWindow;
extern SDL_GLContext MyContext;
extern bool gQuit;

// VBO and VAO
extern GLuint gVertexArrayObject;
extern GLuint gVertexBufferObject;

// Graphics Pipeline
extern GLuint gGraphicsPipelineProgram;

const char* gVertexShaderSource =
"#version 460 core\n"
"in vec4 position;\n"
"void main() {\n"
"	gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
"}\n";

const char* gFragmentShaderSource =
"#version 460 core\n"
"out vec4 color;\n"
"void main() {\n"
"	color = vec4(1.0f, 0.5f, 0.0f, 1);\n"
"}\n";

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

// Creating Pipeline
GLuint CompileShader(GLuint ShaderType, const char* ShaderSource) {

	GLuint ShaderObject;
	
	if (ShaderType == GL_VERTEX_SHADER) {
		ShaderObject = glCreateShader(ShaderType);
	}

	else if (ShaderType == GL_FRAGMENT_SHADER) {
		ShaderObject = glCreateShader(ShaderType);
	}

	glShaderSource(ShaderObject, 1, &ShaderSource, nullptr);
	glCompileShader(ShaderObject);

	return ShaderObject;
}

GLuint CreateShaderProgram(const char* VertexShaderSource, const char* FragmentShaderSrouce) {

	// Creating an program object which is going to hold the program
	GLuint ProgramObject = glCreateProgram();
	
	// Creating a vertex and fragment shader object

	GLuint VertexShaderObject = CompileShader(GL_VERTEX_SHADER, gVertexShaderSource);
	GLuint FragemtnShaderObject = CompileShader(GL_FRAGMENT_SHADER, gFragmentShaderSource);

	glAttachShader(ProgramObject, VertexShaderObject);
	glAttachShader(ProgramObject, FragemtnShaderObject);

	// Verifying 
	glValidateProgram(ProgramObject);
	
	return ProgramObject;

}

// Creating the graphics pipleline
void CreateGraphicsPipeline() {
	gGraphicsPipelineProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

// Creating Vertex Specs	
void CreateVertexSpecs() {

	// Declaring our vertex data
	const std::vector<GLfloat> VertexPositionList = {
		//x	 //y   //z
		0.8f,  0.0f,  0.0f,
		-0.8f, 0.0f,  0.0f,
		0.0f,  0.8f,  0.0f
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

void PreDraw() {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glViewport(0, 0, 640, 480);
	glClearColor(1.f, 1.f, 0.f, 0.75f);

	glUseProgram(gGraphicsPipelineProgram);
}

void Draw() {

	glBindVertexArray(gVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Cleanup Functions
void cleanup() {
	SDL_DestroyWindow(MyWindow);
	SDL_Quit();
}

